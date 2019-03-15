//Assignment 5 
#include<stdio.h>
#include<stdlib.h>
struct Vertex {
	int data;
	char color;
	int d;
	struct Vertex* left, *right;
};
typedef struct Vertex Vertex;

/* Link-Listed Queue Implementation starts here. */
typedef struct queue{
    int data;
    struct queue* left;
    struct queue* right;
} queue;
typedef struct Queue{
    queue* head;
    queue* tail;
} Queue;

Queue* InitQueue(){
    Queue* temp = (Queue*)malloc(sizeof(Queue));
    temp->head=NULL;temp->tail=NULL;
	return temp;
}

void enqueue(Queue* init_Queue, int a){
    if(!init_Queue->head){
        init_Queue->head=(queue*)malloc(sizeof(queue));
        init_Queue->tail=init_Queue->head;
        init_Queue->tail->right=NULL;
        init_Queue->head->left=NULL;
        init_Queue->head->data=a;
        return;
    }
    queue* temp=(queue*)malloc(sizeof(queue));
    temp->left=init_Queue->tail;
    init_Queue->tail->right=temp;
    temp->right=NULL;
    init_Queue->tail=temp;
    temp->data=a;
}

int dequeue(Queue* init_Queue){
    if(!init_Queue->head) return -1;
    int a;
    queue* temp=init_Queue->head;
    if(temp->right) temp->right->left=NULL;
    init_Queue->head=temp->right;
    a = temp->data;
    free(temp);
    return a;
}
void printqueue(Queue* init_Queue){
    for(queue* temp=init_Queue->head;temp!=NULL;temp=temp->right){
        printf("%d ", temp->data);
    }
    printf("\n");
}

void FreeQueue(Queue** init_Queue){
	if(!(*init_Queue)) return;
	queue* tempa;
	for(queue* temp=(*init_Queue)->head;temp!=NULL;){
		tempa=temp->right;
		free(temp);
		temp=tempa;
	}
	free(*(init_Queue));
	init_Queue=NULL;
}

/* Queue Implementation ends here. */

Queue* BFS(Vertex* Array, int s, int N){
	for(int i=1;i<=N;i++){	//This loop just makes all colors white.
		(Array+i)->color='W';
		for(Vertex* temp=(Array+i)->right; temp!=NULL; temp=temp->right){
			temp->color='W';
		}
	}
	Queue* result = InitQueue();	//Intializing a queue for storing the result...
	Queue* bfsqueue = InitQueue();	//Intializing a queue for the algorithm...
	enqueue(bfsqueue, s);	//enqueue s.
	int u;	
	while(bfsqueue->head){	//While the queue contains entries...
		u=dequeue(bfsqueue);	//dequeue and entry...
		enqueue(result, u);
		(Array+u)->color='G';	//color it gray...
		for(Vertex* temp=Array[u].right;temp!=NULL;temp=temp->right){	//enqueue all those vertices...
			if((Array+(temp->data))->color=='W'){	//which are not yet covered...
				(Array+(temp->data))->color='G';	//color them gray...
				//enqueue(result, temp->data);
				enqueue(bfsqueue, temp->data);	//enqueue those vertices.
			}
		}
		(Array+u)->color='B';
	}
	printqueue(result);
	FreeQueue(&bfsqueue);
	return result;
}

void printpath(int* Path, int s, int d, int N){//Prints the shortest path. 'Path' contains the predecessors for each index.
	if(!Path){
		printf("-1\n");
		return;
	}
	int temp=d;
	Queue* Q = InitQueue();
	while(temp>0 && temp<=N && temp!=s){
		enqueue(Q, temp);
		temp=Path[temp];
	}
	enqueue(Q, s);
	for(queue* tempa=Q->tail;tempa!=NULL;tempa=tempa->left){
		printf("%d ", tempa->data);
	}
	printf("\n");
	FreeQueue(&Q);
}

int* FindPath(Vertex* Array, int s, int d, int N){  //Performs BFS from s to d.
	for(int i=1;i<=N;i++){	//This loop just makes all colors white.
		(Array+i)->color='W';
		for(Vertex* temp=(Array+i)->right; temp!=NULL; temp=temp->right){
			temp->color='W';
		}
	}
	int* Path = (int*)malloc((N+1)*sizeof(int));
	Queue* bfsqueue = InitQueue();	//Intializing a queue for the algorithm...
	enqueue(bfsqueue, s);	//enqueue s.
	int u;	
	while(bfsqueue->head){	//While the queue contains entries...
		u=dequeue(bfsqueue);	//dequeue and entry...
		(Array+u)->color='G';	//color it gray...
		for(Vertex* temp=Array[u].right;temp!=NULL;temp=temp->right){	//enqueue all those vertices...
			if((Array+(temp->data))->color=='W'){	//which are not yet covered...
				Path[temp->data]=u;
				if(temp->data==d){
					printpath(Path, s, d, N);   //if d is found, print the path.
					FreeQueue(&bfsqueue);
					return Path;
				}
				(Array+(temp->data))->color='G';	//color them gray...	
				enqueue(bfsqueue, temp->data);	//enqueue those vertices.
			}
		}
		(Array+u)->color='B';
	}
	FreeQueue(&bfsqueue);
	printf("-1\n");
	return NULL;
}

void FreeGraph(Vertex** Array, int N){  //Frees a Graph.
	Vertex* tempa;
	for(int i=1;i<=N;i++){
		for(Vertex* temp=(*(Array))[i].right; temp!=NULL;){
			tempa=temp->right;
			free(temp);
			temp=tempa;
		}
	}
	free(*Array);
	*Array=NULL;
}
int main() {
	int c = 0, u=0;
	int N = 0;
	int i=-11;
	int* Path=NULL;
	int temp, tempa=-1, tempb=-1;
	Queue* result=NULL;
	Vertex* Array=NULL;
	Vertex* temp_node=NULL;
	while ((c = fgetc(stdin)) != EOF) {
		if (c == 'N') {
			FreeGraph(&Array, N);
			Array=NULL;
			scanf("%d", &N);	//Read the number of nodes.
			Array = (Vertex*) malloc((N+1)*sizeof(Vertex));
			temp_node=Array;
		}
		if (c == 'E') {	//if the user wants to enter an adjacency list entry...
			scanf("%d", &temp);	//the first number should be stored in the array.
			Array[temp].data = temp;
			Array[temp].left=NULL;
			temp_node=Array+temp;
			while ((c = fgetc(stdin)) != '\n') {	//While enter is not seen...
				if (c == ' ') continue;// If a space is detected, continue.
				ungetc(c, stdin);
				//The number is to be detected... hence push it back.
				scanf("%d", &temp);
				(temp_node)->right=(Vertex *)malloc(sizeof(Vertex));
				temp_node->right->left=temp_node;
				temp_node=temp_node->right;
				temp_node->data=temp;
			}
			temp_node->right=NULL;
		}
		if (c == '?'){	//if the user wants to query the Graph...
			fgetc(stdin);	//Just to consume the space in input.
			scanf("%d", &temp);
			fgetc(stdin);
			scanf("%d", &c);
			temp_node=(Array+temp)->right;
			while(temp_node!=NULL){
				if(temp_node->data==c){
					printf("1\n");
					break;
				}
				temp_node=temp_node->right;
			}
			if(temp_node==NULL) printf("0\n");

		}
		
		if (c == 'B'){	//perform a breadth first search on the Graph...
			fgetc(stdin);
			scanf("%d", &u);
			if(tempa==u) printqueue(result);    //Doesn't perform the BFS again.
			else{
				FreeQueue(&result);
				result=BFS(Array, u, N);
				tempa=u;
			}
		}

		if(c=='P'){	//perform a BFS from u till v.
			fgetc(stdin);
			int s, d;
			scanf("%d", &s);
			fgetc(stdin);
			scanf("%d", &d);
			if(s==i && d==tempb) printpath(Path, s, d, N);  //Doesn't perform the BFS again.
			else{
				free(Path);
				Path=NULL;
				Path=FindPath(Array, s, d, N);
				i=s;tempb=d;
			}
		}
	}
	FreeGraph(&Array, N);
	free(Path);
	FreeQueue(&result);

	return 0;
}
