#include <stdlib.h>
#include <string.h>
#include "bitModul.h"

void toBits(char c, byte* bits){
	byte mask = 0x01;

	for(int i = 7; i >= 0; i--){
		bits[i] = c & mask;
		bits[i] >>= -(i - 7);
		mask <<= 1;
	}

}

char fromBits(byte* bits){
	byte c = 0x00,
	     bit;

	for(int i = 0; i < 8; i++){
		bit = bits[i] << -(i - 7);
		c |= bit;
	}
	return c;
}

void encode(byte* bytes, char c){
	byte bits[8];
	toBits(c, bits);

	for(int i = 0; i < 8; i++){
		//We "drop" the last byte of:
		bytes[i] >>= 1;
		bytes[i] <<= 1;

		//and replece it with one of our own:
		bytes[i] |= bits[i];
	}
}

char decode(byte* bytes){
	byte bits[8],
	     mask = 0x01;

	for(int i = 0; i < 8; i++){
		bits[i] = bytes[i] & mask;
	}
	return fromBits(bits);
}

int isBigEndian(){
	int i = 1;

	//We convert the integer i to a byte array:
	byte* p = (byte*) &i;

	/* If this machine is little-endian the first byte
	 * of the integer i will have it's least significand
	 * byte first. And when i = 1 the first byte will have 
	 * the value 1.
	 */
	if(*p == 1)//little-endian
		return 0;
	else//big-endian
		return 1;
}

unsigned int toInteger(byte* bytes){
	unsigned int i;

	//We convert the integer i to a byte array:
	byte* data = (byte*) &i;

	//If this machine is little endian a simple copy is enough:
	if(!isBigEndian()){
		for(unsigned int j = 0; j < sizeof(int); j++)
			data[j] = bytes[j];
	}
	//If this machine is big-endian we need to reverse the bytes:
	else{
		for(unsigned int j = 0; j < sizeof(int); j++)
			data[j] = bytes[sizeof(int)- 1 - j];
	}
	return i;
}

unsigned short toShort(byte* bytes){
	//This function is completely analogous to the 
	//toInteger()-function

	unsigned short s;
	byte* data = (byte*) &s;

	if(!isBigEndian()){
		for(unsigned int j = 0; j < sizeof(short); j++)
			data[j] = bytes[j];
	}
	else{
		for(unsigned int j = 0; j < sizeof(short); j++)
			data[j] = bytes[sizeof(short)- 1 - j];
	}
	return s;
}

void encodeData(byte* area, char* message){
	if(area == NULL || message == NULL)
		return;

	int i = 0;

	//We encode characters until a null-character is found:
	while(message[i] != '\0'){
		encode(&area[i * 8], message[i]);
		i++;
	}

	//Encodes the final null-character:
	encode(&area[(i + 1) * 8], '\0');
}

char* decodeData(byte* area, int maxLenght){

	char* message = malloc(sizeof(char) * maxLenght);
	if(message == NULL)
		return NULL;

	for(int i = 0; i < maxLenght; i++){
		//We decode characters until a null-chracter is found:
		message[i] = decode(&area[i * 8]);
		if(message[i] == '\0')
			break;
	}

	return message;
}
