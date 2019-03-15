// CS17BTECH11022 Assignment 4
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
/*     struct Node* head;
    struct Node* tail; */
    struct BSTNode* leftchild;
    unsigned int data;
    struct BSTNode* rightchild;
};
typedef struct BSTNode BSTNode;

int compare(unsigned int data1, unsigned int data2){
	//This procedure compares the numbers one and two, and prints 1 if num1>num2 and 0 otherwise.
    if(data1>data2) return 1;
    else if(data1<data2) return 0;
    else return -1;
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


void InsertBSTNode(BSTNode** root, unsigned int data){
    //iterative function to insert a Tree node at the appropriate place.
    if(*root==NULL){
        *root=(BSTNode*)malloc(sizeof(BSTNode));
        (*root)->parent=NULL;(*root)->leftchild=NULL;(*root)->rightchild=NULL;
        (*root)->data=data;
        return;
    }
    BSTNode* temp1=NULL;   //define a traversal BSTNode.
    BSTNode* temp=*root;
    int insert=0;
    while(temp!=NULL){  //this loop finds finds appropriate place to insert the new node.
        temp1=temp; //just to store the previous value of temp as it'd get modified. later.
        insert=compare(data, temp->data);
        if(insert==1) temp=temp->rightchild;
        else if(insert==0) temp=temp->leftchild;
        else return;  //this would mean that the tree already has the node.
    }
    temp=(BSTNode*)malloc(sizeof(BSTNode));   //allocate memory for the new node.
    temp->parent=temp1;temp->leftchild=NULL;temp->rightchild=NULL;  //intializing the node.
    temp->data=data;
    if(insert==0) temp1->leftchild=temp;    //more links
    if(insert==1) temp1->rightchild=temp;


}


BSTNode* Successor(BSTNode* root, unsigned int data){
    //this function finds the successor of given element and prints the appropriate result.
    //returns the successor of an element if exists. Else returns NULL.
    if(root==NULL){printf("-1");return NULL;}   //just a failsafe.
    BSTNode* temp1=NULL;   //define a traversal BSTNode.
    BSTNode* temp=root;
    int insert=0;
    while(temp!=NULL){  //this loop finds finds appropriate place to insert the new node.
        temp1=temp; //just to store the previous value of temp as it'd get modified.
        insert=compare(data, temp->data);
        if(insert==1) temp=temp->rightchild;    //given value is more than 
        else if(insert==0) temp=temp->leftchild;
        else break;  //given value is already there in the Tree.
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



void FreeBST(BSTNode** root){
    //Recursive function to free a given BST.
    BSTNode* temp=*root;
    if(temp!=NULL){
        BSTNode* templ=temp->leftchild;
        BSTNode* tempr=temp->rightchild;
        FreeBST(&templ);
        FreeBST(&tempr);
        free(temp);
    }
    *root=NULL;
}

BSTNode* BSTSearch(BSTNode* root, unsigned int data, bool printflag){
    //Function for iterative BSTSearch with an arbitrary large query.
    //prints the path if printflag is set, and returns the node if found. else returns NULL.
    struct Node* pathhead=NULL,*pathtail=NULL;
    BSTNode* temp=root;
    int c=0;
    while(temp!=NULL){
        c=compare(temp->data, data);  //c==0 if query is greater, 1 if query is less, -1 if equal.
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

void BSTSplice(BSTNode** root, BSTNode* node1, BSTNode* node2){
    //function to splice a node1 and replace it with node2
    if(node1->parent==NULL) *root=node2;
    else if(node1==node1->parent->leftchild) node1->parent->leftchild=node2;
    else node1->parent->rightchild=node2;
    if(node2!=NULL) node2->parent=node1->parent;
}

void DeleteBSTNode(BSTNode** root, BSTNode* node){
    //Deletes a BST Node from the tree and frees the memory too.
    if(node==NULL) return;
    if(node->leftchild==NULL && node->rightchild==NULL){// no children...
        if(node->parent==NULL){free(node);*root=NULL;return;}
        if(node->parent->leftchild==node) node->parent->leftchild=NULL; //node is the left child of it's parent.
        else node->parent->rightchild=NULL; //node is the right child of it's parent.
        free(node);node=NULL;
        return;
    }
    else if(node->leftchild==NULL) BSTSplice(root, node, node->rightchild);//has a right child...
    else if(node->rightchild==NULL) BSTSplice(root, node, node->leftchild);//has a left child...
    else{
        BSTNode* succ=Successor(*root, node->data);  //replacing with the successor.
        node->data=succ->data;
        DeleteBSTNode(root, succ);
        return;
    }
    free(node);node=NULL;   //free the present node in case of utmost 1 child.
    return;
}

void PreOrder(BSTNode* root){
    //Recursive function for PreOrder traversal.
    BSTNode* temp=root;
    if(temp!=NULL){
        printf("%u", temp->data);printf(" ");
        PreOrder(temp->leftchild);
        PreOrder(temp->rightchild);
    }
}



void BSTLeftRot(BSTNode** root, BSTNode* node){
    //function to left rotate at node.
    //Assumes left rotation is really possible at node.
    BSTNode* node2=node->rightchild;
    BSTSplice(root, node, node2);
    node->rightchild=node2->leftchild;
    node2->leftchild=node;
    node->parent=node2;
    if(node->rightchild!=NULL) node->rightchild->parent=node;
}
void BSTRightRot(BSTNode** root, BSTNode* node){
    //function to right rotate at node.
    //Assumes right rotation is really possible at node.
    BSTNode* node2=node->leftchild;
    BSTSplice(root, node, node2);
    node->leftchild=node2->rightchild;
    node2->rightchild=node;
    node->parent=node2;
    if(node->leftchild!=NULL) node->leftchild->parent=node;

}

int main(){
    int c=0, digit=0;
    unsigned int data=0;
    BSTNode* root=NULL;
    while((c=fgetc(stdin))!=EOF){   ///constantly reads input from STDIN.
        if(c=='N'){ //if the user wants to insert a new set into BST...

            //  Free previous BST first here.
            FreeBST(&root);
            c=fgetc(stdin);   //skip a space. Now numbers follow seperated by a space till a \n character.
            if(c=='\n') continue;
            while((digit=fgetc(stdin))!='\n'){  //while digit is not a '\n' we must insert the stream till a space.
                if(digit==' '){ // to skip a space and go to the next node.
                    //Insert the list into a BSTNode first here.
                    InsertBSTNode(&root, data);
                    continue;
                }
                ungetc(digit, stdin);   //push back into stream since it's a digit.
                scanf("%u", &data);   
            }
            InsertBSTNode(&root, data);   //this is to insert the last number in the input into the BST, which is not inserted by the above loop.
            continue;
        }
        if(c=='+'){ //if the user wants to insert an element into the same set...
            fgetc(stdin);   //skip a space. what follows is the new element.
            scanf("%u", &data);
            InsertBSTNode(&root, data);   //after completing reading, insert it into BST.
            continue;
        }
        if(c=='>'){//if user wants to find the successor of an element...
            fgetc(stdin);   //skip a space. what follows is the query.
            scanf("%u", &data);
            BSTNode* succ=Successor(root, data);
            if(succ!=NULL) printf("%u", succ->data);   //if there is no successor, don't try to print it.
            printf("\n");
            continue;          
        }
        if(c=='-'){
            fgetc(stdin);   //skip a space. what follows is the query.
            scanf("%u", &data);
            DeleteBSTNode(&root, BSTSearch(root, data, false));
            continue;
        }
        if(c=='S'){ //if the user wants to query an integer in the BST...
            fgetc(stdin); //skip a space. what follows is the query.
            scanf("%u", &data);
            BSTSearch(root, data, true);
            continue;
        }
        if(c=='P'){ //if the user wants to print the PreOrder traversal of the BST...
            fgetc(stdin);//to exhaust the \n after P....
            PreOrder(root);
            printf("\n");
            continue;
        }
        if(c=='C'){
            fgetc(stdin);
            scanf("%u", &data);
            BSTNode* node=BSTSearch(root, data, false);
            if(node!=NULL){
                if(!node->leftchild && !node->rightchild) printf("Nil Nil\n");
                else if(!node->leftchild) printf("Nil %u\n", node->rightchild->data);
                else if(!node->rightchild) printf("%u Nil\n", node->leftchild->data);
                else printf("%u %u\n", node->leftchild->data, node->rightchild->data);
            }
            else printf("-1\n");
        }
        if(c=='U'){
            fgetc(stdin);
            scanf("%u", &data);
            BSTNode* A=BSTSearch(root, data, false);
            if(A==NULL || A->parent==NULL || A->parent->parent==NULL){
                printf("-1\n");
                continue;
            }
            if(A->parent==A->parent->parent->leftchild){
                BSTNode* uncle=A->parent->parent->rightchild;
                if(uncle) printf("%u\n", uncle->data);
                else printf("-1\n");
                continue;
            }
            else{
                BSTNode* uncle=A->parent->parent->leftchild;
                if(uncle) printf("%u\n", uncle->data);
                else printf("-1\n");
                continue;
            }
            
        }
        if(c=='B'){
            //  Free previous BST first here.
            FreeBST(&root);
            c=fgetc(stdin);   //skip a space. Now numbers follow seperated by a space till a \n character.
            if(c=='\n') continue;
            while((digit=fgetc(stdin))!='\n'){  //while digit is not a '\n' we must insert the stream till a space.
                if(digit==' '){ // to skip a space and go to the next node.
                    //Insert the list into a BSTNode first here.
                    InsertBSTNode(&root, data);
                    continue;
                }
                ungetc(digit, stdin);   //push back into stream since it's a digit.
                scanf("%u", &data);   
            }
            InsertBSTNode(&root, data);   //this is to insert the last number in the input into the BST, which is not inserted by the above loop.
            continue;
        }
        if(c=='L'){
            fgetc(stdin);
            scanf("%u", &data);
            BSTNode* node=BSTSearch(root, data, false);
            if(node==NULL || node->rightchild==NULL) continue;
            else{
                BSTLeftRot(&root, node);
            }
        }
        if(c=='R'){
            fgetc(stdin);
            scanf("%u", &data);
            BSTNode* node=BSTSearch(root, data, false);
            if(node==NULL || node->leftchild==NULL) continue;
            else BSTRightRot(&root, node);

        }
    }
    // after EOF is marked, the recently used tree must be freed.
    FreeBST(&root);


    return 0;
}