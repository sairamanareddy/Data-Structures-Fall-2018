//Assignment 7
#include<stdio.h>
#include<stdlib.h>
typedef struct SetNode {
	int data;
	int rank;
	struct SetNode* parent;
} SetNode;

SetNode* MakeSet(int data) {
	/* Create a new tree for a single node with x */
	SetNode* x = (SetNode*)malloc(sizeof(SetNode));
	/* Fill the node. */
	x->data = data;
	x->parent = x;
	x->rank = 1;
	return x;	
}
SetNode* FindSet(SetNode* x){
	/* Returns the address of the set representative. */
	SetNode* temp = x;
	/* While the representative has not been reached, */
	while(temp->parent!=temp){
		temp = temp->parent;
	}
	return temp;	
}
void Union(SetNode* a, SetNode* b){
	/* Performs Union of sets containing a and b */
	SetNode* r_a=FindSet(a), *r_b=FindSet(b);
	/* If r_a, r_b are same, a, b belong to the same set. */
	if(r_a==r_b) return;


	/* Attach Sa to Sb */
	if(r_a->rank<r_b->rank){
		r_a->parent = r_b;
	}


	/* Attach Sb to Sa */
	else{
		r_b->parent=r_a;
	}
	/* Increment rank of Sa */
	if(r_a->rank==r_b->rank){
		r_a->rank++;
	}
	
}

void FreeForest(SetNode*** Array, int N){
	/* Frees the Collection of trees. */
	for(int i=0;i<N;i++){
		free(*(*Array+i));
	}
	free(*Array);
	*Array = NULL;
}


SetNode** InitForest(int N){
	/* Function to Intialize a Collection of trees */
	SetNode** Array = (SetNode**)malloc(N*sizeof(SetNode*));
	for(int i=0;i<N;i++){
		Array[i] = MakeSet(i+1);
	}
	return Array;
}

int main(){
	int read=0, N=0;
	SetNode** Array = NULL;
	while((read=fgetc(stdin))!=EOF){
		if(read=='N'){
			FreeForest(&Array, N);
			scanf("%d", &N);
			Array = InitForest(N);
		}
		if(read=='?'){
			int a, b;
			scanf("%d", &a);
			scanf("%d", &b);
			/* If invalid element is given, */
			if(a>N || b>N || a<1 || b<1){
				printf("-1\n");
				continue;
			}
			if(FindSet(Array[a-1])==FindSet(Array[b-1])) printf("1\n");
			else printf("0\n");
		}
		if(read=='S'){
			int a;
			scanf("%d", &a);
			/* If invalid element is given, */
			if(a<1 || a>N){
				printf("-1\n");
				continue;
			}
			printf("%d\n", FindSet(Array[a-1])->data);
		}
		if(read=='U'){
			int a, b;
			scanf("%d", &a);
			scanf("%d", &b);
			/* If invalid element is given, */
			if(a>N || b>N || a<1 || b<1) continue;
			Union(Array[a-1], Array[b-1]);
		}
		if(read=='R'){
			int a;
			scanf("%d", &a);
			/* If invalid element is given, */
			if(a>N || a<1){
				printf("-1\n");
				continue;
			}
			printf("%d\n", (Array[a-1])->rank);
		}
	}
	FreeForest(&Array, N);
	return 0;
}
