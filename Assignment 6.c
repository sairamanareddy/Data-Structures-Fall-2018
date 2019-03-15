//Assignment 6
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
typedef struct Adj_Vertex{
	int data;
	int weight;
	struct Adj_Vertex* right;
} Adj_Vertex;
struct Vertex {
	int data;
	long unsigned int distance;
	struct Adj_Vertex* right;
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
	/* Function to create a Queue Instance. */
    Queue* temp = (Queue*)malloc(sizeof(Queue));
    temp->head=NULL;temp->tail=NULL;
	return temp;
}

void enqueue(Queue* init_Queue, int a){
    if(!init_Queue->head){ /* Inserting for the first time? */
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
	/* Function to print a Queue */
    for(queue* temp=init_Queue->head;temp!=NULL;temp=temp->right){
        printf("%d ", temp->data);
    }
    printf("\n");
}

void FreeQueue(Queue** init_Queue){
	/* Function to free a given Queue. */
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


/* Priority Queue implementation starts here */
typedef struct MinHeap{
    Vertex* Array;
    int length;
    int HeapSize;
} MinHeap;

void swap_int(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}
void swap(Vertex* a, Vertex* b){
    Vertex temp = *a;
    *a = *b;
    *b = temp;
}
MinHeap* InitHeap(int HeapCap){
	/* Function to create a Heap Instance. */
    MinHeap* Heap = malloc(sizeof(MinHeap));
    Heap->Array = (Vertex *) malloc((HeapCap+1) * sizeof(Vertex));
    Heap->length = HeapCap;
    Heap->HeapSize=0;
    return Heap;
}
void Min_Heapify(MinHeap* Heap, int i, int* Index){
	/* The Main Iterative Heapify procedure that fixes the Heap */
    int l=0, r=0;
    while(i<=Heap->HeapSize){
        l = (i << 1) ;
        r = (i << 1) +1;
        int smallest=i;
        if(l<=Heap->HeapSize && Heap->Array[l].distance<Heap->Array[i].distance) smallest=l;
        if(r<=Heap->HeapSize && Heap->Array[r].distance<Heap->Array[smallest].distance) smallest=r;
        if(smallest!=i){
            swap(Heap->Array+i, Heap->Array+smallest);
			swap_int(Index+(Heap->Array[i]).data, Index+(Heap->Array[smallest]).data);
            i=smallest;
        }
        else break;
    }
}

Vertex* Minimum(MinHeap* Heap){
	/* peeks into the Heap and returns the minimum. */
    return Heap->Array+1;
}

int ExtractMin(MinHeap* Heap, int* Index){
	//Returns the Min and deletes it.
    Vertex min = Heap->Array[1];
    Heap->Array[1]=Heap->Array[Heap->HeapSize];
	Index[Heap->Array[Heap->HeapSize].data]=1;
    Heap->HeapSize-=1;
    Min_Heapify(Heap, 1, Index);
    return min.data;
}

void DecreaseKey(MinHeap* Heap, int i, int key, int* Index){
	//function to decrease key at index i in the Heap.
    int j=0;
    Heap->Array[i].distance = key;
	/* below two lines are for j = parent[i] */
    if(i%2==0) j = i >> 1;
    else j = (i-1) >> 1;
    while(i>0 && j>0 && Heap->Array[j].distance>Heap->Array[i].distance){
        swap(Heap->Array+j, Heap->Array+i);
		swap_int(Index+(Heap->Array[j].data), Index+(Heap->Array[i].data));
        i=j;
		/* below two lines are for j = parent[i] */
        if(i%2==0) j = i >> 1;
        else j = (i-1) >> 1;
    }
	return;
}
void Insert(MinHeap* Heap, Vertex key, int* Index){
    if(Heap->HeapSize==Heap->length) return; //attempting to insert out of bounds.
    Heap->HeapSize++;
    Heap->Array[Heap->HeapSize].distance = INT_MAX;
	Heap->Array[Heap->HeapSize].data = key.data;
	Index[key.data] = Heap->HeapSize;
    DecreaseKey(Heap, Heap->HeapSize, key.distance, Index);
}

void FreeHeap(MinHeap** Heap, int** Index){
	free(*Index);
	*Index = NULL;
	free((*Heap)->Array);
	free(*Heap);
	*Heap = NULL;
}

/* Priority Queue Implementation ends here. */

void printpath(int* Path, int s, int d, int N){
	/* Prints the shortest path. 'Path' contains the predecessors for each index. */
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

void ShortestPath(Vertex* Array, int N, int u, int d){
	/* Prints the shortest path form u to d */
	int a;
	/* Contains the predecessor of an element. */
	int* Path = (int *)malloc((N+1) * sizeof(int));
	int* Index = (int *)malloc((N+1) * sizeof(int));

	/* Intialize all elements' distances to Infinity. */
	for(int i=1;i<=N;i++){
		Array[i].distance = INT_MAX;
	}

	/* Distance of u is zero trivially */
	Array[u].distance = 0;

	/* Intialize a Heap of size N */
	MinHeap* Heap = InitHeap(N);

	/* Insert all elements into the Heap. */
	for(int i=1;i<=N;i++){
		Insert(Heap, Array[i], Index);
	}

	while(Heap->HeapSize){
		a = ExtractMin(Heap, Index);

		/* If all reachable vertices have been visited */
		if(Array[a].distance==INT_MAX) break;

		/* For all elements adjacent to a */
		for(Adj_Vertex* temp=Array[a].right;temp!=NULL;temp=temp->right){

			/* If a shorter distance is found */
			if(Array[a].distance+temp->weight<Array[temp->data].distance){

				/* Change the distance. */
				Array[temp->data].distance = Array[a].distance + temp->weight;

				/* Change the priority in the Priority Queue */
				DecreaseKey(Heap, Index[temp->data], Array[temp->data].distance, Index);

				/* Make note of the predecessor. */
				Path[temp->data] = a;
			}
		}
	}
	/* If d is not reachable... */
	if(Array[d].distance == INT_MAX) {
		FreeHeap(&Heap, &Index);
		free(Path);
		printf("-1\n");
		return;
	}
	else{
	/* d is reachable. Print the distance*/
	printf("%ld ", Array[d].distance);
	/* Then prints the path. */
	printpath(Path, u, d, N);
	FreeHeap(&Heap, &Index);
	free(Path);
	}
	
	
}

void Djikstra(Vertex* Array, int N, int u){
	/* Djikstra walk from Vertex u .
	Prints the Walk*/
	int a;
	int* Index = (int *)malloc((N+1) * sizeof(int));

	/* Intialize all distances to infinity, here, INT_MIN since distance is less than this. */
	for(int i=1;i<=N;i++){
		Array[i].distance = INT_MAX;
	}
	Array[u].distance = 0;

	/* Intialize a Priority Queue with size N */
	MinHeap* Heap = InitHeap(N);

	/* Insert all vertices into the Heap. */
	for(int i=1;i<=N;i++){
		Insert(Heap, Array[i], Index);
	}
	while(Heap->HeapSize){
		a = ExtractMin(Heap, Index);

		/* If all reachable vertices have been visited... */
		if(Array[a].distance==INT_MAX) break;

		/* Print the Vertex */
		printf("%d %ld\n", a, Array[a].distance);

		/* For all vertices adjacent to a, */
		for(Adj_Vertex* temp=Array[a].right;temp!=NULL;temp=temp->right){

			/* If we have a shorter distance than existing */
			if(Array[a].distance+temp->weight<Array[temp->data].distance){

				/* Change the distance */
				Array[temp->data].distance = Array[a].distance + temp->weight;

				/* Decrease the key. */
				DecreaseKey(Heap, Index[temp->data], Array[temp->data].distance, Index);
			}
		}
	}
	for(int i=1;i<=N;i++){

		/* Print -1 for all unreachable vertices. */
		if(Array[i].distance == INT_MAX) printf("%d -1\n", i);
	}

	/* Free the Heap */
	FreeHeap(&Heap, &Index);
}

void FreeGraph(Vertex** Array, int N){
	Adj_Vertex* tempa;
	for(int i=1;i<=N;i++){
		/* This loop frees the adjacency list */
		for(Adj_Vertex* temp=(*(Array))[i].right; temp!=NULL;){
			tempa=temp->right;
			free(temp);
			temp=tempa;
		}
	}
	/* Free the array. */
	free(*Array);
	*Array=NULL;
}
int main() {
	int c = 0, u=0;
	int N = 0;
	int i=-1;
	int* Path=NULL;
	int temp, tempa=-1, tempb=-1;
	Queue* result=NULL;
	/* The actual Array representing the Graph */
	Vertex* Array=NULL;
	Vertex* temp_node=NULL;
	while ((c = fgetc(stdin)) != EOF) {


		if (c == 'N') {
			/* Free the Previous array */
			FreeGraph(&Array, N);
			scanf("%d", &N);	//Read the number of nodes.

			/* Allocate memory for storing the Vertices */
			Array = (Vertex*) malloc((N+1)*sizeof(Vertex));
			for(int i=1;i<=N;i++){
				Array[i].data = i;
				Array[i].right = NULL;
			}
		}


		if (c == 'E') {	//if the user wants to enter an adjacency list entry...
			int temp=0, tempa=0;
			scanf("%d", &temp);	//the first number should be stored in the array.
			Array[temp].data = temp;
			temp_node=Array+temp;
			/* If no vertices can be reached from Vertex */
			if((c=fgetc(stdin))=='\n'){
				temp_node->right=NULL;
				/* Read the Next Request. */
				continue;
			}

			/* Allocate memory for adjacency list vertex */
			temp_node->right = (Adj_Vertex*)malloc(sizeof(Adj_Vertex));
			Adj_Vertex* node = temp_node->right;

			scanf("%d", &temp);	//The vertex.

			scanf("%d", &tempa);	//Its weight.

			node->data = temp;node->weight=tempa;
			while ((c = fgetc(stdin)) != '\n') {	//While enter is not seen...

				if (c == ' ') continue;// If a space is detected, continue.

				ungetc(c, stdin);
				//The number is to be detected... hence push it back.
				scanf("%d", &temp);	//The vertex.

				scanf("%d", &tempa);	//Its weight.
				(node)->right=(Adj_Vertex *)malloc(sizeof(Adj_Vertex));
				node=node->right;

				//Fill the node with details.
				node->data=temp;
				node->weight=tempa;
			}
			node->right=NULL;
		}


		if (c == '?'){
			/* Graph Weight Query */
			int temp=0, c=0;
			fgetc(stdin);	//Just to consume the space in input.
			/* Vertex 1 */
			scanf("%d", &temp);
			fgetc(stdin);
			/* Vertex 2 */
			scanf("%d", &c);
			Adj_Vertex* temp_node=(Array+temp)->right;	//temp_node points to first neighbour.
			while(temp_node!=NULL){
				/* If matched, then print weight and stop searching */
				if(temp_node->data==c){
					printf("%d\n", temp_node->weight);
					break;
				}
				temp_node=temp_node->right;
			}
			/* If Vertex 2 is not found in AdjList of Vertex 1 */
			if(temp_node==NULL) printf("-1\n");

		}

		
		if (c == 'D'){
			/* perform a Djikstra's walk on the Graph starting from u. */
			int u;
			fgetc(stdin);
			scanf("%d", &u);
			Djikstra(Array, N, u);
			
		}

		if(c=='P'){	
			/* Print the shortest path. */
			int a=0, b=0;
			fgetc(stdin);
			/* Source */
			scanf("%d", &a);
			fgetc(stdin);
			/* Destination */
			scanf("%d", &b);
			/* Compute Shortest Path */
			ShortestPath(Array, N, a, b);
		}
	}
	FreeGraph(&Array, N);
	free(Path);
	FreeQueue(&result);

	return 0;
}
