// THIS COMMENT SHOULD BE IN THE NEW BRANCH
// for merge test

const int YO_STARTSIZE = 16;

#include<stdlib.h>
#include<stdio.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#define COUNT 10

// g++ -fprofile-arcs -ftest-coverage -g BinTree.c -o bin
// run bin.exe then "gcov BinTree.c"
struct yonode {
    char value;
    int cursize, maxsize;
    struct yonode **edges;
};

struct yowordlist {
    int cursize, maxsize;
    char **words;
};


struct bin_tree {
int data;
struct bin_tree * right, * left;
};
typedef struct bin_tree node;

struct yonode *yodawg_create()
{
    struct yonode *start, *eow;
    start = yodawg_create_node(0);
    if(start == NULL) return NULL;
    eow = yodawg_create_node(YO_EOW);
    if(eow == NULL) {
        yodawg_free_node(start);
        return NULL;
    }
    if(yodawg_add_node(start, eow)) {
        yodawg_free_node(eow);
        yodawg_free_node(start);
        return NULL;
    }
    return start;
}

struct yonode *yodawg_create_node(char value)
{
    struct yonode *node;
    node = malloc(sizeof (struct yonode));
    if(node == NULL) return NULL;
    node->cursize = 0;
    node->maxsize = YO_STARTSIZE;
    node->value = value;
    node->edges = malloc(YO_STARTSIZE * sizeof (struct yonode *));
    if(node->edges == NULL) {
        free(node);
        return NULL;
    }
    return node;
}

int yodawg_add_node(struct yonode *parent, struct yonode *child)
{
    if (parent->edges == NULL) {
        parent->edges = malloc(YO_STARTSIZE * sizeof (struct yonode *));
        if(parent->edges == NULL) return -1;
    }
    else if (parent->cursize == parent->maxsize) {
        parent->maxsize *= 2;
        parent->edges = realloc(parent->edges, parent->maxsize);
        if(parent->edges == NULL) return -1;
    }
    parent->edges[parent->cursize++] = child;
    return 0;
}

void insertIntoBinTree(node ** tree, int val)
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
        insertIntoBinTree(&(*tree)->left, val);
    }
    else if(val > (*tree)->data)
    {
        insertIntoBinTree(&(*tree)->right, val);
    }

}

node *findBinTreeLCA(node* root, int n1, int n2)
{
    // Base case
    if (root == NULL) return NULL;

    // If either n1 or n2 matches with root's key, report
    // the presence by returning root (Note that if a key is
    // ancestor of other, then the ancestor key becomes LCA
    if (root->data == n1 || root->data == n2)
        return root;

    // Look for keys in left and right subtrees
    node *left_lca  = findBinTreeLCA(root->left, n1, n2);
    node *right_lca = findBinTreeLCA(root->right, n1, n2);

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

void delBinTree(node * tree)
{
    if (tree)
    {
        delBinTree(tree->left);
        delBinTree(tree->right);
        free(tree);
    }
}

TEST_CASE( "Lowest Common ancestors are computed", "[findBinTreeLCA]" ) {
    node *root;
    node *tmp;
    //int i;

    root = NULL;
    // test empty tree
    /* insertIntoBinTreeing nodes into tree */
    insertIntoBinTree(&root, 3);
    insertIntoBinTree(&root, 9);
    insertIntoBinTree(&root, 4);
    insertIntoBinTree(&root, 15);
    insertIntoBinTree(&root, 6);
    insertIntoBinTree(&root, 12);
    insertIntoBinTree(&root, 17);
    insertIntoBinTree(&root, 2);
    insertIntoBinTree(&root, 1);
    prettyPrint(root,0);
    REQUIRE(findBinTreeLCA(root,1,17)->data == 3);
    REQUIRE(findBinTreeLCA(root,12,17)->data == 15);
    REQUIRE(findBinTreeLCA(root,12,6)->data == 9);
    REQUIRE(findBinTreeLCA(root,3,3)->data == 3);
    REQUIRE(findBinTreeLCA(root,1,17)->data == 3);
    REQUIRE(findBinTreeLCA(root,1,9)->data == 3);
    REQUIRE(findBinTreeLCA(root,9,1)->data == 3);
    REQUIRE(findBinTreeLCA(root,15,4)->data == 9);
    REQUIRE(findBinTreeLCA(root,1,1)->data == 1);
    delBinTree(root);
    // fail case
    // REQUIRE(findBinTreeLCA(root,1,17)->data == 5);
}
