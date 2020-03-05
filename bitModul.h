#include <stdint.h>
/*
Purpose: 
	This library contains several byte level
	functions. Most of these are used for 
	encoding and decoding chars to some data.
	There are also a few functions for parsing
	datatypes from raw binary data.

Functions:
	void toBits(char, byte*)
	char fromBits(byte*)
	void encode(byte*, char)
	char decode(byte*)
	int isBigEndian()
	unsigned int toInteger(byte*)
	unsigned short toShort(byte*)
	void encodeData(byte*, char*)
	char* decodeData(byte*, int)

Dependancies: None.
*/

/********************************************
Function: toBits(char, byte*)

Purpose: converts the given character to its binary 
	 representation.

Inputs: the char to be converted and a pointer 
	to a memory area where the binary values 
	should be stored.
	The given memory block MUST HAVE SPACE FOR 
	ATLEAST 8 characters. Defy this at your own peril.

Returns: nothing.

Modifies: the memory area given as a parameter
	  will be overwritten by this function.

Error checking: nothing.

Sample call: tobits('a', myArray)
	     After the call myArray would look like this:
	     myArray[0]= (0000 0000)
	     myArray[1]= (0000 0001)
	     myArray[2]= (0000 0001)
	     myArray[3]= (0000 0000)
	     myArray[4]= (0000 0000)
	     myArray[5]= (0000 0000)
	     myArray[6]= (0000 0000)
	     myArray[7]= (0000 0001)
	     The binary representation for 'a' is (0110 0001)
	     wich is what this function produces to the last 
	     diagonal of myArray.
*********************************************/
void toBits(char, uint8_t*);

/********************************************
Function: fromBits(byte*)

Purpose: Converts the given bits to a character.

Inputs: The pointer argument must point to a memory
	area with ATLEAST 8 chars.
	Each char within the area must contain either
	0x00 or 0x01 in other cases the return value is
	unspecified.

Returns: A character representation of the given bits.

Modifies: Nothing.

Error checking: Nothing.

Sample call: char c = fromBits(myArray);
	     The return value is allways case dependant.
********************************************/
char fromBits(uint8_t*);

/********************************************
Function: encode(byte*, char)

Purpose: Encodes the argument character to the bytes
	 within the memory area specified by the byte 
	 pointer.

Inputs: Character to be encoded and a pointer to the memory
	area where the encoding should be done.
	THE MEMORY AREA MUST BE ATLEAST 8 BYTES LONG

Returns: Nothing.

Modifies: The memory area specified by the pointer will be
	  overwritten.

Error checking: None.

Sample call: encode(myArray, 'a');
	     After the call myArray would look like this:
	     myArray[0]= (xxxx xxx0)
	     myArray[1]= (xxxx xxx1)
	     myArray[2]= (xxxx xxx1)
	     myArray[3]= (xxxx xxx0)
	     myArray[4]= (xxxx xxx0)
	     myArray[5]= (xxxx xxx0)
	     myArray[6]= (xxxx xxx0)
	     myArray[7]= (xxxx xxx1)
	     Basically the last bits of the bytes in myArray
	     will be replaced by bits representing the letter 'a'.
	     The data in myArray will not be chanced in any ther way.
********************************************/
void encode(uint8_t*, char);

/********************************************
Function: decode(byte*)

Purpose: Decodes a character from the given memory area.
	 If the memory area specified has not been encoded
	 by the encode(byte*, char)-function the return value
	 is unspecified.

Inputs: A memory area with ATLEAST 8 CHARACTERS.

Returns: A character representing the bits encoded to the
	 given memory area.

Modifies: Nothing.

Error checking: Nothing.

Sample call: char c = decode(myArray);
	     The return value is allways case dependant.
********************************************/
char decode(uint8_t*);

/********************************************
Function: isBigEndian()

Purpose: Tells what architecture this machine uses.
	 The little- or the big-endian architecture.

Inputs: Nothing.

Returns: 1 if this machine is big-endinan, 0 otherwise
	 (=this machine is little-endian.)

Modifies: Nothing.

Error checking: None.

Sample call: if(isBigEndina)
		... code for big-endian cases...
	     else
		... code for little-endian cases...
********************************************/
int isBigEndian();

/********************************************
Function: toInteger(byte*)

Purpose: Turns the given LITTLE-ENDIAN byte array
	 to the unsigned int -datatype used by
	 this machine.

Inputs: An array of bytes representing an unsigned
	integer in little-endian format.
	The array must have enough bytes to fill the
	datatype, e.g. the amount specified by 
	sizeof(unsigned int).
	
Returns: An unsigned integer with the value specified
	 by the given byte array. 

Modifies: Nothing

Error checking: None.

Sample call: unsigned int i = toInteger(myArray);
********************************************/
unsigned int toInteger(uint8_t*);

/********************************************
Function: toShort(byte*)

Purpose: Returns an unsigned short value of the
	 given little-endian byte array.

Inputs: A byte array to be converted in LITTLE-ENDIAN
	format.
	The array must contain enough bytes to be
	considered a valid representation of an unsigned 
	short, e.g. the amount specified by 
	sizeof(unsigned short).

Returns: An unsigned short representing the given array.

Modifies: Nothing.

Error checking: None.

Sample call: unsigned short s = toShort(myArray);
********************************************/
unsigned short toShort(uint8_t*);

/********************************************
Function: encodeData(byte*, char*)

Purpose: Encodes (as specified by the encode()-function)
	 the given string (char*) to the given byte area.

Inputs: The area where the encoding should be done and
	the message to be encoded.
	The given message MUST BE NULL TERMINATED! If not
	this function will get stuck on an infinite loop.
	You need to make sure the byte area is large enough
	to receive the message. Each character in the message
	requires atleast 8 bytes for encoding. There needs to
	be space for the terminating null-character as well.

Returns: Nothing.

Modifies: Overwrites some of the data in the given 
	  byte-array.

Error checking: None.

Sample call: encodeData(myArray, "message to be encoded");
********************************************/
void encodeData(uint8_t*, char*);

/********************************************
Function: decodeData(byte*, int)

Purpose: Decodes a message from the given data
	 area. The decoding will be successfull
	 only if the encodeData-function has been
	 used for encoding the data.
	 If the encodeData()-function has not been
	 used this function can cause some very strange
	 errors...

Inputs: The area (byte*) where the decoding operation
	should be done.
	The int-parameter must specify the maximum lenght
	for THE MESSAGE TO BE DECODED. This can be counted
	by dividing the lenght of the given byte array
	by 8.

Returns: A pointer to the start of the decoded message.
	 This string will be null-terminated only if a null-
	 character was found while performing the decoding
	 operation (there will allways be one if the encodeData-
	 function was used for encoding).

Modifies: Reserves memory from the heap for the returned
	  string, you must free this memory later by yourself.

Error checking: None.

Sample call: char* message = decodeData(myArray, myArrayL / 8)
********************************************/
char* decodeData(uint8_t*, int);
