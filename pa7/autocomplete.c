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
            if (buffer[i] > 'a' && buffer[i] < 'z') {
                buffer[i] -= 32;
            } else if (buffer[i] < 'A' || buffer[i] > 'Z') {
                printf("Enter a valid input: %s", buffer);
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