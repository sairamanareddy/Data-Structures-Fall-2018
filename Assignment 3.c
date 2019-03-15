// CS17BTECH11022 Assignment 3
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
    if(head==NULL) return;  //if empty input is given.
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
        temp1=temp; //just to store the previous value of temp as it'd get modified. later.
        insert=compare(head, tail, temp->head, temp->tail);
        if(insert==1) temp=temp->rightchild;
        else if(insert==0) temp=temp->leftchild;
        else{ freelist(&head);return;}  //this would mean that the tree already has the node.
    }
    temp=(struct BSTNode*)malloc(sizeof(struct BSTNode));   //allocate memory for the new node.
    temp->parent=temp1;temp->leftchild=NULL;temp->rightchild=NULL;  //intializing the node.
    temp->head=head;temp->tail=tail;//links...
    if(insert==0) temp1->leftchild=temp;    //more links
    if(insert==1) temp1->rightchild=temp;


}


struct BSTNode* Successor(struct BSTNode* root, struct Node* head, struct Node* tail){
    //this function finds the successor of given element and prints the appropriate result.
    //returns the successor of an element if exists. Else returns NULL.
    if(root==NULL){printf("-1");return NULL;}   //just a failsafe.
    struct BSTNode* temp1=NULL;   //define a traversal BSTNode.
    struct BSTNode* temp=root;
    int insert=0;
    while(temp!=NULL){  //this loop finds finds appropriate place to insert the new node.
        temp1=temp; //just to store the previous value of temp as it'd get modified.
        insert=compare(head, tail, temp->head, temp->tail);
        if(insert==1) temp=temp->rightchild;    //given value is more than 
        else if(insert==0) temp=temp->leftchild;
        else{break;}  //given value is already there in the Tree.
    }
    //think of the given node as a virtual not yet present node in the Tree.
    //temp1 is now the parent of the virtual node temp.
    if(insert==1){      //virtual node is the right child of temp1.
        //successor of virtual node is same as actual successor of temp1.
        temp=temp1->parent;
        while(temp!=NULL && temp1==temp->rightchild){
            temp1=temp;
            temp=temp->parent;
        }
        if(temp==NULL) printf("-1");
        return temp;
    }
    else if(insert==0) return temp1;  //virtual node is the left child of temp1.
    else{                              //virtual node is already present at temp in the tree.
        if(temp->rightchild!=NULL){//if there's a rightchild...
            temp1=temp->rightchild;     //go to the right child...
            while(temp1->leftchild!=NULL){
                temp1=temp1->leftchild;     //and keep on traversing to the leftchild...
            }
            return temp1;
        }
        temp1=temp->parent; //if there's no right child...
        while(temp1!=NULL && temp==temp1->rightchild){  //while we are still the right child of our parent...
            temp=temp1;
            temp1=temp1->parent;    //keep on traversing towards our ancestors...
        }
        if(temp1==NULL) printf("-1");   //there's no left subtree that we're in. So there's no successor.
        return temp1;
    }
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

struct BSTNode* BSTSearch(struct BSTNode* root, struct Node* head, struct Node* tail, bool printflag){
    //Function for iterative BSTSearch with an arbitrary large query.
    //prints the path if printflag is set, and returns the node if found. else returns NULL.
    struct Node* pathhead=NULL,*pathtail=NULL;
    struct BSTNode* temp=root;
    int c=0;
    while(temp!=NULL){
        c=compare(temp->head, temp->tail, head, tail);  //c==0 if query is greater, 1 if query is less, -1 if equal.
        if(c==0){   //traverse right;
            temp=temp->rightchild;
            if(printflag) insertlast(&pathhead, &pathtail, 1);
            continue;
        }
        if(c==1){   //traverse left;
            temp=temp->leftchild;
            if(printflag) insertlast(&pathhead, &pathtail, 0);
            continue;
        }
        if(c==-1){
            if(printflag){
                printlist(pathhead);
                printf("\n");
            }
            freelist(&pathhead);
            return temp;
        }
    }
    if(printflag) printf("-1\n");
    freelist(&pathhead);
    return NULL;    
}

void BSTSplice(struct BSTNode** root, struct BSTNode* node1, struct BSTNode* node2){
    //function to splice a node1 and replace it with node2
    if(node1->parent==NULL) *root=node2;
    else if(node1==node1->parent->leftchild) node1->parent->leftchild=node2;
    else node1->parent->rightchild=node2;
    if(node2!=NULL) node2->parent=node1->parent;
}

void DeleteBSTNode(struct BSTNode** root, struct BSTNode* node, bool succflag){
    //Deletes a BST Node from the tree and frees the memory too.
    //takes only the value to be deleted as argument.
    if(node==NULL) return;
    if(node->leftchild==NULL && node->rightchild==NULL){
        if(node->parent==NULL){freelist(&(node->head));free(node);node=NULL;*root=NULL;return;}
        if(node->parent->leftchild==node) node->parent->leftchild=NULL; //node is the left child of it's parent.
        else node->parent->rightchild=NULL; //node is the right child of it's parent.
        if(!succflag) freelist(&(node->head));
        free(node);node=NULL;
        return;
    }
    else if(node->leftchild==NULL) BSTSplice(root, node, node->rightchild);
    else if(node->rightchild==NULL) BSTSplice(root, node, node->leftchild);
    else{
        struct BSTNode* succ=Successor(*root, node->head, node->tail);  //replacing with the successor.
        freelist(&(node->head));    //Free the present value in the node.
        node->head=succ->head;      //Inherit the value of the successor.
        node->tail=succ->tail;
        DeleteBSTNode(root, succ, true);    //Delete the successor with the succ flag.
        return;
    }
    if(!succflag) freelist(&(node->head));  //Value of successor shouldn't be freed, for it's in use.
    free(node);node=NULL;   //free the present node in case of utmost 1.
    return;
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
            leadingflag=true;head=NULL;tail=NULL;
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
            continue;
        }
        if(c=='+'){ //if the user wants to insert an element into the same set...
            head=NULL;tail=NULL;
            leadingflag=true;
            fgetc(stdin);   //skip a space. what follows is the new element.
            while((digit=fgetc(stdin))!='\n'){
                if(digit!=48) leadingflag=false;
                if(digit==48 && leadingflag) continue;
                if(!leadingflag) insertlast(&head, &tail, digit-48);    //Insert the element into the linked list.
            }
            InsertBSTNode(&root, head, tail);   //after completing reading, insert it into BST.
            head=NULL;  //reintialize head, tail for using again.
            tail=NULL;  //Note that we must not free the list here.
            continue;
        }
        if(c=='>'){//if user wants to find the successor of an element...
            head=NULL;tail=NULL;
            leadingflag=true;
            fgetc(stdin);   //skip a space. what follows is the query.
            while((digit=fgetc(stdin))!='\n'){
                if(digit!=48) leadingflag=false;
                if(digit==48 && leadingflag) continue;
                if(!leadingflag) insertlast(&head, &tail, digit-48);    //Insert the element into the linked list.
            }
            struct BSTNode* succ=Successor(root, head, tail);
            if(succ!=NULL) printlist(succ->head);   //if there is no successor, don't try to print it.
            printf("\n");
            freelist(&head);
            tail=NULL;
            continue;          
        }
        if(c=='-'){
            head=NULL;tail=NULL;
            leadingflag=true;
            fgetc(stdin);   //skip a space. what follows is the query.
            while((digit=fgetc(stdin))!='\n'){
                if(digit!=48) leadingflag=false;
                if(digit==48 && leadingflag) continue;
                if(!leadingflag) insertlast(&head, &tail, digit-48);    //Insert the element into the linked list.
            }
            DeleteBSTNode(&root, BSTSearch(root, head, tail, false), false);
            freelist(&head);tail=NULL;
            continue;
        }
        if(c=='S'){ //if the user wants to query an integer in the BST...
            fgetc(stdin); //skip a space. what follows is the query.
            leadingflag=true; head=NULL;tail=NULL;
            while((digit=fgetc(stdin))!='\n'){  //we're in the query stream.
                if(digit!=48) leadingflag=false;
                if(digit==48 && leadingflag) continue;
                if(!leadingflag) insertlast(&head, &tail, digit-48);

            }
            BSTSearch(root, head, tail, true);
            freelist(&head);    //frees the search query.
            tail=NULL;
            continue;
        }
        if(c=='P'){ //if the user wants to print the PreOrder traversal of the BST...
            fgetc(stdin);//to exhaust the \n after P....
            PreOrder(root);
            printf("\n");
            continue;
        }
    }
    // after EOF is marked, the recently used tree must be freed.
    FreeBST(&root);


    return 0;
}