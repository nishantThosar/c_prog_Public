//lINKED LIST OPERATIONS

#include<stdio.h>
#include<stdlib.h>
struct link_list {//declration of strcut .
	int data;
	struct link_list *next; //declation of struct which poniting to next element 
};// struct end.

void add_beg(void);
void display(void);
void first_node(void);
struct link_list* make_node(void);

struct link_list *head, *t1; 

int total_node = 0;

int main()
{
	int choice;
	while(1)
	{
		printf("Enter Numerical value: ");
		printf("\n1: add node at begning");
		printf("\n2: add node at begning");
		printf("\n3: display all node");
		printf("\n0: exit\t");
		scanf("%d",&choice);
	
		//Switch case for menu driven program	
		switch(choice){
			
			case 0:	printf("\nThankyou\n");
				exit (0);
				break;
	
			case 1:first_node();
				break;
			
			case 2:add_beg();
				break;
			
			case 3: display();
				break;	
				
		}
	}
	return 0;
}

//adding first node.
void first_node()
{

	if (head==NULL)//to check weither the link list is preasnt  or not.
	{
		printf("\nthere is no link list");
	
		struct link_list *temp;
		temp =(struct link_list *)malloc(sizeof( struct link_list));
		//creating first node.
		printf("Enter the data in first element: ");
		scanf("%d",&temp->data);
		temp->next=NULL;
		head=temp;
		++total_node;// to keep check of no of nodes in the list.
	}
}



//adding node at begining.
void add_beg(){//function calling

	struct link_list *temp;
        
	if(head==NULL){
		printf("There is no link list");
		printf("\nAdd first node to the list.");
		temp = (struct link_list*)malloc(sizeof(struct link_list));
		head = temp;
		printf("\nEnter the data of first node: ");
		scanf("%d", &temp->data);
		temp->next = NULL;
	}
	else{
		temp=(struct link_list *)malloc(sizeof(struct link_list)); //making a new node.
				
		printf("\nEnter the element at begning");	
		scanf("%d",&temp->data);
		temp->next = head;
		head = temp;
		++total_node; 
	}
	
}

//make a fuction to daislay

void display()
{	
	t1=head;//to bring the t1 to tahe starting node.

	while(t1 != NULL){
		printf("\nThe value of  node is %d\n",t1->data);
		t1 = t1->next;
	}
}

//function for making node
struct link_list*  make_node()
{
	struct link_list *temp;
	temp = (struct link_list*)malloc(sizeof(struct link_list));
	return temp;//returning the address of structure.
}















