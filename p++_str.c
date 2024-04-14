//Program to understand pointer arithematic.

#include<stdio.h>
void access_by_ref( char *, int * );//Function prototyping.
void input (int *, char *, char*);
int main()
{
	int a;//defined int var.
	char b;//defined char var
	char d[10];// defined an arr of char (i.e: string)
	
	char array[] = "Hello world";//Already initilized array.

	int arr[] = {1,2,3,6,7,4,8,9,10};// already initilized int array

	access_by_ref(array, arr);// passing array of int & char data type in a function.
	input(&a, &b, d);// passing int variable, passing char variable & array of char int a function (Pass by refrence). 

	printf("\n\nInt value of a: %d", a);// Printing the values scanned from function.
	printf("\n\nChar value of b: %c", b);//Printing value of b scanned from fuction. 
	printf("\n\nString value of d: %s\n", d);// Printing array of char scanned by function.


}// main ends

//Pass by refrence.
 void access_by_ref(char *p, int *ptr)
{
	printf("p++: %c\n\n", *p++);

	printf("ptr++: %d\n\n", *ptr++);
	printf("Again ptr++: %d\n\n", *ptr++);
}// Access_by_ref ends.
 
void input (int *_ptr, char *ch, char *arr_pt)
{

	printf("\nEnter int value for a: ");
	scanf("%d",_ptr);
	printf("\nEnter char value for b: ");
	scanf(" %c",ch);
	printf("\nEnter string value for array: ");
	scanf(" %[^\n]s",arr_pt);

}// input ends.
