// THIS COMMENT SHOULD BE IN THE NEW BRANCH



#include<stdlib.h>
#include<stdio.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#define COUNT 10

// g++ -fprofile-arcs -ftest-coverage -g BinTree.c -o bin
// run bin.exe then "gcov BinTree.c"

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

node *findLCA(node* root, int n1, int n2)
{
    // Base case
    if (root == NULL) return NULL;

    // If either n1 or n2 matches with root's key, report
    // the presence by returning root (Note that if a key is
    // ancestor of other, then the ancestor key becomes LCA
    if (root->data == n1 || root->data == n2)
        return root;

    // Look for keys in left and right subtrees
    node *left_lca  = findLCA(root->left, n1, n2);
    node *right_lca = findLCA(root->right, n1, n2);

    // If both of the above calls return Non-NULL, then one key
    // is present in once subtree and other is present in other,
    // So this node is the LCA
    if (left_lca && right_lca)  return root;

    // Otherwise check if left subtree or right subtree is LCA
    return (left_lca != NULL)? left_lca: right_lca;
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

TEST_CASE( "Lowest Common ancestors are computed", "[findLCA]" ) {
    node *root;
    node *tmp;
    //int i;

    root = NULL;
    // test empty tree
    //REQUIRE(findLCA(root,1,17) == NULL);
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
    prettyPrint(root,0);
    REQUIRE(findLCA(root,1,17)->data == 3);
    REQUIRE(findLCA(root,12,17)->data == 15);
    REQUIRE(findLCA(root,12,6)->data == 9);
    REQUIRE(findLCA(root,3,3)->data == 3);
    REQUIRE(findLCA(root,1,17)->data == 3);
    REQUIRE(findLCA(root,1,9)->data == 3);
    REQUIRE(findLCA(root,9,1)->data == 3);
    REQUIRE(findLCA(root,15,4)->data == 9);
    REQUIRE(findLCA(root,1,1)->data == 1);
    deltree(root);
    // fail case
    // REQUIRE(findLCA(root,1,17)->data == 5);
}
