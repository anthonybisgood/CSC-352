/**
 * CSC 352
 * 
 * @file zstr.c
 * @author Anthony Bisgood (anthonybisgood@email.arizona.edu)
 * @brief This program mimics the Str library interface. It uses mallocation and pointers
 * to store string data and metadata, as well as adjusts the size of the string when needed.
 * A number of functions can be performed on the zstring, such as create, destroy, append, search, 
 * compare, substring, and print
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "zstr.h"
#include <stdlib.h>
#include <stdio.h>
zstr_code zstr_status;

/**
 * @brief Creates a new zstr based on the size of the initial_data and allocates
 * memory on the heap with malloc() and free(). In addition, meta data is stored at
 * the front of the zstr containing the size and length of the 
 * 
 * @param initial_data a String that we add to the zstr
 * @return zstr 
 */
zstr zstr_create(char* initial_data) {
    int sizeOfInitial = mySizeOf(initial_data);
    int idSize = sizeOfInitial + 1 + 4 + 4;
    int mallocSize = 0;
    if (idSize <= 16) {mallocSize = 16;}
    else if (idSize <= 32) {mallocSize = 32;}
    else if (idSize <= 256) {mallocSize = 256;}
    else if (idSize <= 1024) {mallocSize = 1024;}
    else if (idSize <= 2048) {mallocSize = 2048;}
    // if idSize > 2048
    else {
        printf("invalid initial_data length \n");
        errorCode(ZSTR_ERROR);
    }
    char* mallocated_space = malloc(mallocSize);
    // if mallocating space fails
    if (mallocated_space == NULL) {errorCode(ZSTR_ERROR);}
    // next four lines adds the int metadata to the front of the zstr
    int* sizeLoc = (int*) (&mallocated_space[0]);
    *sizeLoc = sizeOfInitial;
    int* mallocSpot = (int*) (&mallocated_space[4]);
    *mallocSpot = mallocSize;
    // adds the letters to the rest of the zstr
    for (int i = 0; i < sizeOfInitial; i++) {
        mallocated_space[i+8] = initial_data[i];
    }
    mallocated_space[8+sizeOfInitial] = '\0';
    return mallocated_space + 8;
}

/**
 * @brief Frees the memory allocated to to_destroy
 * 
 * @param to_destroy the zstr to destroy
 */
void zstr_destroy(zstr to_destroy) {
    zstr trueZstr = to_destroy - 8;
    free(trueZstr);
}

/**
 * @brief Appends 2 zstr's together, if when combined. Uses loops to run through the base 
 * and to_append zstrings and combines them into one zstr.
 * 
 * @param base the zstr we add to 
 * @param to_append the zstr we are adding to the base
 */
void zstr_append (zstr* base, zstr to_append) {
    int baseSize = mySizeOf(*base);
    int appSize = mySizeOf(to_append);
    // check for size problem
    // TODO: might change based on how ints are stored
    zstr* tempBase = base - 8;
    int baseAllocation = *((int*) &tempBase[4]);
    int appendedSize = baseSize + appSize;
    //if formula for max # of characters is less than the number of characters, resize
    char newBase[appendedSize + 1];
    // pointer for to_append array
    int j = 0;
    for (int i = 0; i < appendedSize; i += 1){
        if (base[0][i] != '\0') {
            newBase[i] = base[0][i];
        }
        else {
            newBase[i] = to_append[j];
            j += 1;
        }
    }
    // adds an endline char to the newBase
    newBase[appendedSize] = '\0';
    if ((baseAllocation-(sizeof(int)*2)-1) < (baseSize+appSize)){
        zstr_destroy(*base);
    }
    *base = zstr_create(newBase);
}

/**
 * @brief should search for the first occurrence of
 * to_search within base. It should return the index if found, or -1 if not found.
 * It should return the index basedon the beginning of the actual char array.
 * 
 * @param base the zstr we are searching through
 * @param to_search the zstr we are trying to find
 * @return int the index of to_search in base
 */
int zstr_index(zstr base, zstr to_search) {
    // outer loop runs through the base
    int outer = 0;
    while (base[outer] != '\0') {
        int found = 1;
        int inner = 0;
        // runs through the inner loop
        while (to_search[inner] != '\0') {
            // if letters are diff, breaks
            if (to_search[inner] != base[inner + outer]) {
                found = 0;
                break;
            }
            inner += 1;
        }
        if (found) {return outer;}
        outer += 1;
    }
    return -1;
}

/**
 * @brief Returns the count of how many times to_search appears in the base
 * 
 * @param base the zstr we are searching through
 * @param to_search the zstr we are trying to find 
 * @return int the number of times to_search appears in the base
 */
int zstr_count(zstr base, zstr to_search) {
    int count = 0;
    int outer = 0;
    while (base[outer] != '\0') {
        int found = 1;
        int inner = 0;
        while (to_search[inner] != '\0') {
            if (to_search[inner] != base[inner + outer]) {
                found = 0;
                break;
            }
            inner += 1;
        }
        if (found) {count += 1;}
        outer += 1;
    }
    return count;
}

/**
 * @brief should compare return ZSTR_GREATER if x > y,
 *  ZSTR_EQUAL if x == y, and ZSTR_LESS if x < y. 
 * 
 * @param x a zstr to compare
 * @param y a zstr to compare
 * @return int returns 0 if the zstr's are equal, 1 if x > y, or -1 if x < y
 */
int zstr_compare(zstr x, zstr y) {
    int xSize = mySizeOf(x);
    int ySize = mySizeOf(y);
    // if sizes arent the same
    if (xSize != ySize) {
        if (xSize > ySize) {return ZSTR_GREATER;}
        else {return ZSTR_LESS;}
    }
    // we know sizes are the same, returns when the first instance of a dufferent letter
    int i = 0;
    while (x[i] != '\0' && y[i] != '\0') {
        if (x[i] > y[i]) {return ZSTR_GREATER;}
        else if (x[i] < y[i]) {return ZSTR_LESS;}
        i += 1;
    }
    return ZSTR_ERROR;
}

/**
 * @brief Creates a new zstr with the contents of the base that are in
 * begin (inclusive) and end (exclusive). Also does basic checking for validity.
 * 
 * @param base the zstr we parse through
 * @param begin the starting index
 * @param end the ending index
 * @return zstr the substring of base
 */
zstr zstr_substring(zstr base, int begin, int end) {
    if (end < 0) {end = 0;}
    if (begin < 0) {begin = 0;}
    if (end < begin) {zstr_status = ZSTR_ERROR;}
    char newBase[end-begin + 1];
    int newBaseIndex = 0;
    int i = begin;
    // if not the end or begin <end
    while (base[i] != '\0' && i < end) {
        newBase[newBaseIndex] = base[i];
        newBaseIndex += 1;
        i += 1;
    }
    // sets end char
    newBase[newBaseIndex] = '\0';
    return zstr_create(newBase);
}

/**
 * @brief Prints the zstring and any meta data that it has at the from
 * 
 * @param data the zstr to print
 */
void zstr_print_detailed(zstr data) {
    int size = mySizeOf(data);
    char string[size + 1];
    int i = 0;
    while (data[i] != '\0') {
        string[i] = data[i];
        i += 1;
    }
    string[i] = '\0';
    zstr trueStart = data-8;
    printf("STRLENGTH: %d\n",*((int*) &trueStart[0]));
    printf(" DATASIZE: %d\n", *((int*) &trueStart[4]));
    printf("   STRING: >%s", string);
    printf("<\n");
}

/**
 * @brief Sets the zstr_status to the errorType and exits the program 
 * with the errorType
 * 
 * @param errorType the ZSTR code denoting an error
 */
void errorCode(int errorType) {
    zstr_status = errorType;
    exit(errorType);
}

/**
 * @brief iterates through the word and returns the size of the word
 * 
 * @param word the string we iterate through
 * @return int the size of the word
 */
int mySizeOf(char* word) {
    int i = 0;
    while (word[i] != '\0'){
        i += 1;
    }
    return i;
}