#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "bitModul.h"
#include "bmpFileParser.h"

int main(void){

	//Test for the de- and encoding of strings
	char* string = "höpönpöpön pullamössöystäänkäänmätkä\0";

	//Data area where coding:
	size_t size = 8 * (strlen(string) + 1);
	byte* area= malloc(sizeof(char) * size);
	for(unsigned int i = 0; i < size; i++){
		area[i] = (byte) (rand() * time(0));
	}

	printf("Encoding string: %s\n", string);
	encodeData(area, string);

	printf("Decoded string: %s\n", decodeData(area, strlen(string) + 1));
	free(area);
	
	printf("Starting the bmp test:\n");
	//Specify the path for the bitmap to be used:
	BMP_FILE* file = openBmp("testiKuva2.bmp");

	if(file == NULL)
		return 0;

	int er1 = parseHeader(file);
	int er2 = parseData(file);

	printf("Errors on parsing:\n");
	printf("header: %i, Data: %i\n", er1, er2);

	printf("The header data of this file:\n");
	printf("File size: %u\n", file->fSize);
	printf("File offset: %u\n", file->offset);
	printf("File header size: %u\n", file->hSize);
	printf("Image data size: %u\n", file->imgSize);
	printf("Image bpp: %u\n", file->bpp);
	printf("Image compression: %u\n", file->compression);
	printf("Image width: %i\n", file->width);
	printf("Image height: %i\n", file->height);
	printf("File error: %i\n", file->error);
	printf("Paddin byte: %x amount: %u\n", file->padder, file->padding);
	printf("Some image data:\n");

	//Choose the i by yourself
	for(int i = 0; i < 20; i++){
		printf("%x", file->data[i]);
	}
	
	//Testing the filewrite:
	er1 = writeToFile(file, "kopio");
	printf("\nError on writing: %i\n", er1);

	closeBmp(file);
	return(EXIT_SUCCESS);
}
