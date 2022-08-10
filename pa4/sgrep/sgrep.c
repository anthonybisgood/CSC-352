/**
 * @file sgrep.c
 * @author Anthony Bisgood
 * @brief A simplified version of the grep unix command. This program uses c to 
 *  parse through a text file using the command (cat filename.txt | ./sgrep (flagname) patterns).
 *  The 3 flags this code accepts are -i (case insensitive) -o (to print only the exact parts of the line
 *  that match) or -e (to use your own pattern that can be a combination of # and '.'. a # means that any 
 *  combination of characters can fit in there until the next character. A '.' means that any character can fit 
 *  inside. 
 * @version 0.1
 * @date 2022-02-15
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief checks if 2 chars are equal regardless of case using integer reps of alphabetic chars
 * 
 * @param a a alphabetic character
 * @param b a alphabetic character
 * @return int returns 0 if the chars dont match, 1 if they do
 */
int caseChecker(char a, char b) {
    // if the chars are equal (accounts for nonAlpha characters)
    if (a == b) {return 1;}
    // if a is upper case
    if (a > 64 && a < 91){
        // if b is lower case, convert to upper case then check for equality
        if (b > 96 && b < 123){
            b -= 32;
            if (a == b) {return 1;}
        } 
    // if a is lower case
    } else if (a > 96 && a < 123){
        // covernt b to lowercase and check for equality
        if (b > 64 && b < 91){
            b += 32;
            if (a == b) {return 1;}
        }
    }
    return 0;
}

/**
 * @brief for pattern specific flags, parses through the buffer using pointers to 
 * both the pattern and the buffer. To
 * 
 * @param pattern the pattern the user inputed
 * @param flagBank the flags the user inputed (e,i, or o)
 * @param buffer the line from the txt file that we are pattern matching for
 * @param patternIndex a pointer for the pattern
 * @param bufferIndex the start of the word that we check in the buffer
 * @return int returns the total length of the buffered string so we can print out the entirety later
 */
int patternSpec(char *pattern, int *flagBank, char *buffer, int bufferIndex) {
    if (buffer[bufferIndex] == ' ') {return 0;}
    int patternIndex = 0;
    int patternLen = strlen(pattern);
    int totalLen = 0;
    while (buffer[bufferIndex] != ' ' || buffer[bufferIndex] != '\n'){
        // if theres a . then it just updates the pointers
        if (pattern[patternIndex] == '.') {
            if (buffer[bufferIndex] == ' ' || buffer[bufferIndex] == '\n' 
            || buffer[bufferIndex] == '\0'){return 0;}
            bufferIndex += 1;
            patternIndex += 1;
            if (buffer[bufferIndex] == ' ' || buffer[bufferIndex] == '\n' 
            || buffer[bufferIndex] == '\0'){return 0;}
        /* if there is a # in the pattern then we set toFind to the next character in the pattern 
           A while loop is then performed over the buffer stopping when the character in the buffer 
           equals the toFind character in the pattern*/
        } else if (pattern[patternIndex] == '#'){
            char toFind = pattern[patternIndex + 1];
            while (buffer[bufferIndex] != toFind){
                if (buffer[bufferIndex] == ' ') {return 0;}
                bufferIndex+=1;
                totalLen += 1;
            }
            patternIndex+=1;
        // if just a regular character then checks for case insensitivity and equality
        } else {
            if (flagBank[1] == 1){
                if (caseChecker(buffer[bufferIndex], pattern[patternIndex]) == 0){return 0;}
            } else if (buffer[bufferIndex] != pattern[patternIndex]) {return 0;}
            bufferIndex+=1;
            patternIndex+=1;
        }
        totalLen += 1;
        patternLen -= 1;
        if (patternLen == 0) {
            return totalLen;}
    }
    return totalLen;
}


/**
 * @brief Checks if the next word (from the current index to the next space) matches the 
 *  pattern that we were passed in based on the flags. Because there are no special characters, 
 *  (# or .) we dont have to keep track of the pattern length and can just end the loop 
 *  when the pattern index == the patternlen
 * 
 * @param pattern the pattern the user inputed
 * @param flagBank the flags the user inputed (e,i, or o)
 * @param buffer the line from the txt file that we are pattern matching for
 * @param index where we check relative to the buffer
 * @return int returns the length of the pattern
 */
int wordChecker(char *pattern, int *flagBank, char *buffer, int index) {
    if (buffer[index] == ' ') { return 0; }
    int patternIndex = 0;
    // for each character, iterate through next word
    int bufferIndex = index;
    // goes through rest of word form index to space
    while (buffer[bufferIndex] != ' ' || buffer[bufferIndex] != '\n') {
        // if case insensitive first check for equality
        if (flagBank[1] == 1) {
            int res = caseChecker(buffer[bufferIndex], pattern[patternIndex]);
            if (res == 0) {return 0;}
        // if characters dont match and case sensitive
        } else if (buffer[bufferIndex] != pattern[patternIndex]) { 
            return 0;
        }
        // if we've reached the end of the pattern without returning
        bufferIndex += 1;
        patternIndex += 1;
        if (patternIndex == strlen(pattern)){return strlen(pattern);}
    } 
    return strlen(pattern);
}

/**
 * @brief Loops through the txt file with standard input and calls wordChecker and patternSpec 
 * with each line
 * 
 * @param pattern the pattern the user inputed
 * @param flagBank the flags the user inputed (e,i, or o)
 */
void lineLoop(char *pattern, int *flagBank){
    char buffer[200];
    // line by line, max len is 200 characters
    while (fgets(buffer, 200, stdin) != NULL) {
        int i = 0;
        // char by char
        while (buffer[i] != '\0') {
            int theSame = 0;
            if (flagBank[2] == 1){
                theSame = patternSpec(pattern, flagBank, buffer, i);
            } else {
                theSame = wordChecker(pattern, flagBank, buffer, i);
            }
            // if the pattern in the line
            if (theSame > 0) {
                // to print the whole line and moves on to next line
                if (flagBank[0] == 0){
                    printf("%s", buffer);
                } else {
                    // prints just the characters that match
                    for (int bufferIndex = 0; bufferIndex < theSame; bufferIndex++) {
                        int tot = bufferIndex + i;
                        printf("%c", buffer[tot]);
                    }
                    printf("\n");
                }
                if (flagBank[0] == 0) {break;}
            }
            i += 1;
        }
    }
}

/**
 * @brief Validates that the pattern is valid and contains only upper/lowercase letters
 * and hashtags or periods
 * 
 * @param pattern the user inputed pattern
 */
void validatePattern(char *pattern){
    // checks first and last characters
    if (pattern[0] == '#') {
        printf("Invalid search term.\n");
        exit(2);
    }
    int i = 0;
    while (pattern[i] != '\0'){
        // if not a upper/lower case letter
        if ((pattern[i] < 65 || pattern[i] > 90) && (pattern[i] < 97 || pattern[i] > 122)){
            if (!(pattern[i] == '.' || pattern[i] == '#')){
                printf("Invalid search term.\n");
                exit(2);
            }
            // make sure before and after are characters
        } 
        if (pattern[i] == '#'){
            if (pattern[i-1] == '.' || pattern[i+1] == '.' ||
             pattern[i-1] == '#' || pattern[i+1] == '#') {
                printf("Invalid search term.\n");
                exit(2);
            }
        }
        i += 1;
    }
    // if last char a #
    if (pattern[i-1] == '#'){
        printf("Invalid search term.\n");
        exit(2);
    }
}

int main(int argc, char *argv[]){
    // array of flag arguments (-o,-i,-e, pattern)
    int flagBank[3] = {0, 0, 0};
    // the pattern/string that we should search for
    char pattern[200];
    int isPattern = 0;
    if (argc < 2){
        printf("Requires more command-line arguments.\n");
        exit(1);
    }
    for (int i = 1; i < argc; i++){
        // if a flag
        if (argv[i][0] == '-'){
            if (argv[i][1] == 'o'){
                flagBank[0] = 1;
            } else if (argv[i][1] == 'i'){
                flagBank[1] = 1;
            } else if (argv[i][1] == 'e'){
                flagBank[2] = 1;
            }
        } else{
            strcpy(pattern, argv[i]);
            isPattern = 1;
        }
    }
    // if it doesnt see a pattern
    if (flagBank[2] == 1 && isPattern == 0){
        printf("Invalid search term.\n");
        exit(2);
    }
    //validate it
    validatePattern(pattern);
    lineLoop(pattern, flagBank);
    if (flagBank[0] == 0){ printf("\n") ;}
    return 0;
}