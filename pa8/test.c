#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "autocomplete.h"

int main(int argc, char **argv) {
    //LookupTreeNode *res = ltn_create();
    // printf("%ld\n", sizeof(res));
    // printf("%ld\n", sizeof(LookupTreeNode));
    // printf("%ld\n", sizeof(res -> children));

    // char* toAdd = "CATCH";
    // WordList *newList = wl_create();
    // wl_add(newList, toAdd);
    // // wl_add(newList, "MATCH");
    // LookupTreeNode *ret = build_tree_from_words(newList);
    // printf("%d\n", node_count(ret));
    // printf("%d\n", result_count(ret));


    FILE *file = fopen("temp.txt", "r");
    WordList *list = build_wordlist_from_file(file);
    LookupTreeNode *FileList = build_tree_from_words(list);

    //print_words(FileList, "BROUILLARD");
    LookupTreeNode *lu = lookup(FileList, "BROUILLARD");
    print_words(lu, "BROUILLARD");

    // if (lu != NULL) {
    //     WordList *wl = lu -> result_words;
    //     printf("%s", wl -> words[0]);
    // }
   

    // printf("\n");
    // printf("%d\n", node_count(FileList)); 
    // printf("%d\n", result_count(FileList)); 

    
    // for (int i = 0; i < strlen(toAdd); i+=1) {
    //     ltn_add_result_word(res, toAdd + i, "");
    //     printf("%d\n", result_count(res)); 
    //     printf("%d\n", node_count(res)); 
    // }
    // LookupTreeNode **children = res -> children;
    // LookupTreeNode *C = children[2];
    // LookupTreeNode **Cchildren = C -> children;
    // LookupTreeNode *A = children[0];
    // LookupTreeNode **Achildren = A -> children;
    // if (Cchildren[0] != NULL) {printf("A");}
    // if (Achildren[19] != NULL){printf("T");}
    // // printf("%c\n", res-);
    // // printf("%c\n", res->children[2]);
    // // printf("%c\n", res->children[7]);
    // printf("%d\n", node_count(res));   
}