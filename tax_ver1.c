//Program to calculate the income tax.
//
#include<stdio.h>
#include<stdlib.h>
#define FIRST_SLAB 250000
#define SECOND_SLAB 500000
#define THIRD_SLAB 750000
#define FOURTH_SLAB 1000000
#define FIFTH_SLAB 1250000
#define SIXTH_SLAB 1500000



float tax =0;
int main()
{
	float income;
	printf("Enter a income: ");
	scanf("%f",&income);

	if(income<=0){
		perror("\nSalary cannot be less than zero");
		exit(0);
	}//if ends
	else if(income < FIRST_SLAB){	
		printf("\nNo tax");

	}// if else ends
	else if(FIRST_SLAB<income<=SECOND_SLAB ){
	
		tax = (income-FIRST_SLAB)*0.05;
	}//else if ends
	else if( SECOND_SLAB<income<=THIRD_SLAB){
		tax = (FIRST_SLAB*0.05)+(SECOND_SLAB*0.20)+(income - SECOND_SLAB);
	}//else if ends
	else if (THIRD_SLAB<income<=FOURTH_SLAB){
		tax = (FIRST_SLAB*0.05)+(SECOND_SLAB*0.20)+(THIRD_SLAB*0.30)+(income - THIRD_SLAB);
	}// else if ends
	else if (FOURTH_SLAB<income<=FIFTH_SLAB){
		tax = (FIRST_SLAB*0.05)+(SECOND_SLAB*0.20)+(THIRD_SLAB*0.30)+(FOURTH_SLAB*0.30)+(income - FOURTH_SLAB);
	}// else if ends
	else if (FIFTH_SLAB<income<=SIXTH_SLAB){
		tax = (FIRST_SLAB*0.05)+(SECOND_SLAB*0.20)+(THIRD_SLAB*0.30)+(FOURTH_SLAB*0.30)+(FIFTH_SLAB*0.30)+(income - FIFTH_SLAB);
	}// else if ends

	printf("Tax is: %.3f", tax);
}// main ends
