/*Prog for finding the mod of program*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


	j = 10;
void const temp(void);
int main(int argc, char const *argv[])
{
	// printf("%d\n",atoi(argv[1])%5 ); // if argv <5 then the answer will be that number itself.
	j = 30; //this will give error as no data type is defined if j=10 is not written as globl.
	printf("Value of j: %d\n",j);
	int export = 20;
	printf("%d\n", export);
	printf("%s", "\n4 2");
	
	float a = 0.1; //here the complier takes it like double we need to implictly declare the value as float.
  float b = 0.2f; //this is implicetly declaration of a value as float.
	if (a == 0.1)
	{
		printf("%f\n",a);
	}
	else
		printf("Not equal\n");

	temp();
	return 0;
}

void const temp()
{
	printf("In the const function.\n");
}
