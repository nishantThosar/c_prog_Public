/*This program shows how functoin can be modified using const identifier*/

#include <stdio.h>
void print(int);				//func prototype
const int* ret(int);			//func ptorotype

int main(void)
{
	int a = 10;
	print(a);
	
	const int *ptr = ret(a); //this is where the const function is called.
	printf("%d\n", *ptr);
	return 0;
}//main ends

void const print(int a) //here the const kyword is neglected by complier.
{
	printf("%d\n",a );
}//print function ends


/*The following way shows how properly we can use the const keyword in function declaration.*/
const int* ret(int z)
{
	static const int value = 45; //static keyword is used to keep the data in the value variable persistent, else it will give seg fault.
	return &value;
}//ret function ends
