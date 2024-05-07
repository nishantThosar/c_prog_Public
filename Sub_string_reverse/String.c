/*Program to pick the last word of the string*/

#include<string.h>
#include <stdio.h>

int count=0;

int main(int argc, char const *argv[])
{

	char arr[10] = "Gupta Ji";
	for (int i = 0;  arr[i] != '\0'; i++)
	{
		++count;
		if(arr[i] == ' ')
		{
			for (int i = count+1; i >= 0; i--)
			{
				printf("%c", arr[i]);

			}//for ends
		}//if ends		
	}//for ends
	return 0;
}//main ends