#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef DAWG
#define DAWG

#ifndef STARTSIZE
#define STARTSIZE 16
#endif

#ifndef EOW
#define EOW -1
#endif

struct node {
    char value;
    int cursize, maxsize;
    struct node **edges;
};

struct wordlist {
    int cursize, maxsize;
    char **words;
};

struct node *dawg_create();
struct node *dawg_create_node(char);
int dawg_add_node(struct node *, struct node *);
int dawg_value_in_node(struct node *, char);
int dawg_add_string(struct node *, char *);
int dawg_string_in_dawg(struct node *, char *);
void dawg_free_node(struct node *);
struct node **dawg_add_dawg_to_list(struct node **, int *, int *, struct node *);
int dawg_free_dawg(struct node *);
int dawg_build_strings(struct node *, const char *, struct wordlist *);
struct wordlist *dawg_create_wordlist(int);
void dawg_free_wordlist(struct wordlist *);
struct wordlist *dawg_find_strings(struct node *, char *);
#endif
