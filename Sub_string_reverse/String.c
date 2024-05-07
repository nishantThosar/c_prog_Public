/*Program to pick the last word of the string*/

#include<string.h>
#include <stdio.h>
#define SIZE 40
int count=0;

void print(char [], int);

int main(int argc, char const *argv[])
{

	int count=0;
	char arr[SIZE];
	printf("\nEnter the string: ");
	fgets(arr,SIZE,stdin);
	for (int i = 0; arr[i] != '\0'; ++i)
	{
		count++;
	}
	print(arr, count);
	return 0;
}//main ends

void print(char arr[], int count)
{

	int i = count;
	while(i)
	{
		printf("%c", arr[i-1]);
		i--;
	}//while ends

}//print ends