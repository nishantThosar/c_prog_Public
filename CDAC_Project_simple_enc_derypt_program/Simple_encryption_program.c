/*Simple encryption program*/
/*
	Author: Nishant Thosar
*/
/*Rules: 
For encryption don't give any space between the name of file 
*eg: Filename = Ashish bansal.text (not allowed)
*eg: Filename = Ashish_bansal.text (is allowed)
*Curently program accepts filename from command line arguments but can be changed
*Do not change SHIFT value.
*Uncomment line no 22 for debugging.
*/

/*Testing:
*The program has been tested with 21 file formats (Given in test.txt) 
*To check the result copy and paste link: https://www.diffchecker.com/xSN7boU9/ 
*If you want to debug use command 'cat test.txt | xargs -L 1 ./simple_encrypt > result.txt'*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
// #define DEBUG            //uncomment for debugging
/*---------------------------------------FUNCTION PROTOTYPES---------------------------------------------*/
char* encryption(char[]);
char* decryption(char[]);
#define SHIFT 27 			// change this for changing the SHIFTing of alphabets

/*---------------------------------------GLOBAL VARIABLES---------------------------------------------*/
char string_encrypt[32];
char string_decrypt[32];
int size; 

/*---------------------------------------MAIN STARTS---------------------------------------------*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./encrypt <string>\n");
        return 1;
    }
    size = strlen(argv[1]);
#ifdef DEBUG
    printf("Sizeof input: %d\n",size); //debugger
#endif
    if (size > 32)
    {
    	printf("Size of filename grater than 32 characters\n");
    	return 1;
    }//if ends
    else
    {
	    char *encrypt= encryption(argv[1]);
    #ifdef DEBUG
	    printf("Encrypted: %s\n",encrypt);
    #endif
	    char *decrypt = decryption(encrypt);
    #ifdef DEBUG
	    printf("%s\n",decrypt);
    #endif
	}
    return 0;
}//main ends

/*---------------------------------------ENCRYPTION FUNCTION DEFINATION---------------------------------------------*/
char* encryption(char file_name[])
{
	char *string = file_name;
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (isalpha(string[i])) 
        {
            char base = islower(string[i]) ? 'a' : 'A';
            string[i] = ((string[i] - base + SHIFT) % 26) + base;
        }//if ends
    }//for ends    
#ifdef DEBUG
    printf("After first hashing: %s\n",string);
#endif    
    memcpy(string_encrypt,string,strlen(string));
#ifdef DEBUG    
    printf("String received in decrypt function: %s\n",string);
#endif
    for (int j=size; j < (size+6) && j<31 ; j++) //adding the salt
    {
    	string_encrypt[j] = string[j] + 'z'+9;
    }//for ends
	return string_encrypt;
}//encryption function ends

/*---------------------------------------DECRYPTION FUNCTION DEFINATION---------------------------------------------*/
char* decryption(char file_name[])
{
	char *string = file_name;
    for (int i = 0; string[i] != '\0'; i++) 
    {
        if (isalpha(string[i])) 
        {
            char base = islower(string[i]) ? 'a' : 'A';
            string[i] = ((string[i] - base - SHIFT + 26) % 26) + base;
        }
    }//for ends
    for (int i = 0; i < size && i < 32; ++i) //removing the salt
     {
     	string_decrypt[i] = string[i];
     } 
    return string_decrypt;
}//decryption function ends
/*_____________________________________________________ END ______________________________________________________*/