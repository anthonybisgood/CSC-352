#include <stdlib.h>
#include <stdio.h>


int main(){
    char buffer[128];
    char alphabet[]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // while there are lines in the standard input (input.txt)
    // fgets(*where chars are stores, max # of chars to be read, fileStream)
    int isFirstLine = 1;
    char key[128];
    int keyIndex = 0;
    while (fgets(buffer, 127, stdin) != NULL){
        // coppies the first line from the buffer into the key char key[] array
        if (isFirstLine){
            int i = 0;
            while (buffer[i] != '\0') {
                key[i] = buffer[i];
                i += 1;
            };
            isFirstLine = 0;
        } else {
            // for the actual message
            int i = 0;
            while (buffer[i] != '\0') {
                // if the key has ran through reset the keyIndex
                if (key[keyIndex + 1] == '\0'){
                    keyIndex = 0;
                }
                if (buffer[i] == '\n'){
                    keyIndex += 1;
                    break;
                }
                // increment i without incrementing the keyIndex
                if (buffer[i] == ' '){
                    i += 1;
                    printf(" ");
                    continue;
                    // encryption is just the key - message
                } else if (buffer[i] > key[keyIndex]){
                    int encryption = key[keyIndex] - buffer[i] + 26;
                    printf("%c", alphabet[encryption]);
                } else {
                    int encryption = key[keyIndex] - buffer[i];
                    printf("%c", alphabet[encryption]);
                }
                keyIndex += 1;
                i += 1;
            };
            keyIndex -= 1;
            printf("\n");
        }
    }
}






