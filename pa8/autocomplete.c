/**
 * @file autocomplete.c
 * @author Anthony Bisgood (anthonybisgood@email.arizona.edu)
 * @brief The following code performs some actions pertaining to an autocompletion program.
 * When a file is read the contents are turned into a WordList that will be used to create a 
 * tree that can be used to autocomplete a word that the user inputs. The program uses a WordList
 * struct to store pointers to words that have been allocated in the heap as well as the size of the 
 * array and the allocation size. 
 * @version 0.1
 * @date 2022-03-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "autocomplete.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/**
 * @brief Creates a new WordList Object and returns a pointer to it. callocs the size
 * of the WordList, as well as the **words field in it.
 * 
 * @return WordList* A new WordList pointer in free'd memory.
 */
WordList *wl_create(void) {
    // pointer to an array of char* pointers
    char **wordList = calloc(4, sizeof(char*));
    WordList *wl = calloc(1, sizeof(WordList));
    if (wordList == NULL || wl == NULL) {printf("Failed to allocate memory"); exit(1);}
    wl -> words = wordList;
    wl -> count = 0;
    wl -> alloc = 4;
    return wl;
}

/**
 * @brief Frees the list object as well as the **words list inside said object.
 * 
 * @param list the WordList struct to destroy
 */
void wl_destroy(WordList *list) {
    free(list -> words);
    free(list);
}

/**
 * @brief This function adds a new word to the words field in the list. 
 * If the words field is full, it will double the field and add the word 
 * to the end of the list.
 * 
 * @param list The WordList we are trying to add to 
 * @param word The word we are adding to wordList
 * @return int 0 if successful, -1 otherwise
 */
int wl_add(WordList *list, char *word) {
    /*  if the number of words in the list is equal to the word allocation of the list
        double the size of the alloc field
    */
    int listCount = list -> count;
    int allocCount = list -> alloc;
    if (listCount == allocCount - 1) {
        // doubles the alloc field
        list -> alloc = allocCount * 2;
        // sets word pointer to new reallocated array of size alloc * 2 * size of char *
        char **newList = realloc(list -> words, allocCount * 2 * sizeof(char*));
        if (newList == NULL) {printf("Failed to allocate memory"); return -1;}
        list -> words = newList;
    }
    // adds the new word to the index count in words
    list -> words[listCount] = word;
    list -> count += 1;
    return 0;
}

/**
 * @brief Builds a WordList based on file inputs given. iterates through a file,
 * cleans up the file, turning upper to lower case, removing all \n and doesnt
 * add any words that are not alphabetic. After, calls calloc with the word size, 
 * then copies the buffer into callocSpace. Calls wl_add with list and callocSpace.
 * 
 * @param fp The file pointer we are trying to read
 * @return WordList* The WordList containing all the valid words in fp.
 */
WordList *build_wordlist_from_file(FILE *fp) {
    char buffer[256];
    WordList *list = wl_create();
    while (fgets(buffer, 256, fp)) {
        int i = 0;
        // cleanup code, removes \n and \0
        while (buffer[i] != '\0') {
            if (buffer[i] == '\n') {buffer[i] = '\0'; i += 1; break;}
            if (buffer[i] >= 'a' && buffer[i] <= 'z') {
                buffer[i] -= 32;
            } else if (buffer[i] < 65 || buffer[i] > 90) {
                buffer[i] = '\0';
            }
            i += 1;
        }
        buffer[i] = '\0';
        // sets aside space in memory and copies the buffer into it
        char* callocSpace = calloc(1, i * sizeof(char));
        int j = 0;
        while (buffer[j] != '\0') {
            callocSpace[j] = buffer[j];
            j += 1;
        }
        wl_add(list, callocSpace);
    }
    fclose(fp);
    return list;
}

/**
 * @brief This method creates a new LookupTree and saves the tree using calloc.
 * 
 * @return LookupTreeNode* the new lookup tree.
 */
LookupTreeNode *ltn_create(void) {
    LookupTreeNode *treeNode = calloc(1, sizeof(LookupTreeNode));
    if (treeNode == NULL) {return NULL;}
    treeNode -> result_words = wl_create();
    return treeNode;
}

/**
 * @brief Frees the memory allocated by list. calls Free on the fields within
 * list and also frees any nodes that are connected to list recursively
 * 
 * @param list the Node to be destroyed
 */
void ltn_destroy(LookupTreeNode *list) {
    if (list == NULL) {return;}
    LookupTreeNode **childArray = list -> children;
    for (int i = 0; i < 26; i++){
        if (childArray[i] != NULL) {
            ltn_destroy(childArray[i]);
        }
    }
    WordList *wordList = list -> result_words;
    wl_destroy(wordList);
    free(list);
}

/**
 * @brief Adds the each character of the search_word to the ltn. Then, for each character
 * after the character that was just added, it recursevly adds any character after that and so on
 * until each branch of the recursive tree is reached(when searchword is '/0').
 * where it then adds the result word into the last ltn that was passed in.
 * When recursively adding characters(nodes), if the next character of the search word is
 * in nodes children, then it goes down that path. If the next character isnt in the children,
 * then it creates a new node and recurses until it reaches the end of search_word.
 * 
 * @param ltn the LookupTreeNode we are adding nodes to 
 * @param search_word The "path" we are taking to get to that node
 * @param result_word The word we put at the end of th path
 */
void ltn_add_result_word(LookupTreeNode *ltn, char *search_word, char *result_word) {
    if (search_word[0] == '\0' || search_word[0] == '\n') {
        if (search_word[0] == '\n') {search_word[0] = '\0';}
        if (result_word != NULL) {
            WordList *wordList = ltn -> result_words;
            wl_add(wordList, result_word);
        }
    } else {
        // gets the first letter of search_word and converts it to 0-25 based on ascii code
        int toFindDec = search_word[0] - 65;
        LookupTreeNode **childArray = ltn -> children;
        // removes first letter from search word
        char *newSearchWord = search_word + 1;
        // else go into the lookuptree at the index of the letter we have
        if (childArray[toFindDec] != NULL) {
            ltn_add_result_word(childArray[toFindDec], newSearchWord, result_word);
        // if the node isnt in the lookuptree
        } else {
            LookupTreeNode *newNode = ltn_create();
            if (newNode == NULL) {return;}
            // sets the old array value that used to be NULL to our newNode
            childArray[toFindDec] = newNode;
            // recurses with that new node, and search shortened by 1 letter
            ltn_add_result_word(newNode, newSearchWord, result_word);
        }
    }
}

/**
 * @brief Counts the number of nodes and subnodes recusively starting from root. If root is NULL, returns
 * NULL
 * 
 * @param root the root that we start counting at
 * @return int the number of noes in the tree
 */
int node_count(LookupTreeNode *root) {
    if (root == NULL) {return 0;}
    LookupTreeNode **childArray = root -> children;
    int count = 1;
    for (int i = 0; i < 26; i++) {
        if (childArray[i] != NULL) {
            count += node_count(childArray[i]);
        }
    }
    return count;
}

/**
 * @brief returns the number of words in all the nodes starting at the root. Recurses 
 * through all the nodes starting at the root and adds the number of words in the nodes to the final number
 * 
 * @param root the beginning node
 * @return int the number of words contained in all the nodes starting at the root
 */
int result_count(LookupTreeNode *root){
    if (root == NULL) {return 0;}
    LookupTreeNode **childArray = root -> children;
    WordList *words = root -> result_words;
    int count = words -> count;
    for (int i = 0; i < 26; i++) {
        if (childArray[i] != NULL) {
            count += result_count(childArray[i]);
        }
    }
    return count;
}

/**
 * @brief Converts the string parameter to UpperCase.
 * 
 * @param search the string to make all upper case
 * @return char* the pointer to the parameter
 */
char *toUpper(char *search) {
    int i = 0;
    while (search[i] != '\0') {
        if (search[i] == '\n') {search[i] = '\0'; i += 1; break;}
        if (search[i] >= 'a' && search[i] <= 'z') {
            search[i] -= 32;
        } else if (search[i] < 'A' && search[i] > 'Z') {
            printf("%d", search[i]);
            search[i] = 0;
        }
        i += 1;
    }
    return search;
}

/**
 * @brief Searches through root iteratively by the search parameter. If at any point the next 
 * character in the search parameter is not within the root path, then returns null. Otherwise, 
 * continues until search = '\0' where it then returns the new root
 * 
 * @param root the LookupTreeNode that we are starting out search at
 * @param search the string we are searching for
 * @return LookupTreeNode* 
 */
LookupTreeNode *lookup(LookupTreeNode *root, char *search) {
    while (search[0] != '\0') {
        int firstToDec = 0;
        // if lower case then set to uppercase
        if (search[0] >= 'a' && search[0] <= 'z') {
            firstToDec = search[0] - 97;
        } else {
            firstToDec = search[0] - 65;
        }
        // get the children array
        LookupTreeNode **children = root->children;
        // if the children array at index firstToDec is NULL, return NULL
        if (children[firstToDec] == NULL) {
            return NULL;
        }
        // update to the new root
        root = children[firstToDec];
        search += 1;
    }
    return root;
}

/**
 * @brief Iterates over the parameter words and creates a new LookupTreeNode with each subsequent letter 
 * of each word. (Ex. CALL -> ALL -> LL -> L) is the path each word takes when being put into the parameter 
 * for ltn_add_result_word. Returns the root of the new LookupTreeNode with all the words added to it.
 * 
 * @param words The WordList we are adding to the new LookupTreeNode
 * @return LookupTreeNode* The root of out new node containing all the words from words
 */
LookupTreeNode *build_tree_from_words (WordList *words) {
    LookupTreeNode *toRet = ltn_create();
    // gets how many words in words
    if (toRet == NULL) {return NULL;}
    int count = words->count;
    // The individual words in each wordlist
    char** wordList = words->words;
    for (int i = 0; i < count; i++) {
        // for each next letter in the word call ltn_add_result_word
        char* word = wordList[i];
        toUpper(word);
        // calls ltn_add_result_word for each iteration of word
        int j = 0;
        while (j < strlen(word)){
            ltn_add_result_word(toRet, word + j, wordList[i]);
            j += 1;
        }
    }
    return toRet;
}

/**
 * @brief Recursively prints all the words that are found from the result parameter all the way down to its 
 * lowest subtree member. Takes the words that were stored in each node and prints them.
 * 
 * @param result The root of out LookupTreeNode
 * @param search a string
 */
void print_words(LookupTreeNode *result, char *search) {
    if (result == NULL) {return;}
    LookupTreeNode **childArray = result -> children;
    WordList *wordList = result -> result_words;
    // Prints all the words contained in wordList
    for (int j = 0; j < wordList -> count; j++) {
        char* word = wordList -> words[j];
        printf("The string '%s' was found inside the word '%s'\n", search, word);
    }
    // iterates over each index of the childArray, and calls itself to print subtrees
    for (int i = 0; i < 26; i++) {
        if (childArray[i] != NULL) {
            print_words(childArray[i], search);
        }
    }
}

