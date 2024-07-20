#include <stdio.h>
int main(int argc, char const *argv[])
{
	char *p;
	p = "Hello";
	printf("%c\n",*&*p); // this is how you can use the & symbol to derefer.
	return 0;
}

/*The output of this program is "H"*/
