/*First file for file ops in c language*/

#include <stdio.h>
#include <string.h>
#include "fops1_switch.h"

#define DEF_FILE_NAME "Example.txt"
extern char *filename;
int main(int argc, char const *argv[])
{
/*In the starting line of code we are keeping a user input option to input a filename
	if user don't give any file name then the default file name will be taken
	if provided then the user-defined filename will be taken*/
	if (argc == 1)
	{
		FILE *fp = fopen(DEF_FILE_NAME, "r");
		if (fp)
		{
			printf("\n File opend in read mode");
			printf("\n Default File name is %s \n", DEF_FILE_NAME);
			fclose(fp);	
		}
		if (!fp)
		{
			FILE *fp = fopen(DEF_FILE_NAME, "w");			
			printf("\n New File created");
			fclose(fp);	
		}//if ends
		else
			printf("Cannot create a file\n");
	}//if ends
	else
	{
		filename = (char*)calloc(strlen(argv[1]+5),1);
		// memcpy(filename,argv[1], strlen(argv[1]));
		// strcat(filename, ".txt");
		sprintf(filename,"%s%s",argv[1],".txt");
		FILE *fp = fopen(filename, "r");	
		if (fp)
		{
			printf("\n File available");
			printf("\n File name is %s \n", DEF_FILE_NAME);
			fclose(fp);	

		}
		if (!fp)
		{
			FILE *fp = fopen(*argv[1], "w");			
			printf("\n New File created");
			fclose(fp);	
		}//if ends
		else
			printf("Cannot create a file\n");	
	}//else ends

switch_case();

	return 0;
}
