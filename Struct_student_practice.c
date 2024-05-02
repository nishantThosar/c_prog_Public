/*THIS IS NOT MENU DRIVEN PROGRAM, THIS IS TESTING PROGRAM.*/
//Program to maintain student data in structure.
//In this program only the ID is used.
#include<stdio.h>
#include<stdlib.h>

void student_add(void);// to add the student
void display(void);//To display the details of student
int search(void);//to search the student.
void modify(void);//to search and modify the details of student.
void free_(void);//to free the details of student
void free_all(void);// to free all details of the student.

/*Student structure currently contains just ID currently.*/
struct student
{
    int id;// student structure member.
}stu, *stu_p;//stu = variable , *stu_p is the structure pointer.

int student_no = 0; // global declaration of number of student.

int main()
{
    /*In main all the functions are been tested, no this is not a menu driven program*/
    student_add();
    int sea = search();
    printf("Id found at: %d", sea);
    modify();
    free_();
    free_all();
}//main ends

/*To add the student in the array.
In this function we are making a malloc (calloc is use but interprit it as malloc)*/
void student_add()
{
    
    int no_of_stu = 0;
    printf("\nEnter no of student: ");
    scanf("%d", &no_of_stu);
    student_no = no_of_stu;
    stu_p = (struct student*) calloc(no_of_stu, sizeof(stu));
    //null check
    if (!stu_p){
        perror("\nerror in malloc");
        exit(0);
    }//if ends

    for(int i=0; i<no_of_stu; i++){
        printf("\nEnter the ID for student%d: ", i);
        scanf("%d", &stu_p[i].id);

    }//for ends
    display();
}//student add ends

void display()
{
    for (int i = 0; i <student_no; i++)
    {
        printf("id: %d", stu_p[i].id);
    }
    
}//display ends

int search()
{
    int i=0;
    int target;
    printf("\n\nEnter ID to find");
    scanf("%d", &target);
    for(;i<student_no; i++)
    {
        if(target == stu_p[i].id){
            break;
        }
    }//for ends

    printf("\nID found at %d position", i+1);
    return i;
}//search ends


void modify()
{
    int target = 0;
    int index = search();

    printf("\n\nCurrent ID: %d", stu_p[index].id);
    printf("\nEnter new ID: ");
    scanf("%d", &stu_p[index].id);
    display();
}//modify ends

void free_()
{
    int index = 0;
    index = search();
    stu_p[index].id = 0;
    display();
}//free ends

void free_all()
{
    for(int i=0;i<student_no;i++){
        stu_p[i].id = 0;
    }
    display();
    free(stu_p);
}
