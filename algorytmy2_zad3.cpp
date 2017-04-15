#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <bitset>
#include <string.h>
using namespace std;

struct node{
	int amount;
    char character;
    struct node *up;
    struct node *left;
    struct node *right;
}*root,*huffroot, S;
void setup_S()
{
	S.left = &S;
	S.right = &S;
	S.up = &S;
}

int huffcharacters=0; int characters=0;
queue <struct node*> kolejka;

struct node* tree_search(struct node *X, char k){
	if(X==&S || k==X->character)
		return X;
	if(k<X->character)
		return tree_search(X->left, k);
	else
		return tree_search(X->right, k);
}

void in_order_tree_walk(struct node *X){
	if(X!=&S){
		in_order_tree_walk(X->left);
		printf("%d %c\n", X->amount, X->character);
		in_order_tree_walk(X->right);
	}
}
struct node* tmp_node(char k, bool flag){
    struct node *tmp;
    if(flag)
        tmp = (struct node*)malloc(sizeof *root);
	else
        tmp = (struct node*)malloc(sizeof *huffroot);
	tmp->up = &S;
    tmp->left = &S;
    tmp->right = &S;
    tmp->character = k;
    tmp->amount = 1;
    return tmp;
}
void que_tree(struct node *X){
	if(X!=&S){
		que_tree(X->left);
		struct node *Z = tmp_node(X->character, 0);
		string binary = bitset<8>(X->character).to_string();
		characters+= binary.size()*X->amount;
		Z->amount = X->amount;
		kolejka.push(Z);
		que_tree(X->right);
	}
}
void tree_insert(struct node *T, struct node *Z, bool flag){
	struct node *Y = &S;
	struct node *X;
	if(flag)
        X = root;
    else
        X = huffroot;
	while(X != &S)
	{
	    Y = X;
	    if(Z->character < X->character) X=X->left;
	    else X=X->right;
    }
    Z->up = Y;
    if( Y == &S)
    {
        if(flag)
            root = Z;
        else
            huffroot = Z;
    }
    else if(Z->character < Y->character) Y->left = Z;
    else Y->right = Z;
}
struct node* tree_minimum(struct node *X){
	while(X->left != &S)
	X = X->left;
	return X;
}

struct node* que_min()
{
    int min_value, size = kolejka.size();
    struct node *X;
    X = kolejka.front();
    min_value = X->amount;
    while (size--)
    {
        X = kolejka.front();
        kolejka.pop();
        kolejka.push(X);
        if (X->amount < min_value)
            min_value = X->amount;
    }
    size = kolejka.size();
    while(size--)
    {
        X = kolejka.front();
        kolejka.pop();
        if(X->amount == min_value)
            break;
        kolejka.push(X);
    }
    return X;
}

struct node* Huffman()
{
    int n = kolejka.size();
    for(int i = 1; i<=n-1; i++)
    {
        struct node *Z = (struct node*)malloc(sizeof *huffroot);
        struct node *X, *Y;
        X = que_min();
        Y = que_min();
        Z->amount = X->amount + Y->amount;
        Z->left = X;
        Z->right = Y;
        kolejka.push(Z);
    }
    return que_min();
};

void huffman_tree_walk(struct node *X){
  if(X==NULL || X==&S) return;
  if((X->left==NULL || X->left==&S)&&(X->right==NULL || X->right==&S) )
  printf("%d %c\n", X->amount, X->character);
  huffman_tree_walk(X->left);
  huffman_tree_walk(X->right);
  /*  if(X!=NULL && X!=&S){
		huffman_tree_walk(X->left);
		printf("%d %c\n", X->amount, X->character);
		huffman_tree_walk(X->right);
	}*/
}

void printArr(int arr[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
    {
        printf("%d", arr[i]);
    }
    printf("\n");
}
void printCodes(struct node* root, int arr[], int top)
{
    if(root==NULL || root==&S) return;
    // Assign 0 to left edge and recur
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if ((root->left==NULL || root->left==&S)&&(root->right==NULL || root->right==&S))
    {
        printf("%c: ", root->character);
        printArr(arr, top);
        huffcharacters += top*root->amount;
    }
}


int main(int argc, char * argv[])
{
    root = &S;
    FILE * stream;
    if ((stream=fopen("input.txt", "r"))==NULL) {
     printf ("Nie mogê otworzyæ pliku test.txt do zapisu!\n");
     exit(1);
     }
    int amofchars=0; //amount of characters
    char znak;
    while((znak = fgetc(stream)) != EOF)
    {
        struct node *X = tree_search(root, znak);
        if(X==&S)
        {
            tree_insert(root, tmp_node(znak, 1), 1);
            amofchars++;
        }
        else
            X->amount++;
    }
    que_tree(root);
    struct node *X = Huffman();
    int arr[amofchars+1];
    string binary = bitset<8>(X->amount).to_string();
    cout<<binary<<endl;
    printCodes(X, arr, 0);
    double x = characters, y = huffcharacters;
    printf("ilosc znakow przed: %d ilosc po : %d\n kompresja do: %.2f\n", characters, huffcharacters, 1-(y/x));
    //huffman_tree_walk(X);
    fclose(stream);
    return 0;
}
