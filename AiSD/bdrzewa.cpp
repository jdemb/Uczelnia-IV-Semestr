#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NODEKEYS 3+1

using namespace std;
int fileIndex = 0; int t=2;

string fileNameGenerate()
{
    char tab[8];
    sprintf(tab, "%d.txt", fileIndex);
    fileIndex++;
    string ret = tab;
    return ret;
}

struct node{
	int key[NODEKEYS];
	int keysAmount;
	string n[NODEKEYS+1];
	bool isLeaf;
	string fileName;
}*root, *readNode;

struct NodeAndIndex{
    int i;
    struct node *Y;
}*returnValue;

struct node* AllocateNode(string name)
{
    fstream file;
    file.open(name.c_str(), ios::out);
    file.close();
    struct node *tmp = new node;
    int i=1;
    while(i<NODEKEYS+1)
    {
        tmp->n[i] = "0";
        i++;
    }
    tmp->fileName = name;
    return tmp;
}

void DiskWrite(struct node *x)
{
    string name = x->fileName;
    fstream file;
    file.open(name.c_str(), ios::out);
    file << x->fileName << endl; // first line = file name
    int i = 1;
    file << x->keysAmount << " ";
    while(i <= x->keysAmount)
    {
        file << x->key[i] << " "; // second line = keys
        i++;
    }
    file << endl << x->isLeaf << endl; // third line = isLeaf
    i=1;
    while(i<NODEKEYS+1)
    {
        file << x->n[i] << " ";
        i++;
    }
    file.close();
}

void DiskRead(string name)
{
    fstream plik;
    plik.open(name.c_str(), ios::in);
    if( plik.good() == true )
    {
        struct node *read = new node;
        plik >> read->fileName;
        plik >> read->keysAmount;
        for( int i = 1; i<=read->keysAmount; i++)
            plik >> read->key[i];
        plik>>read->isLeaf;
        for( int i = 1; i<=NODEKEYS; i++)
            plik >> read->n[i];
        readNode = read;
        plik.close();
    }

}

void BTreeCreate()
{
    struct node *x = AllocateNode(fileNameGenerate());
    x->isLeaf = true;
    x->keysAmount = 0;
    DiskWrite(x);
    root = x;
}

string BTreeSearch(struct node *root, int k)
{
    int i = 1;
    while(i<=root->keysAmount && k>root->key[i])
        i++;
    if(i <= root->keysAmount && k == root->key[i])
    {
        return root->fileName;
    }
    if(root->isLeaf)
    {
        return NULL;
    }
    else
    {
        DiskRead(root->n[i]);
        return BTreeSearch(readNode, k);
    }
}

void BTreeSplitChild(struct node *x, int i, struct node *y)
{
    struct node *z = AllocateNode(fileNameGenerate());
    z->isLeaf = y->isLeaf;
    z->keysAmount = t-1;
    for(int j=1; j<= t-1; j++)
    {
        z->key[j] = y->key[j+t];
    }
    if(!y->isLeaf)
    {
        for(int j=1; j<=t; j++)
            z->n[j] = y->n[j+t];
    }
    y->keysAmount = t-1;
    for(int j = x->keysAmount+1; j>=i+1; j--)
    {
        x->n[j+1] = x->n[j];
    }
    x->n[i+1] = z->fileName;
    for(int j=x->keysAmount; j>=i; j--)
    {
        x->key[j+1] = x->key[j];
    }
    x->key[i] = y->key[t];
    x->keysAmount++;
    DiskWrite(y);
    DiskWrite(z);
    DiskWrite(x);
}

void BTreeInsertNonfull(struct node *x, int k)
{
    int i = x->keysAmount;
    if(x->isLeaf)
    {
        while(i>=1 && k<x->key[i])
        {
            x->key[i+1] = x->key[i];
            i--;
        }
        x->key[i+1] = k;
        x->keysAmount++;
        DiskWrite(x);
    }
    else
    {
        while(i>=1 && k<x->key[i])
        {
            i--;
        }
        i++;
        DiskRead(x->n[i]);
        if(readNode->keysAmount == 2*t-1)
        {
            BTreeSplitChild(x,i,readNode);
            if(k>x->key[i])
                i++;
        }
        BTreeInsertNonfull(readNode,k);
    }
}

void BTreeInsert(int k)
{
    struct node *r = root;
    if(r->keysAmount == 2*t-1)
    {
        struct node *s = AllocateNode(fileNameGenerate());
        root = s;
        s->isLeaf = false;
        s->keysAmount = 0;
        s->n[1] = r->fileName;
        BTreeSplitChild(s,1,r);
        BTreeInsertNonfull(s,k);
    }
    else
    {
        BTreeInsertNonfull(r,k);
    }
}



int main()
{
    root = NULL;
    BTreeCreate();
    for(int i=1; i<=10; i++)
    BTreeInsert(i);
    cout<<BTreeSearch(root, 5);

    return 0;
}
