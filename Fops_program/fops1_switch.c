/*Switch menu for the user defined file for the program of fops in c language*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fops1_switch.h"


#define OPTIONS 4 // add number to increase the user input options in the switch case.

enum u_ip{EXIT,OPEN,READ,WRITE};
enum u_ip choice; // enum var for user input

char *options[] = {"Welcome to Fops program written By Nishant\n","Exit=> 0","Open=> 1","Read=> 2","Write=> 3"};

void switch_case(void)
{
	
	int  usr, uip2 = 0;
	while(true)
	{
		for (int i = 0; i<5; ++i)
		{
			printf("%s\t", options[i]);
		}
		printf("\n");
		printf("\nEnter the desired input: ");
		scanf("%d", &usr); // to get user input
		if(usr < OPTIONS && usr >= 0)
		{
			choice = usr;
			switch(choice)
			{
				case EXIT:
					printf("Exiting the program\n");
					exit(0);
				case OPEN:
					printf("OPEN called\n");
					break;
				case READ:
					printf("READ called\n");
					break;
				case WRITE:
					printf("WRITE called\n");		
					printf("Write mode in append or you want to start writing from scratch?\n Press:\t1 To Append\t 2 To write from scratch");
					scanf("%d", &uip2);
					switch(uip2){
					case 1:
						printf("Entered in Append mode\n");
						break;
					case 2:
						printf("Entered to write from scratch.\n");
						break;
					default:
						printf("No case match!\n Exiting the write mode");
					}//switch for write case ends
					break;
				default:
					printf("Error in input \n");
			}//outer switch ends
		}//if ends
		else
			printf("Wrong input.. Exiting\n"); 
	}//while ends
}// switch_case function ends