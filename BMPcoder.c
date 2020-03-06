#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "bitModul.h"
#include "bmpFileParser.h"

//Prints a message explaining the use of this program.
void help(){
	printf("You must specify atleast 2 parameters.\n");
	printf("The first parameter must specify the operation to be conducted, decoding(-d) or encoding(-e). The second parameter must specify the file on wich the operation will be applied.\n");
	printf("e.g.\nBMPcoder -e normalBitmap.bmp or\n");
	printf("BMPcoder -d BMPwithMessage.bmp\n");
}

//Prints (hopefully) a helpfull error message.
void error(BMP_FILE* file){

	if(file == NULL){
		printf("The file specified does not exist.\n");
		return;	
	}

	switch(file->error){
	
		case NO_ERROR:
			break;
		
		case NOT_VALID_BITMAP_ERROR :
			puts("The file supplied to the program was not a valid bitmap file. Either the file was not a bitmap file at all, or it was of an unsupported format.\nIt is also possible that the file has allready got a message encoded within.\n");
			break;

		case NULL_FILE_ERROR :
			puts("Internal program error.\nThe filehandle within the BMP_FILE struct was NULL.\n");
			break;

		case UNSUPPORTED_MEMORY_FORMAT_ERROR :
			puts("The current machine architecture used does not support the functions used by this program. Running this program successfully is impossible.\n");
			break;

		case MEMORY_ALLOCATION_ERROR :
			puts("There is not enough free memory on the system for the program to function properly. Consider trying to free some memory and then trying again.\n");
			break;

		case FILE_WRITING_ERROR:
			puts("There was an error while writing to a file. Check that the currently open directory does not have a file named encodedBitmap.bmp inside.\n");
			break;

		case HEADER_NOT_PARSED :
			puts("Internal program error.\nA function that requires the BMP_FILE's header to be parsed received a BMP_FILE wichs header was not parsesd.\n");
			break;

		default:
			puts("Internal program error.\nError function called on a BMP_FILE with an unknown value in the error variable.\n");
	}

	closeBmp(file);
}

//Parses a BMP_FILE struct from the given filename and checks 
//for various error conditions.
int bmpErrors(char* fName, BMP_FILE** fileP){
	if(!(*fileP = openBmp(fName))){
		error(*fileP);
		return 0;
	}
	else if(!parseHeader(*fileP)){
		error(*fileP);
		return 0;
	}
	else if((*fileP)->bpp != 24 || (*fileP)->compression != 0){
		(*fileP)->error = NOT_VALID_BITMAP_ERROR;
		error(*fileP);
		return 0;
	}
	else if(!parseData(*fileP)){
		error(*fileP);
		return 0;
	}

	return 1;
}

//Handles the operation for encoding a message to a file.
void encodeOperation(char* fName){
	BMP_FILE* file = NULL;
	
	if(!bmpErrors(fName, &file))
		return;

	unsigned int maxLenght = dataSize(file) / 8 ;
	
	char* buffer = malloc(maxLenght);
	if(buffer == NULL){

		puts("Not enough memory available for operations.\nTerminating program.");
		return;
	}
	
	printf("Please enter the message to be encoded (max. %u characters)\n", maxLenght);
	fgets(buffer, maxLenght, stdin);

	if(buffer == NULL){
		//Error while reading from stdin
		puts("There was an error while reading the input.\nTerminating program.\n");
		exit(EXIT_FAILURE);
	}

	encodeData(file->data, buffer);
	if(!writeToFile(file, "encodedBitmap.bmp")){
		error(file);
		return;
	}

	closeBmp(file);
	free(buffer);
}

//Handles the operation for decoding a message.
void decodeOperation(char* fName){
	BMP_FILE* file = NULL;
	char* message = NULL;
	
	if(!bmpErrors(fName, &file))
		return;

	message = decodeData(file->data, dataSize(file));

	if(message == NULL){
		file->error = MEMORY_ALLOCATION_ERROR;
		error(file);
		return;
	}

	printf("Message decoded from file %s:\n", fName);
	printf("%s\n", message);
	
	closeBmp(file);
	free(message);
}

int main(int argc, char** argv){

	switch (argc){

		case 1:
		case 2: help();
			break;

		case 3:
			if(strncasecmp(argv[1], "-e" , 2) == 0)
				encodeOperation(argv[2]);


			if(strncasecmp(argv[1], "-d", 2) == 0)
				decodeOperation(argv[2]);

			break;

		default: help();
	}
	return(EXIT_SUCCESS);
}
