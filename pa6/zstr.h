/**
 * CSC 352
 * 
 * @file zstr.h
 * @author Anthony Bisgood (anthonybisgood@email.arizona.edu)
 * @brief This program mimics the Str library interface. It uses mallocation and pointers
 * to store string data and metadata, as well as adjusts the size of the string when needed.
 * A number of functions can be performed on the zstring, such as create, destroy, append, search, 
 * compare, substring, and print.
 * @version 0.1
 * @date 2022-03-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// what we use to represent a zstr when using this library
typedef char* zstr; 
// used to specify various types of errors.
typedef int zstr_code;

/**
 * Error codes for zstr
 */
static const int ZSTR_OK = 0;
static const int ZSTR_ERROR = 100;
static const int ZSTR_GREATER = 1;
static const int ZSTR_LESS = -1;
static const int ZSTR_EQUAL = 0;


zstr zstr_create(char* initial_data);
void zstr_destroy(zstr to_destroy);
void zstr_append (zstr * base, zstr to_append);
int zstr_index(zstr base, zstr to_search);
int zstr_count (zstr base, zstr to_search);
int zstr_compare (zstr x, zstr y);
zstr zstr_substring (zstr base, int begin, int end);
void zstr_print_detailed(zstr data);

/**
 * @brief returns the number of chars in word 
 * "test1" returns 5
 * 
 * @param word 
 * @return int 
 */
int mySizeOf(char* word);
void errorCode(int errorType);
