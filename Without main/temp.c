/*Here we cannot use float data type*/
#include<stdio.h>
#include <stdlib.h>
#define SQUARE(x) x*x

void main(int **);

void prt(void)
{
	int a = 20;
	int *ptr = &a;
	
	printf("\nHello guptaji\n");
	printf("%d\n",2*SQUARE(3));
	*ptr = 6;
	printf("%d\n%d\n", a ,*ptr);
	main(&ptr);
	printf("\nAfter main: %d\n", *ptr);
	exit(0);
	
}
void main(int **pt)
{
	printf("\nin main");
	**pt = 200;
}
