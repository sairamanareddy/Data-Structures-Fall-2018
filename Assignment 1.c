#include <stdio.h>
#include<stdbool.h>//used only for boolean flags.
#include<stdlib.h>
struct Node{
    struct Node* left;
    int data;
    struct Node* right;
};//declaring a doubly linked list.
void freelist(struct Node** head);//declaring a function called freelist that frees a doubly linked list.
void printlist(struct Node* head){//function for printing a list without leading zeros.
	struct Node* node=head;bool flag=true;
	while(node!=NULL){
		if(node->data!=0) flag=false;
		if (!flag) printf("%d", node->data);
		node=node->right;
	}
	printf("\n");
}
void insertfirst(struct Node** head, int data){
	//This function inserts data at the start of a list in constant time.
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));//creating a new raw node...
	node->right=*head;node->left=NULL;node->data=data;// getting new node ready...
	*head=node;//making node the new head...
}
void insertlast(struct Node** head, struct Node** tail, int data){
    // This function inserts data at the end of a list in constant time.
    if (*head==NULL){//if calling this function for the first time?
        *head=(struct Node*)malloc(sizeof(struct Node));
        (*head)->right=NULL;(*head)->left=NULL;(*head)->data=data;//making new head ready.
		*tail=*head;
        return;
    }
    (*tail)->right=(struct Node*)malloc(sizeof(struct Node));//node->right is now a new node, the new tail basically.
    (*tail)->right->right=NULL;(*tail)->right->data=data;(*tail)->right->left=*tail;//making the new tail ready...
	*tail=(*tail)->right;
}
void add(struct Node* node1, struct Node* node2){
	//This function takes the two linked lists, adds them and prints their sum. Assumes that head1 and head2 are not NULL
	struct Node* tail1=node1;
	struct Node* tail2=node2;
	int data=0,carry=0;
	struct Node* head3 = NULL;	//head3 points to the list of result.
	while(tail1!=NULL&&tail2!=NULL){
		data=(tail1->data)+(tail2->data)+carry;//add the digits
		carry=data/10;data=data%10;//compute the carry and the actual value of digit
		//printf("%d %d", data, carry);
		insertfirst(&head3, data);//insert the digit into the third list at the first.
		tail1=tail1->left;tail2=tail2->left;//propagate both lists backwards towards the head.
		//printf("%d %d %d", data, carry, head3->data);
	}
	while(tail1!=NULL){	//this loop executes if and only if second list is shorter.
		data=(tail1->data)+carry;//add the previous carry if any and present digit.
		carry=data/10;data=data%10;//compute the carry and the actual value of digit
		insertfirst(&head3, data);//insert the digit into the third list list at the first.
		tail1=tail1->left;//propogate this list backwards.
	}
	while(tail2!=NULL){
		data=(tail2->data)+carry;//add the previous carry if any and present digit.
		carry=data/10;data=data%10;//compute the carry and the actual value of digit
		insertfirst(&head3, data);//insert the digit into the third list list at the first.
		tail2=tail2->left;//propogate this list backwards.
	}
	insertfirst(&head3, carry);//if any carry remains, it gets inserted.
	printlist(head3);
	freelist(&head3);

}
void freelist(struct Node** head){//function to free a given linked list.
	struct Node* temp=*head;//assign a temp node to head.
	while(temp!=NULL){//while temp reaches NULL i.e. end of list
		struct Node* next=temp->right;//preserve next node as it's going to be unrecoverable.
		free(temp);// free current node.
		temp=next;//propagate rightwards.
	}
	*head=NULL;
}


int main(){
  int digit;bool flag=true;
  struct Node* head1=NULL;struct Node* tail1=NULL;
  struct Node* head2=NULL;struct Node* tail2=NULL;
  while((digit=fgetc(stdin))!=EOF){
    if(digit=='\n'){
      //Your code here.
	  add(tail1, tail2);
	  freelist(&head1);freelist(&head2);flag=true;
    }
    else if(digit==' '){
      //Your code here.
      flag=false;
    }
    else{
      //Your code here.
      if(flag){
          insertlast(&head1, &tail1, digit-48);//48 because each digit is actually read in ascii encoding with fgetc.
      }
      else{
          insertlast(&head2, &tail2, digit-48);
      }      
    }
  }
  return(0);
}