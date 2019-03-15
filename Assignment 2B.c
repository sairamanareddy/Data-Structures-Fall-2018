// CS17BTECH11022 Assignment 2B
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

struct Node{
    struct Node* left;
    int data;
    struct Node* right;
};
struct BSTNode{
    struct BSTNode* parent;
    struct Node* head;
    struct Node* tail;
    struct BSTNode* leftchild;
    struct BSTNode* rightchild;
};

int compare(struct Node* head1, struct Node* tail1, struct Node* head2, struct Node* tail2){
	//This procedure compares the numbers one and two, and prints 1 if num1>num2 and 0 otherwise.
	struct Node* temp1=head1;//we'll use temp1 to traverse the first list.
	struct Node* temp2=head2;//we'll use temp2 to traverse the second list.
	int flag=-1;
	temp1=tail1;temp2=tail2;
	while(temp1!=NULL && temp2!=NULL){
		if(temp1->data>temp2->data){
			flag=1;
		}
		if(temp1->data<temp2->data){
			flag=0;
		}
		temp1=temp1->left;
		temp2=temp2->left;
	}
	if(temp1==NULL && temp2!=NULL){
		return 0;
	}
	else if(temp1!=NULL && temp2==NULL){
		return 1;
	}
	else{
		return flag;
	}

	
}

void printlist(struct Node* head){//function for printing a given list.
	struct Node* node=head;
	while(node!=NULL){
		printf("%d", node->data);
		node=node->right;
	}
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

void freelist(struct Node** head){//function to free a given linked list.
	struct Node* temp=*head;//assign a temp node to head.
	while(temp!=NULL){//while temp reaches NULL i.e. end of list
		struct Node* next=temp->right;//preserve next node as it's going to be unrecoverable.
		free(temp);// free current node.
		temp=next;//propagate rightwards.
	}
	*head=NULL;
}


void InsertBSTNode(struct BSTNode** root, struct Node* head, struct Node* tail){
    //iterative function to insert a Tree node at the appropriate place.
    if(*root==NULL){
        *root=(struct BSTNode*)malloc(sizeof(struct BSTNode));
        (*root)->parent=NULL;(*root)->leftchild=NULL;(*root)->rightchild=NULL;
        (*root)->head=head;(*root)->tail=tail;
        return;
    }
    struct BSTNode* temp1=NULL;   //define a traversal BSTNode.
    struct BSTNode* temp=*root;
    int insert=0;
    while(temp!=NULL){  //this loop finds finds appropriate place to insert the new node.
        temp1=temp;
        insert=compare(head, tail, temp->head, temp->tail);
        if(insert==1) temp=temp->rightchild;
        else if(insert==0) temp=temp->leftchild;
        else{ freelist(&head);return;}
    }
    temp=(struct BSTNode*)malloc(sizeof(struct BSTNode));
    temp->parent=temp1;temp->leftchild=NULL;temp->rightchild=NULL;
    temp->head=head;temp->tail=tail;
    insert=compare(head, tail, temp1->head, temp1->tail);
    if(insert==0) temp1->leftchild=temp;
    if(insert==1) temp1->rightchild=temp;


}



void FreeBST(struct BSTNode** root){
    //Recursive function to free a given BST.
    struct BSTNode* temp=*root;
    if(temp!=NULL){
        struct BSTNode* templ=temp->leftchild;
        struct BSTNode* tempr=temp->rightchild;
        freelist(&(temp->head));
        FreeBST(&templ);
        FreeBST(&tempr);
        free(temp);
    }
    *root=NULL;
}

void BSTSearch(struct BSTNode* root, struct Node* head, struct Node* tail){
    //Function for iterative BSTSearch with an arbitrary large query.
    struct Node* pathhead=NULL,*pathtail=NULL;
    struct BSTNode* temp=root;
    int c=0;
    while(temp!=NULL){
        c=compare(temp->head, temp->tail, head, tail);  //c==0 if query is greater, 1 if query is less, -1 if equal.
        if(c==0){   //traverse right;
            temp=temp->rightchild;
            insertlast(&pathhead, &pathtail, 1);
            continue;
        }
        if(c==1){   //traverse left;
            temp=temp->leftchild;
            insertlast(&pathhead, &pathtail, 0);
            continue;
        }
        if(c==-1){
            printlist(pathhead);
            printf("\n");
            freelist(&pathhead);
            return;
        }
    }
    printf("-1\n");
    freelist(&pathhead);
    
    
}

void PreOrder(struct BSTNode* root){
    //Recursive function for PreOrder traversal.
    struct BSTNode* temp=root;
    if(temp!=NULL){
        printlist(temp->head);printf(" ");
        PreOrder(temp->leftchild);
        PreOrder(temp->rightchild);
    }
}

int main(){
    int c=0, digit=0;
    struct BSTNode* root=NULL;struct Node* head=NULL, *tail=NULL;
    bool leadingflag=true;
    while((c=fgetc(stdin))!=EOF){   ///constantly reads input from STDIN.
        if(c=='N'){ //if the user wants to insert a new set into BST...

            //  Free previous BST first here.
            FreeBST(&root);
            leadingflag=true;
            c=fgetc(stdin);   //skip a space. Now numbers follow seperated by a space till a \n character.
            if(c=='\n') continue;
            while((digit=fgetc(stdin))!='\n'){  //while digit is not a '\n' we must insert the stream till a space.
                if(digit==' '){ // to skip a space and go to the next node.
                    //Insert the list into a BSTNode first here.
                    InsertBSTNode(&root, head, tail);
                    //Set the head and tail pointers to NULL for recording next number.
                    head=NULL;tail=NULL;
                    leadingflag=true;
                    continue;   
                }
                if(digit!=48) leadingflag=false;
                if(digit==48 && leadingflag) continue;
                if(!leadingflag){
                    insertlast(&head, &tail, digit-48);
                }    
            }
            InsertBSTNode(&root, head, tail);   //this is to insert the last number in the input into the BST, which is not inserted by the above loop.
        }


        leadingflag=true;   //this flag is for the query which might need to be sanitized.
        head=NULL;tail=NULL;    //these head and tail variables shall be used for the query number.


        if(c=='S'){ //if the user wants to query an integer in the BST...
            fgetc(stdin); //skip a space. what follows is the query.
            while((digit=fgetc(stdin))!='\n'){  //we're in the query stream.
                if(digit!=48) leadingflag=false;
                if(digit==48 && leadingflag) continue;
                if(!leadingflag) insertlast(&head, &tail, digit-48);

            }
            BSTSearch(root, head, tail);
            freelist(&head);    //frees the search query.
            tail=NULL;
        }


        if(c=='P'){ //if the user wants to print the PreOrder traversal of the BST...
            fgetc(stdin);//to exhaust the \n after P....
            PreOrder(root);
            printf("\n");
        }
    }
    // after EOF is marked, the recently used tree must be freed.
    FreeBST(&root);


    return 0;
}