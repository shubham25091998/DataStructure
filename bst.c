#include <stdio.h>
#include <stdlib.h>

#define RECURSIVE 1
#define NORMAL 0

struct bst_node{
	
	struct bst_node* left;
	struct bst_node* right;
	struct bst_node* parent;
	
	int data;
};

struct bst{
	
	struct bst_node* root;
	long long int no_elements;
};

struct bst* bst_create();
void bst_insert(struct bst*,int);
void bst_preorder_traversal(struct bst*, int);
void bst_inorder_traversal(struct bst*, int);
void bst_postorder_traversal(struct bst*, int);
void bst_remove(struct bst*, int);
int bst_search(struct bst*, int);
void bst_destroy(struct bst**);

struct bst_node* helper_get_node(int);
void helper_preorder_rec(struct bst_node*);
void helper_inorder_rec(struct bst_node*);
void helper_postorder_rec(struct bst_node*);
struct bst_node* helper_search_data(struct bst*, int);

struct bst* bst_create(){
	struct bst* p_bst = (struct bst*)malloc(sizeof(struct bst));
	if(p_bst == NULL){
		perror("Memory allocation");
		exit(-1);
	}
	
	p_bst->root = NULL;
	p_bst->no_elements = 0;
	return p_bst;
}

void bst_insert(struct bst* p_bst, int value){
	
	struct bst_node* new_node = NULL;
	struct bst_node* run = NULL;
	if(p_bst == NULL){
		printf("Empty BST\n");
		return;
	}
	
	new_node = helper_get_node(value) ;
	
	if(new_node == NULL){
		return;
	}
	
	if(p_bst->root == NULL){
		
		p_bst->root = new_node;
	}else{
		run = p_bst->root;
		while(1){
			if(run->data >= value){
				if(run->left == NULL){
					run->left = new_node;
					new_node->parent = run;
					break;
				}else{
					run = run->left;
				}
			}else{
				if(run->right == NULL){
					run->right = new_node;
					new_node->parent = run;
					break;
				}else{
					run = run->right;
				}
			}
		}
	}
	p_bst->no_elements +=1;
}

struct bst_node* helper_get_node(int value){
	struct bst_node* new_node = (struct bst_node*)malloc(sizeof(struct bst_node));
	if(new_node == NULL){
		perror("Unable to allocate memory");
		return NULL;
	}
	
	new_node->data = value;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	
	return new_node;
}

void bst_preorder_traversal(struct bst* p_bst, int mode){
	if(p_bst == NULL){
		printf("Invalid BST\n");
		return;
	}
	
	if(mode == RECURSIVE){
		printf("Preorder traversal :  \n");
		printf("[START]->");
		helper_preorder_rec(p_bst->root);
		printf("[END]");
	}
}

void helper_preorder_rec(struct bst_node* p_node){
	
	if(p_node != NULL){
		printf("[%d]<->", p_node->data);
		helper_preorder_rec(p_node->left);
		helper_preorder_rec(p_node->right);
	}
}

void bst_inorder_traversal(struct bst* p_bst, int mode){
	if(p_bst == NULL){
		printf("Invalid BST\n");
		return;
	}
	
	if(mode == RECURSIVE){
		printf("\nInorder Traversal : \n");
		printf("[START]->");
		helper_inorder_rec(p_bst->root);
		printf("[END]\n");
	}
}

void helper_inorder_rec(struct bst_node* p_node){
	if(p_node != NULL){
		helper_inorder_rec(p_node->left);
		printf("[%d]<->", p_node->data);
		helper_inorder_rec(p_node->right);
	}
}

void bst_postorder_traversal(struct bst* p_bst, int mode){
	if(p_bst == NULL){
		printf("Invalid BST\n");
		return;
	}
	if(mode == RECURSIVE){
		printf("Postorder Traversal : \n");
		printf("[START]->");
		helper_postorder_rec(p_bst->root);
		printf("[END]\n");
	}	
}

void helper_postorder_rec(struct bst_node* p_node){
	if(p_node!= NULL){
		helper_postorder_rec(p_node->left);
		helper_postorder_rec(p_node->right);
		printf("[%d]<->",p_node->data);
	}
}

void bst_remove(struct bst* p_bst, int value){
	struct bst_node* delete_node = NULL;
	struct bst_node* replacement = NULL;
	
	if(p_bst == NULL){
		printf("Invalid BST\n");
		return;
	}
	
	delete_node = helper_search_data(p_bst,value);
	if(delete_node == NULL){
		printf("Data not found in BST\n");
		return;
	}
	
	if(delete_node->left == NULL){
		if(delete_node->parent == NULL){
			p_bst->root = delete_node->right;
		}else if(delete_node == delete_node->parent->left){
			delete_node->parent->left = delete_node->right;
		}else{
			delete_node->parent->right = delete_node->right;
		}
	}
	else if(delete_node->right == NULL){
		if(delete_node->parent == NULL){
		
			p_bst->root = delete_node->left;
		}else if(delete_node == delete_node->parent->left){
			delete_node->parent->left = delete_node->left;
		}else{
			delete_node->parent->right = delete_node->left;
		}
	}else{
		replacement = delete_node->right;
		while(replacement->left != NULL){
			replacement = replacement->left;
		}
		
		if(replacement != delete_node->right){
			replacement->parent->left = replacement->right;
			
			if(replacement->right != NULL)
				replacement->right->parent = replacement->parent;
			replacement->right = delete_node->right;
			replacement->right->parent = replacement;
		}
		
		if(delete_node->parent == NULL){
			p_bst->root = replacement;
		}else if(delete_node == delete_node->parent->left){
		
			delete_node->parent->left = replacement;
		}else{
			delete_node->parent->right = replacement;
		}
		
		replacement->parent = delete_node->parent;
		replacement->left = delete_node->left;
		
		if(replacement->left != NULL)
			replacement->left->parent = replacement;
	}
	
	p_bst->no_elements -=1;
	free(delete_node);
	delete_node = NULL;
	
}

struct bst_node* helper_search_data(struct bst* p_bst, int value){
	
	struct bst_node* run = p_bst->root;
	while(run != NULL){
		if(value == run->data){
			return run;
		}else if(value < run->data){
			run = run->left;
		}else{
			run = run->right;
		}
	}
	return run;
}

int bst_search(struct bst* p_bst, int value){
	
	if(p_bst == NULL){
		printf("Invalid BST\n");
		return -1;
	}
	
	if(helper_search_data(p_bst,value) != NULL){
		return 1;
	}
	return 0;
}

void bst_destroy(struct bst** pp_bst){
	
	struct bst_node* p_root = NULL;
	struct bst_node* p_run = NULL;
	struct bst_node* parent = NULL;
	if(pp_bst== NULL && *pp_bst == NULL){
		printf("Invalid BST\n");
		return;
	}
	p_root = (*pp_bst)->root;
	if(p_root==NULL){
		return;
	}
	
	p_run = p_root;
	
	while(1){
		if(p_root->left == NULL && p_root->right == NULL)
			break;
		if(p_run->left != NULL){
			p_run = p_run->left;
		}else if(p_run->right != NULL){
			p_run = p_run->right;
		}else{
			parent = p_run->parent;
			if(p_run == parent->left){
				parent->left = NULL;
			}else{
				parent->right = NULL;
			}
			free(p_run);
			p_run = parent;
			(*pp_bst)->no_elements -=1;
		}
	}
	free(p_root);
	free(*pp_bst);
	*pp_bst = NULL;
	
}
int main(){
	
	int data[] = {230, 1231, 12, 24, 4456, 34,324,23,32421,34,345,6,45,723,624,627,893,7235,352,832,323,334,456};
	struct bst* p_bst = NULL;
	
	p_bst = bst_create();
	
	for(int i = 0; i < sizeof(data)/sizeof(int); i++){
		bst_insert(p_bst,data[i]);
		
	}
	bst_preorder_traversal(p_bst, RECURSIVE);
	bst_inorder_traversal(p_bst, RECURSIVE);
	bst_postorder_traversal(p_bst,RECURSIVE);
	
	bst_remove(p_bst,345);
	if(bst_search(p_bst,345)==1){
		printf("Data exist\n");
	}else{
		printf("Data not exist\n");
	}
	
	bst_destroy(&p_bst);
}
