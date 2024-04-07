//Menu driven program.
// By Nishant Thosar.
// There is an issue in the search in this code.
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
struct student
{
	char name[50];
	char gender;
	int roll;
	int prn;
}stu1,*stu2;

int i=0;
int no_of_student;
int choise;
int search_val;

void input(int);
void display(int);
void modify(int);
void search(int i);

int main()
{
	do{
	
		printf("\n============================= Welcome to manager ==============================\n");
		printf("Enter the choise: ");
		printf("\n1. To input data");
		printf("\n2. To find the data");
		printf("\n3. To modify the data");
		printf("\n0. To exit the program");
		printf("\nEnter the choise: ");
		scanf("%d",&choise);
		switch (choise){
				// case 1 to enter the student data.
				case 1: printf("\nEnter how much student you want to enter: ");
					scanf("%d", &no_of_student);
					stu2 = (struct student*)malloc(sizeof(stu1)*no_of_student);
					input(i);
					break;
		
				//case 2 to find the student.
				case 2: printf("\nEnter how you want to find the data of student");
					printf("\n Enter numerical value: 1=By name, 2=By Roll no, 3=By PRN, 0=Exit");
					scanf("%d", &search_val);
					search(search_val);
					if(choise != 0){
						main();
					}// if ends.
					else
						break;
				//case 3 to modify the data.
				case 3: printf("\nEnter how you want to find the data of student");
					printf("\n Enter numerical value: 1=By name, 2=By Roll no, 3=By PRN, 0=Exit");
					scanf("%d", &search_val);
					search(search_val);
					break;
				//default case to eit the program.
				default: printf("\nBye\n");
					 break;
		}//switch ends
	}while(choise !=0);//do while ends.
	free(stu2);// free the malloc pointer.
}// main end.

void input(int i)
{	printf("================================ Enter information ==========================================\n");
	for (;i<no_of_student;){
		printf("Enter name of student %d: ", i+1);
		scanf(" %[^\n]s", stu2[i].name);
		
		printf("Enter Roll no of student %d: ",i+1);
		scanf(" %d", &stu2[i].roll);
		
		printf("Enter PRN of student %d: ", i+1);
		scanf(" %d", &stu2[i].prn);
		display(i);
		modify(i);
		i++;
	}//For end
}//input ends

void display(int i)
{
	printf("Name of student %s: ", stu2[i].name);
	printf("Roll no of student %d: ", stu2[i].roll);
	printf("PRN of student %d: ", stu2[i].prn);

}//display ends
 
void modify(int i)
{
	printf("\n=================================== You can modify ====================================\n");
	int ans =0;
	printf("\nDo you want to modify the information? ");
	printf("0 = NO || 1 = YES");
	scanf("%d", &ans);
	if(ans == 1){
		int choice;
		printf("\nEnter what you want to moidfy");
		printf("\nEnter in numeric \"1=Name, 2=PRN, 3=Roll no\" ");
		scanf("%d", &choice);

		switch(choice){
			case 1: printf("OLD name : %s", stu2[i].name);
				printf("Enter updated name : ");
				scanf(" %[^\n]s", stu2[i].name);
				display(i);
				modify(i);
				break;
			case 2: printf("OLD PRN : %d", stu2[i].prn);
				printf("Enter updated PRN : ");
				scanf(" %d", &stu2[i].prn);
				display(i);
				modify(i);
				break;
			case 3: printf("OLD Roll no : %d", stu2[i].roll);
				printf("Enter updated Roll no : ");
				scanf(" %d",&stu2[i].roll);
				display(i);
				modify(i);
				break;
		}// Switch ends.
		
	}//if ends

}//Modify ends

void search(int search_val)
{
	printf("\n================================ You are now in search mode ========================================\n");
	printf("\n---You can modify the details also---");
	char name[50];
	int roll_no;
	int prn_no;
	switch (search_val){
		case 1: printf("Enter the name of student to search: ");
			scanf(" %[^\n]s", name);
			for(int j =0; j<i; j++){
				if(strcmp(name, stu2[j].name)){// compare string and print.
					display(j);
					modify(j);
				}// if ends.
			
			}// for ends.
			break;

		case 2: printf("Enter Roll no: ");
			scanf("%d", &roll_no);
			for(int j=0; j<i; j++){
				if(roll_no == stu2[j].roll){
					display(j);
					modify(j);
				}//if ends.
			}//for ends.
			break;

		case 3: printf("Enter PRN : ");
			scanf("%d",& prn_no);
			for(int j=0; j<i; j++){
				if(roll_no == stu2[j].prn){
					display(j);
					modify(j);
				}//if ends.
			}//for ends.
			break;		
	}//switch ends

}// Search ends

