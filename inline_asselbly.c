/*Program for inline asselbly*/

#include <stdio.h>

int main(int argc, char const *argv[])
{
	int var_name = 0;
	asm __volatile__("mov $1, %0"	: "=r" (var_name)); // this line is for inline assembly.
	printf("%d\n",var_name);
	return 0;
}
