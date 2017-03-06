#include<stdio.h>
#include<stdlib.h>

struct node{
	int key;
    char color;
    struct node *up;
    struct node *left;
    struct node *right;
}*root, S;

void setup_S()
{
	S.color = 'B';
	S.left = &S;
	S.right = &S;
	S.up = &S;
}

void in_order_tree_walk(struct node *X){
	if(X!=&S){
		in_order_tree_walk(X->left);
		printf("%d %c\n", X->key, X->color);
		in_order_tree_walk(X->right);
	}
}

struct node* tree_search(struct node *X, int k){
	if(X==&S || k==X->key)
		return X;
	if(k<X->key)
		return tree_search(X->left, k);
	else
		return tree_search(X->right, k);
}

struct node* tmp_node(int k){
	struct node *tmp = (struct node*)malloc(sizeof *root);
	tmp->up = &S;
    tmp->left = &S;
    tmp->right = &S;
    tmp->key = k;
    tmp->color = 'R';
    return tmp;
}

void tree_insert(struct node *T, struct node *Z){
	struct node *Y = &S;
	struct node *X = root;
	while(X != &S)
	{
	    Y = X;
	    if(Z->key < X->key) X=X->left;
	    else X=X->right;
    }
    Z->up = Y;
    if( Y == &S)
    root = Z;
    else if(Z->key < Y->key) Y->left = Z;
    else Y->right = Z;
        
    
}

void left_rotate(struct node *X){
	struct node *Y = X->right;
	X->right = Y->left;
	if(Y->left != &S)
		Y->left->up = X;
	Y->up = X->up;
	if(X->up == &S)
		root = Y;
	else if( X == X->up->left)
		X->up->left = Y;
		else X->up->right = Y;
	Y->left = X;
	X->up = Y;
}

void right_rotate(struct node *X){
	struct node *Y = X->left;
	X->left = Y->right;
	if(Y->right != &S)
		Y->right->up = X;
	Y->up = X->up;
	if(X->up == &S)
		root = Y;
	else if( X == X->up->left)
		X->up->left = Y;
	else X->up->right = Y;
	Y->right = X;
	X->up = Y;
}

void RB_Insert(struct node *T, int k){
	struct node *X = tmp_node(k);
	tree_insert(T, X);
	X->color = 'R';
	while(X != root && X->up->color == 'R'){
		if(X->up == X->up->up->left){
			struct node *Y = X->up->up->right;
			if(Y != &S && Y->color == 'R'){
				X->up->color = 'B';
				Y->color = 'B';
				X->up->up->color = 'R';
				X = X->up->up;
			}
			else{
				if (X == X->up->right){
				X = X->up;
				left_rotate(X);
				
				}
				X->up->color = 'B';
				X->up->up->color = 'R';
				right_rotate(X->up->up);
			}
		}
		else{
			struct node *Y = X->up->up->left;
			if(Y != &S && Y->color == 'R'){
				X->up->color = 'B';
				Y->color = 'B';
				X->up->up->color = 'R';
				X = X->up->up;
			}
			else{
				if (X == X->up->left){
				X = X->up;
				right_rotate(X);
				}
				X->up->color = 'B';
				X->up->up->color = 'R';
				left_rotate(X->up->up);
			}
		}
	}
	root->color = 'B';
}

struct node* tree_minimum(struct node *X){
	while(X->left != &S)
	X = X->left;
	return X;
}

struct node* tree_successor(struct node *X){
	if(X->right != &S)
	return tree_minimum(X->right);
	struct node *Y = X->up;
	while(Y!= &S && X == Y->right){
		X = Y;
		Y = Y->up;
	}
	return Y;
}

void RB_Delete_Fixup(struct node *T, struct node *X){
	while(X != root && X->color == 'B'){
		if(X==X->up->left){
			struct node *W = X->up->right;
			if(W->color == 'R'){
				W->color = 'B';
				X->up->color = 'R';
				left_rotate(X->up);
				W = X->up->right;
			}
			if(W->left->color == 'B' && W->right->color == 'B'){
				W->color = 'R';
				X=X->up;
			}
			else if(W->right->color == 'B'){
				W->left->color = 'B';
				W->color = 'R';
				right_rotate(W);
				W=X->up->right;
			}
			W->color = W->up->color;
			X->up->color = 'B';
			W->right->color = 'B';
			left_rotate(X->up);
			X=root;
		}
		else{
			struct node *W = X->up->left;
			if(W->color == 'R'){
				W->color = 'B';
				X->up->color = 'R';
				right_rotate(X->up);
				W = X->up->left;
			}
			if(W->right->color == 'B' && W->left->color == 'B'){
				W->color = 'R';
				X=X->up;
			}
			else if(W->left->color == 'B'){
				W->right->color = 'B';
				W->color = 'R';
				left_rotate(W);
				W=X->up->left;
			}
			W->color = W->up->color;
			X->up->color = 'B';
			W->left->color = 'B';
			right_rotate(X->up);
			X=root;
		}
	}
X->color = 'B';
}

struct node* RB_Delete(struct node *T, struct node *Z){
	struct node *Y, *X;
	if(Z->left == &S && Z->right == &S) Y = Z;
	else Y = tree_successor(Z);
	if(Y->left != &S)  X=Y->left;
	else X=Y->right;
	X->up = Y->up;
	if(Y->up == &S) root = X;
	else if (Y == Y->up->left) Y->up->left = X;
	else Y->up->right = X;
	if(Y!=Z) Z->key = Y->key;
	if(Y->color == 'B') RB_Delete_Fixup(T,X);
	return Y;
}


int main(){
	
	root = &S; 
	int T[] = {5,26,17,8,9,30,10,1,23};
	int i;
	for(i=0; i<9; i++){
		printf("%d ", T[i]);
		RB_Insert(root, T[i]);
	}
	printf("\n");
	in_order_tree_walk(root);
	printf("usuwam wartosc 17 z drzewa\n");
	RB_Delete(root, tree_search(root, 17));
	in_order_tree_walk(root);
	
	return 0;
}
