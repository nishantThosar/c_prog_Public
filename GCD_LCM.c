//
//
#include<stdio.h>
int main()
{
	int a,b, gcd;
	printf("Enter 2 values: ");
	scanf("%d %d", &a,&b);
	
	while (b){
		
		int temp = a%b;
		a=b;
		b=temp;

	}// while ends
	 printf("GCD is : %d", a);


	return 0;
}// main ends
