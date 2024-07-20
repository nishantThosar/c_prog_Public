/*This program demonstrate the block scope of the variable*/

#include <stdio.h>

int main(int argc, char const *argv[])
{
	int k = 20, j = 10;

	{
		int k = 90; // this is local to block and will be a different variable in total.
		printf("inside block k: %d\n",k);
		j = 50; // this is just like updating the variable
		printf("inside block j: %d\n",j);
	}
	printf("Out of block k: %d\n", k);//you will see change here.
	printf("Out of block j: %d\n", j);
	return 0;
}