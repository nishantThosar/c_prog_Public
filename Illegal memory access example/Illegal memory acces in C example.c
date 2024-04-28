/*Q4. Write a C/C++ program to implement circular queue data
 * structure using array. Implement below metioned functions.
 * a) add b) delete c) peep d) displayQueue e)isQueueFull
 * f) isQueueEmpty
 */

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define GOOD_GARBAGE -99

int main()
{
	int size;
	printf("\n\nEnter how much size of queue you want: ");
	scanf("%d", &size);
	int *my_calloc = (int*) calloc(size,sizeof(int));
	/*Here even if the calloc(size of array) is for less the for loop accesses the next addresses
	and assigns -99 to them till 5 place.
	This is how illegal memory is accesed in c language.*/
	for(int i=0; i<5; i++){
		my_calloc[i] = GOOD_GARBAGE;
	}// for ends.
	print_arr(my_calloc);
	free(my_calloc);

}// main ends
 
void print_arr(int *arr)
{
	for(int i=0;i<5;i++){
		printf("\t%d", arr[i]);
	}//for ends

}
