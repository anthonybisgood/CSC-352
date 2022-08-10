#ifndef __AUTOCOMPLETE_H__INCLUDED__
#define __AUTOCOMPLETE_H__INCLUDED__


#include <stdio.h>


typedef struct WordList WordList;
struct WordList
{
    int    count;
    int    alloc;
    char **words;
};



typedef struct LookupTreeNode LookupTreeNode;
struct LookupTreeNode
{
    /* YOU MUST IMPLEMENT THIS FIELD
     *
     * This field must be an array of 26 pointers.  It must be an actual
     * array of pointers, declared here (not a pointer-pointer).
     *
     * But here's the trick: you are going to mix the * and the [] in your
     * declaration.  Which has a higher precedence to the parser - are you
     * declaring an array of pointers, or a pointer to a single
     * fixed-length buffer?
     *
     * I can never remember.  I always run a testcase using sizeof() to
     * check, each time that I write a program that needs this sort of thing.
     * You should do the same!
     */
    LookupTreeNode *(children[26]);
    /* see the WordList type (with its associated functions) above. */
    WordList *result_words;
};



WordList *wl_create (void);
int       wl_add    (WordList *list, char *word);
void      wl_destroy(WordList *list);

LookupTreeNode *ltn_create(void);
void            ltn_add_result_word(LookupTreeNode *ltn, char *search_word, char *result_word);
void            ltn_destroy(LookupTreeNode *ltn);

int node_count  (LookupTreeNode *root);
int result_count(LookupTreeNode *root);

LookupTreeNode *lookup(LookupTreeNode *root, char *search);

WordList       *build_wordlist_from_file(FILE *fp);
LookupTreeNode *build_tree_from_words   (WordList *words);
void            print_words(LookupTreeNode *result, char *search);

char *toUpper(char *str);

#endif     // __AUTOCOMPLETE_H__INCLUDED__

