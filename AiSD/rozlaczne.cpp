#include <iostream>
#include <algorithm>
#define MAX 100

using namespace std;

int index;

struct node{
	int key;
	struct node *up;
	int rank;
};

struct edge{
    struct node *start, *end;
    int weight;
};

struct graf{
    struct node *tops[MAX];
    int topsAmount;
    struct edge *edges[MAX];
    int edgesAmount;
};

void makeSet(struct node *z, int key){
    z->up = z;
    z->rank = 0;
    z->key = key;
}

struct node *findSet(struct node *x){
    if(x!=x->up)
        x->up = findSet(x->up);
    return x->up;
};

struct node *returnPath(struct node *x){
    cout<<"klucz: "<<x->key<<" ranga: "<<x->rank<<endl;
    if(x!=x->up)
        {
            returnPath(x->up);
        }
}

void link(struct node *x, struct node *y){
    if(x->rank>y->rank)
    {
        y->up=x;
    }
    else
    {
        x->up=y;
        if(x->rank == y->rank)
            y->rank++;
    }
}

void unionFun(struct node *x, struct node *y){
    link(findSet(x), findSet(y));
}

bool acompare(edge lhs, edge rhs) { return lhs.weight < rhs.weight; }

int cmpfunc (const void * a, const void * b)
{
    edge *lhs;
    edge *rhs;
    lhs = (edge *)a;
    rhs = (edge *)b;
   return lhs->weight - rhs->weight;
}

struct edge *MSTKruskal(struct graf *G){

    struct edge *A = new edge[MAX];
    index=0;

    //sort(G->edges[0], G->edges[G->edgesAmount-1], acompare);
    qsort(G->edges[0], 7, sizeof(edge), cmpfunc);
		for(int i=0; i<G->edgesAmount; i++)
    {
        cout<<G->edges[i]->start->key<<" ";
				cout<<G->edges[i]->end->key<<" "<<G->edges[i]->weight<<endl;
    }

    for(int i=0; i<G->edgesAmount; i++)
    {
        if(findSet(G->edges[i]->start) != findSet(G->edges[i]->end))
        {
            A[index].start = G->edges[i]->start;
            A[index].end = G->edges[i]->end;
            A[index].weight = G->edges[i]->weight;
            index++;
            unionFun(G->edges[i]->start, G->edges[i]->end);
						cout<<"union na "<<G->edges[i]->start->key<<" "<<G->edges[i]->end->key<<endl;

        }
				else
				cout<<"odrzucam "<<G->edges[i]->start->key<<" "<<G->edges[i]->end->key<<endl;
    }
    return A;
}


int main()
{
/*
    struct node *values[11];
    for(int i=0; i<10; i++)
    {
        values[i] = new node;
        makeSet(values[i],i);
    }
    unionFun(values[0], values[1]);
    unionFun(values[2], values[3]);
    unionFun(values[1], values[2]);
    unionFun(values[5], values[6]);
    unionFun(values[7], values[8]);
    unionFun(values[3], values[5]);
    unionFun(values[0], values[7]);
    for(int i=0; i<10; i++)
    {
        cout<<"sciezka klucza "<<i<<endl;
        returnPath(values[i]);
    }
*/
    struct graf *Graf = new graf;
    Graf->topsAmount=5;


		struct node *Tops[6];
		Tops[1] = new node; makeSet(Tops[1], 1);
		Tops[2] = new node; makeSet(Tops[2], 2);
		Tops[3] = new node; makeSet(Tops[3], 3);
		Tops[4] = new node; makeSet(Tops[4], 4);
		Tops[5] = new node; makeSet(Tops[5], 5);
		Tops[0] = new node; makeSet(Tops[0], 0);


    struct edge *Edges = new edge[7];
    Edges[0].start = Tops[1];
		Edges[0].end = Tops[5];
		Edges[0].weight = 2;
		Edges[1].start = Tops[1];
		Edges[1].end = Tops[2];
		Edges[1].weight = 1;
		Edges[2].start = Tops[2];
		Edges[2].end = Tops[3];
		Edges[2].weight = 5;
		Edges[3].start = Tops[3];
		Edges[3].end = Tops[4];
		Edges[3].weight = 7;
		Edges[4].start = Tops[4];
		Edges[4].end = Tops[5];
		Edges[4].weight = 3;
		Edges[5].start = Tops[2];
		Edges[5].end = Tops[5];
		Edges[5].weight = 1;
		Edges[6].start = Tops[2];
		Edges[6].end = Tops[4];
		Edges[6].weight = 2;

		Graf->tops[0] = Tops[1];
		Graf->tops[1] = Tops[2];
		Graf->tops[2] = Tops[3];
		Graf->tops[3] = Tops[4];
		Graf->tops[4] = Tops[5];

    Graf->edges[0] = &Edges[0];
    Graf->edges[1] = &Edges[1];
    Graf->edges[2] = &Edges[2];
    Graf->edges[3] = &Edges[3];
		Graf->edges[4] = &Edges[4];
		Graf->edges[5] = &Edges[5];
		Graf->edges[6] = &Edges[6];
    Graf->edgesAmount = 7;

    struct edge *A = MSTKruskal(Graf);

    for(int i=0; i<index; i++)
    {
        cout<<"start: "<<A[i].start->key<<" end: "<<A[i].end->key<<" weight: "<<A[i].weight<<endl;
    }

    return 0;
}
