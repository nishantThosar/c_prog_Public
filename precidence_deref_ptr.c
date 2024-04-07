//File made for understanding precedence of dereferencing of ptr.
#include<stdio.h>
void ptr1(int *);
void ptr2(int *);
int main()
{
	int x = 10;
	//int *ptr = &x;
	printf("ptr(before any operation) = \"%p\"\n", &x);
//	ptr1(&x);
	ptr2(&x);


	return 0;
}
/*
void ptr1(int *ptr)//First derefrence will be done then value will be assigned to x then the ptr will be incrimented by 4.
{
	
	printf("x = %d\n", *ptr);
	printf("*ptr++ = %d\n", *ptr++);// first x=*ptr then ptr=ptr+1.
	printf("ptr = %p\n", ptr);
}*/
/*
void ptr2(int *ptr)// first ptr will be incrimented by 4 then the value will be derefrenced.
{ 	
	printf("x = %d\n", *ptr);
	printf("*++ptr = %d\n", *++ptr);// first ptr=ptr+1 then x=*ptr.
	printf("ptr(after *++ptr) = %p\n", ptr);
	printf("x(after *++ptr) = %d\n", *ptr);
}*/
