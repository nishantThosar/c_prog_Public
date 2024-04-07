// program for struct pointer.
#include<stdio.h>
struct pointer_struct{// struct globally declared.
	int a;
	float b;
	char c;

}name,*ptr;// intiilization of struct pointer & struct variable.

int main()
{
	ptr = &name;//we need to point the struct pointer to the struct variable like this.
	name.b = 2.02;
	ptr->a = 20;// accessing by pointer type.
	
	// printing by arrow notation as pointer is used to print..
	printf("\nThe value of structure variable a is: %d", ptr->a);
	printf("\nThe value of structure variable b is: %f", ptr->b);
	return 0;
}
