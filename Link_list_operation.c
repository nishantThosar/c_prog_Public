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
void del_at_end(void);
void free_ptr(void);

struct link_list* make_node(void);

struct link_list *head, *t1; 

int total_node = 0;

int main()
{
	int choice;
	while(1)
	{
		printf("Enter Numerical value: ");
		printf("\n1: Add first node");
		printf("\n2: Add node at begning");
		printf("\n3: Display all node");
		printf("\n4: Delete at end");
		printf("\n5: To delete whole list");
		printf("\n0: Exit\t");
		printf("\n");
		printf("\nEnter choice:\t");
		
		scanf("%d",&choice);
	
		//Switch case for menu driven program	
		switch(choice){
			
			case 0:	printf("\nThankyou\n");
				printf("\nCurrent Nodes: %d", total_node);
				exit (0);
				break;
	
			case 1:first_node();
				printf("\nCurrent Nodes: %d", total_node);
				break;
			
			case 2:add_beg();
				printf("\nCurrent Nodes: %d", total_node);
				break;
			
			case 3: display();
				printf("\nCurrent Nodes: %d", total_node);
				break;	
			
			case 4:del_at_end();
				printf("\nCurrent Nodes: %d", total_node);
			       break;

			case 5: free_ptr();
				printf("\nDeleted all list");
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
		struct link_list *temp;
		temp =(struct link_list *)malloc(sizeof( struct link_list));
		//creating first node.
		printf("\nEnter the data in first element: ");
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
		printf("\nThere is no link list");
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


//fuction for delete node form end
void del_at_end()
{

	struct link_list *t2;
	t2=head;
	t1=head;
	while(t1->next!=NULL)
	{
		t2=t1;
		t1 = t1->next;
	}
	free(t1);
	t2 -> next = NULL;
	--total_node;
}//delete at end function ends.


//To avoid any memory leak.
void free_ptr()
{
	
	struct link_list *t2;
	struct link_list *t1;
	t1 = t2 = head;
	int count =0;

	while(t1->next != NULL){
		++count;
		t2 = t1;
		t1 = t1->next;
	}//while ends
	t2->next = NULL;
	free(t1);
//	free(head);
//	free(ptr);

	printf("Total nodes freed: %d", count);
}//Free ptr ends
