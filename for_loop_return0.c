/*This program shows what will heppen if we write return in for loop.*/

#include <stdio.h>

int main(int argc, char const *argv[])
{
	for (int i = 0; i < 5; ++i)
		return 0;
}//main ends

/*This program executes sucessfully because the C99 complier allowes it the C80 wouldn't
	Nothing happens when we run the program.
*/