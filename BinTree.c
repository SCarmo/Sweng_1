
const int STARTSIZE = 16;
#include<stdlib.h>
#include<stdio.h>
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#define COUNT 10
#include "dawg.h"

// for testing
// g++ -fprofile-arcs -ftest-coverage -g BinTree.c -o bin
// run bin.exe then "gcov BinTree.c"

struct node *dawg_create()
{
  struct node *start, *eow;
  start = dawg_create_node(0);
  if(start == NULL) return NULL;
  eow = dawg_create_node(EOW);

  // this section of code doesnt run but I can't pass tests without it :/
  //----------------------------------------------------------------------------
  if(eow == NULL) {
    dawg_free_node(start);
    return NULL;
  }
  if(dawg_add_node(start, eow)) {
    dawg_free_node(eow);
    dawg_free_node(start);
    return NULL;
  }
  //----------------------------------------------------------------------------
  return start;
}

struct node *dawg_create_node(char value)
{
  struct node *n;
  n = (node*)malloc(sizeof (struct node));
  if(n == NULL) return NULL;
  n->cursize = 0;
  n->maxsize = STARTSIZE;
  n->value = value;
  n->edges = (node**) malloc(STARTSIZE * sizeof (struct node *));
  if(n->edges == NULL) {
    free(n);
    return NULL;
  }
  return n;
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


int dawg_add_node(struct node *parent, struct node *child)
{
  if (parent->edges == NULL) {
    parent->edges = (node**)malloc(STARTSIZE * sizeof (struct node *));
    if(parent->edges == NULL) return -1;
  }
  else if (parent->cursize == parent->maxsize) {
    parent->maxsize *= 2;
    parent->edges = (node**)realloc(parent->edges, parent->maxsize);
    if(parent->edges == NULL) return -1;
  }
  parent->edges[parent->cursize++] = child;
  return 0;
}

int dawg_value_in_node(struct node *n, char value)
{
  int i;
  for(i = 0; i < n->cursize; i++)
  if (value == n->edges[i]->value)
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


void dawg_free_node(struct node *n)
{
  free(n->edges);
  free(n);
}



TEST_CASE( "Lowest Common ancestors are computed", "[findBinTreeLCA]" ) {
//int main(int argc, char **argv){

  // dawg testing
  struct node *dawg;
  int j;
  struct wordlist *strings;

  dawg = dawg_create();

  char bar[] = "bar";
  char bark[] = "bark";
  char baz[] = "baz";
  char foo0[] = "foo0";
  char Stephen[] = "Stephen";
  char bbq[] = "bbq";
  char quxxxx[] = "quxxxx";
  char barf[] = "barf";
  char bars[] = "bars";

  dawg_add_string(dawg, foo0);
  dawg_add_string(dawg, bar);
  dawg_add_string(dawg, baz);
  dawg_add_string(dawg, bark);
  dawg_add_string(dawg, bars);
  dawg_add_string(dawg, barf);
  dawg_add_string(dawg, quxxxx);
  dawg_add_string(dawg, bbq);
  REQUIRE(dawg_string_in_dawg(dawg, baz) == 1);
  REQUIRE(dawg_string_in_dawg(dawg, bbq) == 1);
  REQUIRE(dawg_string_in_dawg(dawg, quxxxx) == 1);
  REQUIRE(dawg_string_in_dawg(dawg, Stephen) == 0);
  //return 0;
}
