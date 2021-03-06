#include <stdlib.h>
#include <stdio.h>
#include "bitModul.h"
#include "bmpFileParser.h"

unsigned int skipBytes(FILE* file, unsigned int n){
	if(file == NULL || n == 0)
		return 0;

	unsigned int i;
	for(i = 0; i < n; i++)
		if(fgetc(file) == EOF)
			break;

	return i;
}

BMP_FILE* openBmp(char* fileName){
	FILE* file;
	BMP_FILE* p;

	if((file = fopen(fileName, "r+b")) == NULL){
		return NULL;
	}
	if((p = malloc(sizeof(BMP_FILE))) == NULL){
		return NULL;
	}

	p->fileHandle = file;
	p->data = NULL;
	p->error = NO_ERROR;
	p->headerParsed = 0;
	
	return p;
}

void closeBmp(BMP_FILE* p){
	if(p == NULL)
		return;	

	if(p->data != NULL)
		free(p->data);

	if(p->fileHandle != NULL)
		fclose(p->fileHandle);

	free(p);
}

int parseHeader(BMP_FILE* file){
	if(file == NULL)
		return 0;

	if(file->fileHandle == NULL){
		NULL_FILE_ERROR(file);
	}

	rewind(file->fileHandle);
	uint8_t buffer[18];

	//Reads the first 18 bytes from the file to the buffer.
	//fread returns the amount of bytes read wich we require to be 18.
	if(fread(&buffer, sizeof(uint8_t), 18, file->fileHandle) != 18){
		NOT_VALID_ERROR(file);
	}

	//Checks that the file starts with the letters BM
	if(buffer[0] != 0x42 || buffer[1] != 0x4D){
		NOT_VALID_ERROR(file);
	}

	//The first part of the header parsing:
	file->fSize  = toUInt(&buffer[2]);
	file->offset = toUInt(&buffer[10]);
	file->hSize  = toUInt(&buffer[14]);

	// We reserve space for the rest of the header
	// and read the rest of the header data to it.
	uint8_t headerData[file->hSize];
	if(fread(&headerData, sizeof(uint8_t), file->hSize, file->fileHandle) != file->hSize){
		NOT_VALID_ERROR(file);
	}

	//The final part of the header parsing:
	file->width 	  = toUInt(&headerData[0]);
	file->height 	  = toUInt(&headerData[4]);
	file->bpp 		  = toUShort(&headerData[10]);
	file->compression = toUInt(&headerData[12]);
	file->imgSize 	  = toUInt(&headerData[16]);

	/* Each line of a bmp file is padded to be divisible by 32.
	 * The following formula counts the amount of bytes needed
	 * to pad the lines to this limit.
	 * The amount of bytes is either 0, 1, 2 or 3.
	 */
	file->padding = (32 - ((file->width *  file->bpp) % 32)) / 8;
	if(file->padding == 4)
		file->padding = 0;

	file->error = NO_ERROR;
	file->headerParsed = 1;	

	return 1;
}

int parseData(BMP_FILE* file){
	if(file == NULL)
		return 0;

	if(file->headerParsed != 1){
		HEADER_NOT_PARSED_ERROR(file);
	}

	if(file->fileHandle == NULL){
		NULL_FILE_ERROR(file);
	}

	if(file->bpp != 24){
		NOT_VALID_ERROR(file);
	}

	if(file->data != NULL)
		free(file->data);

	if((file->data = malloc(dataSize(file))) == NULL){
		MEMORY_ALLOCATION_ERROR(file);
	}

	rewind(file->fileHandle);

	//We try to skip the header part of the file:
	if((skipBytes(file->fileHandle, file->offset)) != file->offset){
		NOT_VALID_ERROR(file);
	}

	int read  = 0, //The char read from file
	    index = 0; //The current index of the data array.

	for(int i = 0; i < file->height; i++){
		//file->widht * 3 tells the amount of bytes on each line of this bitmap.
		for(int j = 0; j < file->width * 3; j++){
			if((read = fgetc(file->fileHandle)) == EOF){
				NOT_VALID_ERROR(file);
			}
			file->data[index] = (uint8_t) read;
			index++;
		}
		//On the first line we will also read the byte used for padding:
		if(i == 0 && file->padding != 0){
			if((read = fgetc(file->fileHandle)) == EOF){
				NOT_VALID_ERROR(file);
			}
			file->padder = (uint8_t) read;
			skipBytes(file->fileHandle, file->padding - 1);
			continue;
		}
		//Skips the padding bytes in this line:
		if(skipBytes(file->fileHandle, file->padding) != file->padding){
			NOT_VALID_ERROR(file);
		}
	}
	file->error = NO_ERROR;
	return 1;
}

int writeToFile(BMP_FILE* file, char* fname){
	FILE* output;
	int read, index = 0;
	
	if(file == NULL)
		return 0;

	if(file->fileHandle == NULL){
		NULL_FILE_ERROR(file);
	}
	if((output = fopen(fname, "w+b")) == NULL){
		FILE_WRITING_ERROR(file);
	}

	rewind(file->fileHandle);

	/*
	The header is copied straight from the original file.
	This way we don't need to worry about the validity of
	the header if it has been changed.
	*/
	for(unsigned int i = 0; i < 14 + file->hSize; i++){
		read = fgetc(file->fileHandle);
		if(read == EOF){
			NOT_VALID_ERROR(file);
		}
		if(fputc(read, output) == EOF){
			FILE_WRITING_ERROR(file);
		}
	}

	for(int i = 0; i < file->height; i++){
		for(int j = 0; j < file->width * 3; j++){
			//writes the current line of data to the file.
			if(fputc(file->data[index], output) == EOF){
				FILE_WRITING_ERROR(file);
			}
			index++;
		}
		//Writes the padding bytes
		for(int j = 0; j < file->padding; j++)
			if(fputc(file->padder, output) == EOF){
				FILE_WRITING_ERROR(file);
			}
	}
	fclose(output);
	file->error = NO_ERROR;
	return 1;
}

unsigned int dataSize(BMP_FILE* file){
	return file->imgSize - file->height * file->padding;
}
