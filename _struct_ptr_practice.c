/*Name: Data entry for student.
 *Author: Nishant Thosar.
 *Created: 07 April 2024.
 *Discription: This program accepts data for students and then asks about the modification if there is no modification then saves the data in a file.
 */

#include<stdio.h>
#include<stdlib.h>
#define filename "student_info.txt"// This file will be made.
//Defining the structure for student.
struct student
{
	int roll;
	int prn;
	char name[50];
}stu1,*stu2;// declaring a variable and a pointer to structure.
int i=0;// declaration of global variable to keep track of student entring.

int no_of_student;// global declaration of no of students varable.
void modify(int);
void input();
void display(int);

int main()
{	
	FILE *fp;// declaring file pointer.
	fp = fopen(filename, "w");// opening file in write mode.
	printf("Enter number of student you want to enter: ");
	scanf("%d", &no_of_student);// user input for how many number of student he want to input.
	
	stu2 = (struct student*) malloc (sizeof(stu1)*no_of_student);// made space for number of students in heap.
	input();// called input functon.
	// for loop to enter data of students in file.
	for(int j =0; j<no_of_student; j++){
	fprintf(fp, "\nName of student %d:....%s |",j+1,stu2[j].name);
	fprintf(fp, "\nRoll no of student %d:.%d |",j+1, stu2[j].roll);
	fprintf(fp, "\nPRN of student%d:......%d |",j+1, stu2[j].prn);
	}
	fclose(fp);// closing the file.
	printf("\nCheck the file with name \"student_info.txt\" in your current working drictory");
	free(stu2);// freed malloc ptr to avoid any memory leak.
	
	return 0;
}// main ends.


void input(int i)
{
	for(;i<no_of_student;){// for accepting entries till user input.
	printf("\nEnter name of student %d : ", i+1);
	scanf(" %[^\n]s", stu2[i].name);
	printf("\nEnter roll no of studnt %d : ",i+1);
	scanf("%d", &stu2[i].roll);
	printf("\nEnter prn no of studnt %d : ",i+1);
	scanf("%d", &stu2[i].prn);
	display(i);
	modify(i);
	i++;
	}// for loop ends
}// input function ends

void display(int i)
{
	printf("\nName: %s", stu2[i].name);
	printf("\nRoll no: %d", stu2[i].roll);
	printf("\nPRN: %d", stu2[i].prn);
	
}// display function ends.
void modify (int i)
{
	int choise ;
	printf("\nDo you want to modify the the input values? \nEnter numeric (0:NO 1:Yes)");
	scanf("%d",&choise);
	int choose ;
	if( choise == 1){
		printf("\nEnter what you want to modify \n 1. Name \n 2. Roll no \n 3. PRN ");
		scanf("%d", &choose);
		switch(choose){
			case 1: printf("\nThe old name is: %s", stu2[i].name);// to display old name.
				printf("\nEnter the new name: ");
				scanf(" %[^\n]s", stu2[i].name);// to capture user input for updation
				printf("\nUpdated");
				display(i);// display all details with updation.
				modify(i);// to ask if user wants to update anything more for same student.
				break;
			
			case 2: printf("\nThe old Roll no is: %d", stu2[i].roll);// to display old name.
				printf("\nEnter the new Roll no ");
				scanf(" %d", &stu2[i].roll);// to capture user input for updation
				printf("\nUpdated");
				display(i);// display all details with updation.
				modify(i);// to ask if user wants to update anything more for same student.
				break;
		
			case 3: printf("\nThe old PRN is: %d", stu2[i].prn);// to display old name.
				printf("\nEnter the new PRN: ");
				scanf(" %d",&stu2[i].prn);// to capture user input for updation
				printf("\nUpdated");
				display(i);// display all details with updation.
				modify(i);// to ask if user wants to update anything more for same student.
				break;
		}//Switch ends	
	}// if condition ends.
}//modify function ends.
