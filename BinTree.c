
const int STARTSIZE = 16;
#include<stdlib.h>
#include<stdio.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#include "catch.hpp"
#define COUNT 10
#include "dawg.h"

// for testing
// g++ -fprofile-arcs -ftest-coverage -g BinTree.c -o bin
// run bin.exe then "gcov BinTree.c"

struct bin_tree {
  int data;
  struct bin_tree * right, * left;
};
typedef struct bin_tree node;

struct node *dawg_create()
{
  struct node *start, *eow;
  start = dawg_create_node(0);
  if(start == NULL) return NULL;
  eow = dawg_create_node(EOW);
  if(eow == NULL) {
    dawg_free_node(start);
    return NULL;
  }
  if(dawg_add_node(start, eow)) {
    dawg_free_node(eow);
    dawg_free_node(start);
    return NULL;
  }
  return start;
}

struct node *dawg_create_node(char value)
{
  struct node *node;
  node = malloc(sizeof (struct node));
  if(node == NULL) return NULL;
  node->cursize = 0;
  node->maxsize = STARTSIZE;
  node->value = value;
  node->edges = malloc(STARTSIZE * sizeof (struct node *));
  if(node->edges == NULL) {
    free(node);
    return NULL;
  }
  return node;
}

int dawg_add_node(struct node *parent, struct node *child)
{
  if (parent->edges == NULL) {
    parent->edges = malloc(STARTSIZE * sizeof (struct node *));
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

int dawg_value_in_node(struct node *node, char value)
{
  int i;
  for(i = 0; i < node->cursize; i++)
  if (value == node->edges[i]->value)
  return i;
  return -1;
}

int dawg_add_string(struct node *dawg, char *str)
{
  char c;
  struct node *cur, *buf;
  int i;

  cur = dawg;
  while((c = *str++) != '\0') {
    i = dawg_value_in_node(cur, c);
    if(i < 0) {
      buf = dawg_create_node(c);
      if(dawg_add_node(cur, buf)) return -1;
      cur = buf;
    }
    else {
      cur = cur->edges[i];
    }
  }
  if(dawg_value_in_node(cur, EOW) < 0) {
    if(dawg_add_node(cur, dawg->edges[0])) return -1;
  }
  return 0;
}

int dawg_string_in_dawg(struct node *dawg, char *str)
{
  char c;
  int i;
  struct node *cur;

  cur = dawg;
  while((c = *str++) != '\0') {
    i = dawg_value_in_node(cur, c);
    if(i < 0) return 0;
    cur = cur->edges[i];
  }
  if(dawg_value_in_node(cur, EOW) < 0) return 0;
  return 1;
}

void dawg_free_node(struct node *node)
{
  free(node->edges);
  free(node);
}

struct node **dawg_add_dawg_to_list(struct node **nodes, int *cursize, int *maxsize, struct node *dawg)
{
  int i, j, found;
  struct node *cur;

  for(i = 0; i < dawg->cursize; i++) {
    cur = dawg->edges[i];
    j = 0;
    found = 0;
    while(j < (*cursize)) {
      if(nodes[j] == cur) {
        found = 1;
        break;
      }
      j++;
    }
    if(!found) {
      while((*cursize) >= (*maxsize)) {
        (*maxsize) *= 2;
        nodes = realloc(nodes, (*maxsize) * sizeof (struct node *));
        if(nodes == NULL) return NULL;
      }
      nodes[*cursize] = cur;
      ++*cursize;
    }
    if(cur->cursize > 0)
    nodes = dawg_add_dawg_to_list(nodes, cursize, maxsize, cur);
  }
  return nodes;
}

int dawg_free_dawg(struct node *dawg)
{
  int *cursize;
  int *maxsize;
  int _cursize = 0;
  int _maxsize = STARTSIZE;
  int i;
  struct node **nodes;

  cursize = &_cursize;
  maxsize = &_maxsize;
  nodes = malloc(*maxsize * sizeof (struct node *));
  if(nodes == NULL) return -1;

  nodes = dawg_add_dawg_to_list(nodes, cursize, maxsize, dawg);
  if(nodes == NULL) return -1;

  // Free the children.
  for(i = 0; i < *cursize; i++) {
    dawg_free_node(nodes[i]);
  }
  // Free the root.
  dawg_free_node(dawg);

  free(nodes);
  return 0;
}

int dawg_build_strings(struct node *dawg, const char *prefix, struct wordlist *strings)
{
  struct node *cur;
  int i;
  int plen = strlen(prefix);
  char *newprefix;
  newprefix = malloc(plen + 2);
  if(newprefix == NULL) return -1;

  for(i = 0; i < dawg->cursize; i++) {
    cur = dawg->edges[i];
    if(cur->value == EOW) {
      // Allocate enough memory.
      if(strings->cursize >= strings->maxsize) {
        strings->maxsize *= 2;
        strings->words = realloc(strings->words, strings->maxsize * sizeof (char *));
        if(strings->words == NULL) return -1;
      }
      strings->words[strings->cursize] = malloc(plen + 1);
      if(strings->words[strings->cursize] == NULL) return -1;
      strcpy(strings->words[strings->cursize], prefix);
      strings->cursize++;
    }
    else {
      strcpy(newprefix, prefix);
      strncat(newprefix, &(cur->value), 1);
      if(dawg_build_strings(cur, newprefix, strings)) return -1;
    }
  }
  free(newprefix);
  return 0;
}

struct wordlist *dawg_create_wordlist(int size)
{
  struct wordlist *wordlist;
  wordlist = malloc(sizeof (struct wordlist));
  if(wordlist == NULL) return NULL;
  wordlist->cursize = 0;
  wordlist->maxsize = STARTSIZE;
  wordlist->words = malloc(STARTSIZE * sizeof (char *));
  if(wordlist->words == NULL) {
    free(wordlist);
    return NULL;
  }
  return wordlist;
}

void dawg_free_wordlist(struct wordlist *wordlist)
{
  int i;
  if(wordlist == NULL) return;
  for(i = 0; i < wordlist->cursize; i++)
  free(wordlist->words[i]);
  free(wordlist->words);
  free(wordlist);
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

//TEST_CASE( "Lowest Common ancestors are computed", "[findBinTreeLCA]" ) {
int main(int argc, char **argv){

  struct node *dawg;
  int j;
  struct wordlist *strings;

  dawg = dawg_create();

  dawg_add_string(dawg, "foo0");
  dawg_add_string(dawg, "bar");
  dawg_add_string(dawg, "baz");
  dawg_add_string(dawg, "bark");
  dawg_add_string(dawg, "bars");
  dawg_add_string(dawg, "barf");
  dawg_add_string(dawg, "quxxxx");
  dawg_add_string(dawg, "bbq");
  //printf(" Hi there: %s\n",dawg -> value);
  /*if(strings->cursize < 0)
  printf("Prefix not in dawg.\n");
  else
  printf("Found %d matching strings:\n", strings->cursize);

  dawg_free_dawg(dawg);


  dawg_free_wordlist(strings);
*/
  node *root;
  node *tmp;

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
  delBinTree(root);
  return 0;
}
