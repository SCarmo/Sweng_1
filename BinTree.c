#include<stdlib.h>
#include<stdio.h>
#define COUNT 69

struct bin_tree {
int data;
struct bin_tree * right, * left;
};
typedef struct bin_tree node;

void insert(node ** tree, int val)
{
    node *temp = NULL;
    if(!(*tree))
    {
        temp = (node *)malloc(sizeof(node));
        temp->left = temp->right = NULL;
        temp->data = val;
        *tree = temp;
        return;
    }

    if(val < (*tree)->data)
    {
        insert(&(*tree)->left, val);
    }
    else if(val > (*tree)->data)
    {
        insert(&(*tree)->right, val);
    }

}

void prettyPrint(node * tree, int space){
    if(tree == NULL)
      return;
    space += COUNT;

    prettyPrint(tree->right, space);

    // Print current node after space
    // count
    printf("\n");
    int i;
    for (i = COUNT; i < space; i++)
        printf(" ");
    printf("%d\n", tree->data);

    // Process left child
    prettyPrint(tree->left, space);
    // if(tree -> data == NULL)
    //   return;
    // if(tree->right->data != NULL)
    //     prettyPrint(tree->right, l+1);
    // else{
    //   int i;
    //   for(i = 0; i<l; i++)
    //     printf(" ");
    // }
    // else if(tree->data)

}

void deltree(node * tree)
{
    if (tree)
    {
        deltree(tree->left);
        deltree(tree->right);
        free(tree);
    }
}

node* search(node ** tree, int val)
{
    if(!(*tree))
    {
        return NULL;
    }

    if(val < (*tree)->data)
    {
        search(&((*tree)->left), val);
    }
    else if(val > (*tree)->data)
    {
        search(&((*tree)->right), val);
    }
    else if(val == (*tree)->data)
    {
        return *tree;
    }
}

void main()
{
    node *root;
    node *tmp;
    //int i;

    root = NULL;
    /* Inserting nodes into tree */
    insert(&root, 3);
    insert(&root, 9);
    insert(&root, 4);
    insert(&root, 15);
    insert(&root, 6);
    insert(&root, 12);
    insert(&root, 17);
    insert(&root, 2);
    insert(&root, 1);
    prettyPrint(root, 0);
    /* Deleting all nodes of tree */
    //deltree(root);
}
