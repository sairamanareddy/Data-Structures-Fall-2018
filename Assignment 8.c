//Assignment 8
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
struct Node{
    struct Node* left;
    int data;
    struct Node* right;
};
typedef struct RBTNode{
    struct RBTNode* parent;
    unsigned int data;
    char color;
    struct RBTNode* leftchild;
    struct RBTNode* rightchild;
} RBTNode;

static RBTNode Sentinel = {NULL, 0, 'B', NULL, NULL};

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

int compare(unsigned int data1, unsigned int data2){
    if(data1>data2) return 1;
    else if(data1<data2) return 0;
    else return -1;
}

void Recolor(RBTNode* Node){
    if(!Node) return;
    if(Node->color=='R'){
        Node->color='B';
    }
    else{
        Node->color='R';
    }
}

void RBTSplice(RBTNode** root, RBTNode* node1, RBTNode* node2){
    //function to splice a node1 and replace it with node2
    //Doesn't update parent of node1.(Gives versatility)
    if(node1->parent==NULL) *root=node2;
    else if(node1==node1->parent->leftchild) node1->parent->leftchild=node2;
    else node1->parent->rightchild=node2;
    if(node2!=NULL) node2->parent=node1->parent;
}

void RBTLeftRot(RBTNode** root, RBTNode* node){
    if(!node || !(node->rightchild) || (node->rightchild)==&Sentinel) return;
    //function to left rotate at node.
    //Assumes left rotation is really possible at node.
    RBTNode* node2=node->rightchild;
    RBTSplice(root, node, node2);
    node->rightchild=node2->leftchild;
    node2->leftchild=node;
    node->parent=node2;
    if(node->rightchild!=&Sentinel && node->rightchild!=NULL) node->rightchild->parent=node;
}

void RBTRightRot(RBTNode** root, RBTNode* node){
    if(!node || !(node->leftchild) || (node->leftchild)==&Sentinel) return;
    //function to right rotate at node.
    //Assumes right rotation is really possible at node.
    RBTNode* node2=node->leftchild;
    RBTSplice(root, node, node2);
    node->leftchild=node2->rightchild;
    node2->rightchild=node;
    node->parent=node2;
    if(node->leftchild!=&Sentinel && node->rightchild!=NULL) node->leftchild->parent=node;

}

void PreOrder(RBTNode* root){
    //Recursive function for PreOrder traversal.
    RBTNode* temp=root;
    if(temp!=NULL && temp!=&Sentinel){
        printf("%u", temp->data);printf(" ");
        PreOrder(temp->leftchild);
        PreOrder(temp->rightchild);
    }
}

RBTNode* RBTSearch(RBTNode* root, unsigned int data, bool printflag){
    //Function for iterative RBTSearch with an arbitrary large query.
    //prints the path if printflag is set, and returns the node if found. else returns NULL.
    struct Node* pathhead=NULL,*pathtail=NULL;
    if(root==NULL) {
        printf("-1\n");
        return NULL;
    }
    RBTNode* temp=root;
    int c=0;
    while(temp!=&Sentinel){
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
                printf(" %c", temp->color);
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

void FreeRBT(RBTNode** root){
    //Recursive function to free a given RBT.
    if(*root==NULL) return;
    RBTNode* temp=*root;
    if(temp!=&Sentinel){
        RBTNode* templ=temp->leftchild;
        RBTNode* tempr=temp->rightchild;
        FreeRBT(&templ);
        FreeRBT(&tempr);
        free(temp);
    }
    *root=NULL;
}

RBTNode* Uncle(RBTNode* root, RBTNode* Node){
    if(Node==NULL || Node->parent==NULL || Node->parent->parent==NULL)
        return NULL;
    if(Node->parent==Node->parent->parent->leftchild){
        RBTNode* Uncle= Node->parent->parent->rightchild;
        if(!Uncle) return NULL;
        return Uncle;
    }
    else{
        RBTNode* Uncle = Node->parent->parent->leftchild;
        if(!Uncle) return NULL;
        return Uncle;
    }
    return NULL;
}

RBTNode* Successor(RBTNode* root, unsigned int data, bool printflag){
    //this function finds the successor of given element and prints the appropriate result.
    //returns the successor of an element if exists. Else returns NULL.
    if(root==NULL){
        if(printflag) printf("-1");
        return NULL;
    }   //just a failsafe.
    RBTNode* temp1=NULL;   //define a traversal RBTNode.
    RBTNode* temp=root;
    int insert=0;
    while(temp!=&Sentinel){  //this loop finds finds appropriate place to insert the new node.
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
        if(temp==NULL) {
            if(printflag) printf("-1");
            return NULL;
        }
        return temp;
    }
    else if(insert==0) return temp1;  //virtual node is the left child of temp1.
    else{                              //virtual node is already present at temp in the tree.
        if(temp->rightchild!=&Sentinel){//if there's a rightchild...
            temp1=temp->rightchild;     //go to the right child...
            while(temp1->leftchild!=&Sentinel){
                temp1=temp1->leftchild;     //and keep on traversing to the leftchild...
            }
            return temp1;
        }
        temp1=temp->parent; //if there's no right child...
        while(temp1!=NULL && temp==temp1->rightchild){  //while we are still the right child of our parent...
            temp=temp1;
            temp1=temp1->parent;    //keep on traversing towards our ancestors...
        }
        if(temp1==NULL) {
            if(printflag) printf("-1");
            return NULL;
        }   //there's no left subtree that we're in. So there's no successor.
        return temp1;
    }
}

RBTNode* Predecessor(RBTNode* root, RBTNode* Node){
    /* Assumes Node has a leftchild. */
    RBTNode* temp1, *temp=Node->leftchild;
    while(temp!=&Sentinel){
        temp1=temp;
        temp=temp->rightchild;
    }
    return temp1;
}



void FixInsert(RBTNode** root, RBTNode* Node){
    /* Whlie parent is red */
    while(Node->parent && Node->parent->color=='R'){
        RBTNode* U = Uncle(*root, Node);
        /* Node.parent is leftchild */
        if(Node->parent->parent && Node->parent==Node->parent->parent->leftchild){
            if(U && U->color=='R'){
                Recolor(Node->parent);
                Recolor(U);
                Recolor(Node->parent->parent);
                Node = Node->parent->parent;
                continue;
            }
            /* Otherwise.= */
            else{
				if (Node == Node->parent->rightchild) {
					Node = Node->parent;
					RBTLeftRot(root, Node);
                    continue;
				}
				Recolor(Node->parent);
				Recolor(Node->parent->parent);
				RBTRightRot(root, Node->parent->parent);
                return;
            }
        }
        /* Symmetric Case. */
		else if(Node->parent->parent && Node->parent == Node->parent->parent->rightchild) {
			if (U && U->color == 'R') {
				Recolor(Node->parent);
				Recolor(U);
				Recolor(Node->parent->parent);
				Node = Node->parent->parent;
                continue;
			}
			else {
				if (Node == Node->parent->leftchild) {
					Node = Node->parent;
					RBTRightRot(root, Node);
                    continue;
				}
				Recolor(Node->parent);
				Recolor(Node->parent->parent);
				RBTLeftRot(root, Node->parent->parent);
                return;
			}

		}
    }
    (*root)->color = 'B';
}

void InsertRBTNode(RBTNode** root, unsigned int data){
    //iterative function to insert a Tree node at the appropriate place.
    if(*root==NULL){
        *root=(RBTNode*)malloc(sizeof(RBTNode));
        (*root)->parent=NULL;(*root)->leftchild=&Sentinel;(*root)->rightchild=&Sentinel;
        (*root)->data=data;
        (*root)->color = 'B';
        return;
        /* No need to call FixInsert */
    }
    RBTNode* temp1=NULL;   //define a traversal RBTNode.
    RBTNode* temp=*root;
    int insert=0;
    while(temp!=&Sentinel){  //this loop finds finds appropriate place to insert the new node.
        temp1=temp; //just to store the previous value of temp as it'd get modified. later.
        insert=compare(data, temp->data);
        if(insert==1) temp=temp->rightchild;
        else if(insert==0) temp=temp->leftchild;
        else return;  //this would mean that the tree already has the node.
    }
    temp=(RBTNode*)malloc(sizeof(RBTNode));   //allocate memory for the new node.
    temp->parent=temp1;temp->leftchild=&Sentinel;temp->rightchild=&Sentinel;  //intializing the node.
    temp->data=data;
    temp->color='R';
    if(insert==0) temp1->leftchild=temp;    //more links
    if(insert==1) temp1->rightchild=temp;
    FixInsert(root, temp);
    
}

void FixToken(RBTNode** root, RBTNode* Node){
    /* Simply remove the token. */
    if(Node==*root){
        (*root)->color='B';
        return;
    }
    char M;
    /* To be left rotated. */
    if(Node->parent && Node==Node->parent->leftchild) M='L';
    else if(Node->parent) M='R';
    if(Node->color=='R'){
        Node->color='B';
        return;
    }
    /* Covers two cases in one using flag M */
    if(Node->color=='B'){
        RBTNode* S = NULL;
        if(Node->parent && Node==Node->parent->leftchild) S = Node->parent->rightchild;
        else if(Node->parent && Node==Node->parent->rightchild) S = Node->parent->leftchild;
        if(S==NULL) return;
        if(S->color=='R'){
        /* Sibling is Red. */
            Recolor(S);Recolor(S->parent);
            if(M=='L') RBTLeftRot(root, S->parent);
            else if(M=='R') RBTRightRot(root, S->parent);
            if(Node->parent && Node==Node->parent->leftchild) S = Node->parent->rightchild;
            if(Node->parent && Node==Node->parent->rightchild) S = Node->parent->leftchild;
        }
        /* Case 2 */
        if(S->color=='B' && S->leftchild && S->leftchild->color=='B' && S->rightchild && S->rightchild->color=='B'){
            /* Sibling is Black and has two black children */
            S->color='R';
            FixToken(root, S->parent);
            return;
        }
        /* Case 3 */
        if(S->color=='B' && S->leftchild && S->leftchild->color=='R' && S->rightchild && S->rightchild->color=='B'){
            /* Case3: Sibling is Black and has red leftchild, black rightchild */
            Recolor(S);Recolor(S->leftchild);
            RBTRightRot(root, S);
            if(Node->parent && Node==Node->parent->leftchild) S = Node->parent->rightchild;
            if(Node->parent && Node==Node->parent->rightchild) S = Node->parent->leftchild;
        }
        /* Case 4 */
        if(S->color=='B' && S->rightchild && S->rightchild->color=='R'){
            /* Case4: Sibling is black, has red rightchild */
            S->rightchild->color='B';
            S->color = S->parent->color;
            S->parent->color='B';
            RBTLeftRot(root, S->parent);
            FixToken(root, *root);
            return;

        }
    }

}

void DeleteRBTNode(RBTNode** root, RBTNode* Node){
    if(!Node) return;
    if(Node->leftchild!=&Sentinel && Node->rightchild!=&Sentinel){
    /* Case1: Node has two non leaf children */
        RBTNode* succ = Predecessor(*root, Node);
        Node->data = succ->data;
        DeleteRBTNode(root, succ);
        return;
    }
    else if(Node->leftchild!=&Sentinel){
    /* Case2: Node has leftchild non-leaf. */
        if(Node->color=='R'){
            /* Node is Red */
            RBTSplice(root, Node, Node->leftchild);
            free(Node);
            return;
        }
        else if(Node->color=='B' && Node->leftchild->color=='R'){
            /* Node is black and Child is Red */
            RBTSplice(root, Node, Node->leftchild);
            FixToken(root, Node->leftchild);
            free(Node);
        }
        else{
            /* Node is black and Child is black */
            RBTSplice(root, Node, Node->leftchild);
            FixToken(root, Node->leftchild);
            free(Node);
        }
    }
    else if(Node->rightchild!=&Sentinel){
    /* Case2: Node has rightchild non-leaf. */
        if(Node->color=='R'){
            /* M is Red */
            RBTSplice(root, Node, Node->rightchild);
            free(Node);
            return;
        }
        else if(Node->color=='B' && Node->rightchild->color=='R'){
            /* Node is black and Child is Red */
            RBTSplice(root, Node, Node->rightchild);
            FixToken(root, Node->rightchild);
            free(Node);
        }
        else{
            /* Node is black and Child is black */
            RBTSplice(root, Node, Node->rightchild);
            FixToken(root, Node->rightchild);
            free(Node);
        }
    }
    else{
        /* If root is the only element, delete it directly. */
        if(Node==*root){
            free(Node);
            *root = NULL;
            Node=NULL;
            return;
        }
        if(Node->color=='R'){
            /* Node is Red */
            RBTSplice(root, Node, Node->rightchild);
            free(Node);
            return;
        }
        else{
            /* Node is black and Child is black */
            RBTSplice(root, Node, Node->rightchild);
            FixToken(root, Node->rightchild);
            Sentinel.parent=NULL;
            free(Node);
            
        }
    }
}

int main(){
    int c=0, digit=0;
    unsigned int data=0;
    RBTNode* root=NULL;
    while((c=fgetc(stdin))!=EOF){   //constantly reads input from STDIN.
        if(c=='N'){ //if the user wants to insert a new set into RBT...

            //  Free previous RBT first here.
            FreeRBT(&root);
            c=fgetc(stdin);   //skip a space. Now numbers follow seperated by a space till a \n character.
            if(c=='\n') continue;
            while((digit=fgetc(stdin))!='\n'){  //while digit is not a '\n' we must insert the stream till a space.
                if(digit==' '){ // to skip a space and go to the next node.
                    //Insert the list into a RBTNode first here.
                    InsertRBTNode(&root, data);
                    continue;
                }
                ungetc(digit, stdin);   //push back into stream since it's a digit.
                scanf("%u", &data);   
            }
            InsertRBTNode(&root, data);   //this is to insert the last number in the input into the RBT, which is not inserted by the above loop.
            continue;
        }
        if(c=='+'){ //if the user wants to insert an element into the same set...
            fgetc(stdin);   //skip a space. what follows is the new element.
            scanf("%u", &data);
            InsertRBTNode(&root, data);   //after completing reading, insert it into RBT.
            continue;
        }
        if(c=='S'){ //if the user wants to query an integer in the RBT...
            fgetc(stdin); //skip a space. what follows is the query.
            scanf("%u", &data);
            RBTSearch(root, data, true);
            continue;
        }
        if(c=='P'){ //if the user wants to print the PreOrder traversal of the RBT...
            fgetc(stdin);//to exhaust the \n after P....
            PreOrder(root);
            printf("\n");
            continue;
        }
        if(c=='C'){//Print Children
            fgetc(stdin);
            unsigned int t=0;
            scanf("%u", &t);
            RBTNode* Node = RBTSearch(root, t, false);
            if(!Node) {
                printf("-1\n");
                continue;
            }
            else{
                if(Node->leftchild!=&Sentinel) printf("%d %c ", Node->leftchild->data, Node->leftchild->color);
                else printf("L B ");//Child is leaf
                if(Node->rightchild!=&Sentinel) printf("%d %c\n", Node->rightchild->data, Node->rightchild->color);
                else printf("L B\n");
            }

        }
        if(c=='>'){
            fgetc(stdin);
            unsigned int d=0;
            scanf("%u", &d);
            RBTNode* Node = Successor(root, d, true);//Print Successor
            if(Node) printf("%d", Node->data);
            printf("\n");            
        }
        if(c=='-'){
            fgetc(stdin);
            unsigned int d=0;
            scanf("%u", &d);
            RBTNode* Node = RBTSearch(root, d, false);//Search and Delete.
            if(Node) DeleteRBTNode(&root, Node);
        }
        
    }
    FreeRBT(&root);

    return 0;

}