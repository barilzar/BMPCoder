/*
Purpose: This modul contains functions to help
	 the process of reading and writing bitmap
	 files.

Functions:
	unsigned int skipBytes(FILE*, unsigned int)
	BMP_FILE* openBmp(char*)
	void closeBmp(BMP_FILE*)
	int parseHeader(BMP_FILE*)
	int parseData(BMP_FILE*)
	int writeToFile(BMP_FILE*, char*)
	unsigned int dataSize(BMP_FILE*)

Dependancies: Uses the functions:
	      unsigned int toInteger(byte*)
	      unsigned short toShort(byte*)
	      from the bitModul-library.
*/

#define NOT_VALID_ERROR(p)\
		p->error = NOT_VALID_BITMAP_ERROR;\
		return 0

#define NULL_FILE_ERROR(p)\
		p->error = NULL_FILE_ERROR;\
		return  0

#define MEMORY_FORMAT_ERROR(p)\
		p->error = UNSUPPORTED_MEMORY_FORMAT_ERROR;\
		return 0

#define MEMORY_ALLOCATION_ERROR(p)\
		p->error = MEMORY_ALLOCATION_ERROR;\
		return 0

#define FILE_WRITING_ERROR(p)\
		p->error = FILE_WRITING_ERROR;\
		return 0

#define HEADER_NOT_PARSED_ERROR(p)\
		p->error = HEADER_NOT_PARSED;\
		return 0

/********************************************
Enum: ERROR_NO

Purpose: This enum is used to represent all the different
	 kind of error conditions the functions manipulating
	 the BMP_FILE struct can achieve.
	 In case of an error these functions will change the
	 value in the structs error variable to mach the 
	 occured error. The value inserted in the variable in 
	 specified by this enum.
********************************************/
typedef enum{
	NO_ERROR,			//No error in struct
	NOT_VALID_BITMAP_ERROR,		//The file in the struct is not a valid bmp
	NULL_FILE_ERROR,		//The stream in the struct is NULL
	UNSUPPORTED_MEMORY_FORMAT_ERROR,//The memory format in this machine is invalid
	MEMORY_ALLOCATION_ERROR,	//A malloc operatio returnes NULL
	FILE_WRITING_ERROR,		//There was an error while writing to a file
	HEADER_NOT_PARSED		//The header needs to be parsed for this function
}ERROR_NO;

/********************************************
Struct: BMP_FILE

Purpose: This struct is used as storage for some of the values 
	 that can be read from the headers of .bmp files. Once 
	 this struct has been succesfully parsed there is no need 
	 for reading the original file again (this can be done with 
	 the parseHeader()-function).
	 The struct also has funcionality for storing the raw bitmap 
	 data from the file (this can be done with the parseData()-function)

Usage: You should not create these structs manually. Instead you should
       use only the functions provided within this module.
       Also changing these values at runtime can lead into some unwanted 
       functionality 
********************************************/
typedef struct{
	unsigned int fSize;      	//The file size of this bitmap
	unsigned int offset;     	//The start of the bitmap data
	unsigned int hSize;  	 	//The size of the file header of this bitmap
	unsigned int imgSize;	 	//The size of the bitmap data
	unsigned int bpp;	 	//The amount of bits per pixel in this bitmap
	unsigned int compression; 	//The compression used in this bitmap
	int width;			//The width of this bitmap
	int height;			//The height of this bitmap

	byte padder;			//The byte used for padding by this bitmap
	unsigned short padding; 	//The amount of padding bytes used in this bitmap
	int headerParsed;		//Is 1 if the header has been parsed 0 otherwise

	byte* data;			//The bitmap data of this bitmap
	
	ERROR_NO error;			//The error in this bitmap
	FILE* fileHandle;		//The file handle of this bitmap
}BMP_FILE;

/********************************************
Function: skipBytes(FILE*, unsigned int)

Purpose: Reads n bytes from the file specified.
	 Since the read bytes are not saved anywhere
	 this function basically just advances the file 
	 pointer.

Inputs: A pointer to the file wich's bytes should
	be read (can be null).
	An unsigned int specifying the amount of chars
	to skip (can be 0).

Returns: The number of bytes skipped or 0 if the 
	 file was NULL or something else if EOF was 
	 reached while reading,
	 In other words the return value will be n only
	 when the skip was succesfull.

Modifies: Advances the file pointer.

Error checking: Checks for a NULL file and handles the EOF
		situation.

Sample call: unsigned int i = skipBytes(file, amount);
	     if(i != amount)
	     ...error...
	     else
	     ...success...

********************************************/
unsigned int skipBytes(FILE*, unsigned int);

/********************************************
Function: openBmp(char*)

Purpose: Creates a new BMP_FILE struct based on
	 the given filepath.
	 Any of the values in the returned struct
	 WILL NOT be iniatilized, these must be parsed
	 seperately with the parseHeader()-function.

Inputs: A char array (=string) representing a valid
	file path.

Returns: A pointer to a new BMP_FILE struct based on
	 the given file path.
	 A NULL-value will be returned if the file could
	 not be opened, or if the memory allocation for the
	 struct was not successfull.

Modifies: Reserves memory for the created struct with 
	  the malloc-function. This can be later freed
	  with the closeBmp()-function.

Error checking: Checks that a file representing the given
		path could be opened.
		Also incase the malloc-operation was 
		unsuccessull a NULL-value will be returned.

Sample call: BMP_FILE* file = openBmp("filepath");
********************************************/
BMP_FILE* openBmp(char*);

/********************************************
Function: closeBmp(BMP_FILE*)

Purpose: Frees all the resources used by the given
	 BMP_FILE struct.
	 This function works for all phases of the
	 BMP_FILE (after the initial creation, after
	 header parsing and after data parsing).

Inputs: A pointer to the BMP_FILE to be closed.

Returns: Nothing.

Modifies: Frees the memory used by the given BMP_FILE 
	  struct and it's data pointer.
	  The stream to the file on wich the struct is based on
	  will also be closed.

Error checking: Checks that the given pointer is not NULL,
		that the file pointer in the struct is not NULL
		and that the data pointer is not NULL.

Sample call: closeBmp(file);
********************************************/
void closeBmp(BMP_FILE*);

/********************************************
Function: parseHeader(BMP_FILE*)

Purpose: Reads the header data from the file stream
	 within the given BMP_FILE struct and stores
	 it to the struct.
	 Works only on machines with:
	 sizeof(int) == 4 and sizeof(short) == 2
	 if this is not the case the function will report
	 an error.

Inputs: A pointer to the BMP_FILE wich header should be
	parsed.
	The pointer MUST point to struct created with the
	openBmp()-function.

Returns: 1 if the operation was succesfull 0 otherwise.
	 If 0 was returned you can check the value in
	 the error-variable within the struct for a more exact
	 error code as specified by the ERROR_NO enum.

Modifies: Overwrites the current values in the given struct.
	  Advances the file pointer within the given struct.

Error checking: Reports of an error if:
		the file handle in the struct is NULL,
		the memory format of the current machine is invalid,
		the file in the struct does is not a valid bitmap file.

Sample call: if(parseHeader(file))
		...success...
	     else
		...failure...
********************************************/
int parseHeader(BMP_FILE*);

/********************************************
Function: parseData(BMP_FILE*)

Purpose: Parses the bitmap data from the file in
	 the given struct to the struct.
	 The possible padding bytes in the file 
	 will be removed.
	 This function only works for 24 bpp bitmaps.

Inputs: A pointer the struct to wich the parsing should 
	be done.

Returns: 1 on success, 0 otherwise.
	 If 0 was returned a more specific description of
	 the error can be obtained from the error variable
	 in the given struct, the value is specified by the
	 ERROR_NO enum.

Modifies: Frees the previous area pointed by the structs
	  data pointer and reserves a new one.
	  Advances the file pointer in the given struct.

Error checking: Reports an error if:
		the given struct was NULL,
		the bpp of the file is not 24,
		the header for the given struct has not been parsed,
		the file handle in the struct is NULL,
		the memory allocation for the file data was unsuccessfull,
		the file in the struct is not a valid bitmap file.

Sample call: if(parseData(file))
		...success...
	     else
		...failure...
********************************************/
int parseData(BMP_FILE*);

/********************************************
Function: writeToFile(BMP_FILE*, char*)

Purpose: Writes the given BMP_FILE struct to the
	 file specified by the given filepath.
	 Changes to the files header data WILL NOT BE 
	 SAVED, only changes in the data.

Inputs: A BMP_FILE struct to be written.
	A string specifying the file path where to write.
	The given path MUST NOT BE THE SAME as with the file
	in the struct.

Returns: 1 on success 0 otherwise.
	 If 0 was returned a more specific description of
	 the error can be obtained from the error variable
	 in the given struct, the value is specified by the
	 ERROR_NO enum.

Modifies: Overwites the file specified by the given filepath.

Error checking: Reports an error if:
		the given struct was NULL,
		the file specified by the given path could not be opened,
		the file in the given struct was NULL,
		the file in the given struct is not a valid bitmap file,
		there was an error when writing to the new file

Sample call: if(writeToFile(file, "new filepath"))
		...success...
	     else
		...failure...
********************************************/
int writeToFile(BMP_FILE*, char*);

/********************************************
Function: dataSize(BMP_FILE*)

Purpose: Returns the size of the data memory area
	 in the given struct.
	 Note though that this function will return
	 a value even if the data area has not been
	 reserved.

Inputs: The BMP_FILE wich's data area's you want to 
	know.

Returns: The size of the data area in the given 
	 BMP_FILE.

Modifies: Nothing.

Error checking: None.

Sample call:
	with this code you can safely access all the values
	in the data area.

	byte b;
	for(int i = 0; i < dataSize(myBMP_FILE); i++)
		b = myBMP_FILE->data[i];
		... do something for b...

********************************************/
unsigned int dataSize(BMP_FILE*);
