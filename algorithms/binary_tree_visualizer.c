#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node{
	int data;
	struct Node *left;
	struct Node *right;
} Node;

void push(Node **tree,int data){
	Node *new = (Node *)calloc(1, sizeof(Node));
	new->data=data;
	new->left = NULL;
	new->right = NULL;

	if(*tree == NULL){
		*tree=new;
		return;
	}
	Node *temp = *tree;
	while(temp != NULL){
		if(data<temp->data){
			if(temp->left == NULL){
				temp->left=new;
				break;
			}
			temp = temp->left;
		}
		else{
			if(temp->right == NULL){
				temp->right = new;
				break;
			}
			temp = temp->right;
		}
	}
}

void free_tree(Node *item){
	if(item == NULL) return;
	free_tree(item->left);
	free_tree(item->right);
	free(item);
}

int calculate_tree_depth(Node *item) {
    if (item == NULL) {
        return 0;
    }

    int left_depth = calculate_tree_depth(item->left);
    int right_depth = calculate_tree_depth(item->right);

    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

int calculate_tree_item(Node *item){
	if(item == NULL) return 0;
	int a = calculate_tree_item(item->left);
	int b = calculate_tree_item(item->right);
	return 1+a+b;
}

void tree_matrix_pusher(Node *item, int *push_counter, int tree_length, int depth,
						int left_index, int max_depth, int tree_array[max_depth][1 << (max_depth - 1)]){
	if(*push_counter == tree_length) return;
	if(item == NULL) return;
	tree_array[depth][left_index] = item->data;
	*push_counter = *push_counter+1;
	tree_matrix_pusher(item->left, push_counter, tree_length, depth+1,
						2*left_index, max_depth, tree_array);
	tree_matrix_pusher(item->right, push_counter, tree_length, depth+1,
						2*left_index+1, max_depth, tree_array);
	return;
}

void visualize_tree(Node *tree, int space_count){
	if(tree == NULL) return;
	int max_depth = calculate_tree_depth(tree);
	int tree_length = calculate_tree_item(tree);
	int max_col = 1 << (max_depth - 1);

	int tree_array[max_depth][max_col];
	memset(tree_array, -1, sizeof(tree_array));

	int push_counter = 0;
	int depth = 0;
	int left_index = 0;

	tree_matrix_pusher(tree, &push_counter, tree_length, depth, left_index, max_depth, tree_array);

	int max_space = space_count * 2;
	int x;

	for (int i = 0; i < max_depth; i++) {
        int leading_space = ((1 << (max_depth - i - 1)) * max_space) / (1 << max_depth);
        int between_space = ((1 << (max_depth - i)) * max_space) / (1 << max_depth) - 1;

        if (leading_space < 0) leading_space = 0;
        if (between_space < 0) between_space = 0;

        for (x = 0; x < leading_space; x++) printf(" ");

        int active_col = 1 << i;
        for (int j = 0; j < active_col; j++) {
            if (tree_array[i][j] == -1) printf("X");
            else printf("%d", tree_array[i][j]);
            if (j < active_col - 1) for (x = 0; x < between_space; x++) printf(" ");
        }
        printf("\n");
	}
}


int main(void){
	printf("\n");
	Node *tree = NULL;

	push(&tree, 4);
	push(&tree, 2);
	push(&tree, 1);
	push(&tree, 3);
	push(&tree, 6);
	push(&tree, 7);
	push(&tree, 5);

	visualize_tree(tree,30);

	printf("\n");
	free_tree(tree);
	return 0;
}
