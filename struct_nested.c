//Nested structure program.
#include <stdio.h>
struct student{
	char name[20];
	int roll_no;
	struct date{ //Nested structure.
		int day;
		int month;
		int year;
	}birth_date;// nested structure variable name.
	float marks;
}stu1,stu2;// main structure variable name.

int main()
{
	stu1.birth_date.day = 12;// This is how we can access the member inside a structure.
	printf("Birth date is %d", stu1.birth_date.day);//Printing the member of nested structure.
	return 0;
}
