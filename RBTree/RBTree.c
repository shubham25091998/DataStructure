#include <stdio.h>
#include<stdlib.h>

#define RECURSIVE	1
#define NORMAL		0

enum Color{
	RED = 0,
	BLACK
};
struct rbnode{
	
	struct rbnode* left;
	struct rbnode* right;
	struct rbnode* parent;
	enum Color color;
	int data;	
};

struct rbtree{
	
	struct rbnode* root;
	struct rbnode* nil;
	long long int no_elements;
};

struct rbtree* rbtree_create();
void rbtree_insert(struct rbtree*, int);
void rbtree_preorder(struct rbtree*, int);

struct rbnode* helper_get_rbnode(struct rbnode*,int);
void helper_insert_fix(struct rbtree*, struct rbnode*);
void helper_right_rotate(struct rbtree*,struct rbnode*);
void helper_left_rotate(struct rbtree*,struct rbnode*);
void helper_preorder_rec(struct rbnode*,struct rbnode*);

struct rbtree* rbtree_create(){
	
	struct rbtree* p_rbtree = (struct rbtree*) calloc(1, sizeof(struct rbtree));
	if(p_rbtree == NULL){
		printf("Unable to get memory for tree\n");
		exit(-1);
	}
	
	p_rbtree->nil = (struct rbnode*)calloc(1,sizeof(struct rbnode));
	if(p_rbtree->nil == NULL){
		printf("Unable to get memory for tree\n");
		exit(-1);
	}
	
	p_rbtree->nil->left = NULL;
	p_rbtree->nil->right = NULL;
	p_rbtree->nil->parent = NULL;
	p_rbtree->nil->color = BLACK;
	p_rbtree->nil->data = -1;
	p_rbtree->root = p_rbtree->nil;
	p_rbtree->no_elements = 0;
	return p_rbtree;
}

void rbtree_insert(struct rbtree* p_rbtree, int data){
	
	struct rbnode* new_node =NULL;
	struct rbnode* run = NULL;
	if(p_rbtree == NULL){
		printf("Invalid RBTree\n");
		return;
	}
	new_node =  helper_get_rbnode(p_rbtree->nil, data);
	if(new_node == NULL){
		exit(-1);
	}
	
	if(p_rbtree->root == p_rbtree->nil){
		p_rbtree->root = new_node;
	}else{
		run = p_rbtree->root;
		while(1){
			if(data <= run->data){
			
				if(run->left == p_rbtree->nil){
					run->left = new_node;
					new_node->parent = run;
					break;
				}
				run = run->left;
			}else{
				if(run->right == p_rbtree->nil){
					run->right = new_node;
					new_node->parent = run;
					break;
				}
				run = run-> right;
			}
		}
	}
	p_rbtree->no_elements +=1;
	helper_insert_fix(p_rbtree, new_node);
}

struct rbnode* helper_get_rbnode(struct rbnode* nil, int data){
	
	struct rbnode* node = (struct rbnode*) calloc(1,sizeof(struct rbnode));
	if(node == NULL){
		printf("Unable to allocate memory\n");
		return node;
	}
	node->left = nil;
	node->right = nil;
	node->parent = nil;
	node->color = RED;
	node->data = data;
	return node;	
}

void helper_insert_fix(struct rbtree* p_rbtree, struct rbnode* node){
	struct rbnode* sibling = NULL;
	
	while(node->parent->color == RED){

		if(node->parent == node->parent->parent->left){
		
			sibling = node->parent->parent->right;
			
			// If sibling is red that means, parent is black
			if(sibling->color == RED){
				node->parent->color = BLACK;
				sibling->color = BLACK;
				node->parent->parent->color = RED;
				
				// Move to parent i.e. upward to tree to check any unstability 
				node = node->parent->parent;
			}else{
				if(node == node->parent->right){
					node = node->parent;
					helper_left_rotate(p_rbtree, node);
				}
				
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				
				helper_right_rotate(p_rbtree, node->parent->parent);
			}
		}else{
			sibling = node->parent->parent->left;
			
			if(sibling->color == RED){
				node->parent->color = BLACK;
				sibling->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
			}else{
				
				if(node == node->parent->left){
					node = node->parent;
					helper_right_rotate(p_rbtree, node);
				}
				
				node->parent->color = BLACK;
				node->parent->parent->color = RED;
				helper_left_rotate(p_rbtree, node->parent->parent);
			}
		}
	}
	p_rbtree->root->color = BLACK;
}

void helper_right_rotate(struct rbtree* p_rbtree,struct rbnode* p_node){
	struct rbnode* child = NULL;
	
	child = p_node->left;
	p_node->left = child->right;
	
	if(p_node->left != p_rbtree->nil)
		p_node->left->parent = p_node;
	
	child->parent = p_node->parent;
	if(p_node->parent == p_rbtree->nil){
		p_rbtree->root = child;
	}else if(p_node == p_node->parent->left){
		p_node->parent->left = child;
	}else{
		p_node->parent->right = child;
	}
	
	child->right = p_node;
	p_node->parent = child;
}

void helper_left_rotate(struct rbtree* p_rbtree,struct rbnode* p_node){
	
	struct rbnode* child = NULL;
	
	child = p_node->right;
	p_node->right = child->left;
	
	if(p_node->right != p_rbtree->nil)
		p_node->right->parent = p_node;
	
	child->parent = p_node->parent;
	if(p_node->parent == p_rbtree->nil){
		p_rbtree->root = child;
	}else if(p_node == p_node->parent->left){
		p_node->parent->left = child;
	}else{
		p_node->parent->right = child;
	}
	
	child->left = p_node;
	p_node->parent = child;
}

void rbtree_preorder(struct rbtree* p_rbtree, int mode){
	
	if(p_rbtree == NULL || p_rbtree->root == NULL){
		printf("Invalid RBTree\n");
	}
	if(mode == RECURSIVE){
		printf("Preorder traversal :\n[START]->");
		helper_preorder_rec(p_rbtree->root, p_rbtree->nil);
		printf("[END]\n");
	}
}

void helper_preorder_rec(struct rbnode* node, struct rbnode* nil){
	
	if(node != nil){
		printf("[%d]<->", node->data);
		helper_preorder_rec(node->left, nil);
		helper_preorder_rec(node->right,nil);
	}
}
int main(){
	struct rbtree* p_rbtree = NULL;
	int data[] = {100,120,140,160,130,90,70,40,30,80};
	p_rbtree = rbtree_create();
	
	for(int i = 0; i<10; i++){
		rbtree_insert(p_rbtree, data[i]);
	}
	
	rbtree_preorder(p_rbtree, RECURSIVE);
}
