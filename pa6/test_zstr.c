#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include<ctype.h>
#include "zstr.h"

void testCreate() {
    zstr res = zstr_create("abcdefghijklmnopqrstuvwxyz");
    printf("zstr: ");
    res = res-8;
    int x = *((int*) &res[0]);
    printf("%d, ", x);
    int y = *((int*) &res[4]);
    printf("%d, ", y);
    // for (int i = 0; i < 8; i++){
    //     printf("%d, ", res[i]);
    // }
    // printf("%s", res);
    int i = 8;
    while (res[i] != '\0') {
        printf("%c, ", res[i]);
        i+=1;
    }
    printf("\n");
    //printRes(res);
}

void testDestroy() {
    // zstr res = zstr_create("test1");
    // printf(res);
    // zstr_destroy(res);
    // printf(res);
}

void testAppend(){
    zstr az = zstr_create("abcdefghijklmnopqrstuvwxyz");
    zstr wa = zstr_create("wa");
    zstr_append(&az, wa);
    printf("%s\n", az);

    zstr y = zstr_create("yoteabcdefghijklmnopqrstuvwxyz");
    zstr g = zstr_create("giraffe");
    zstr_append(&y, g);
    printf("%s\n", y);
    zstr_print_detailed(y);
}

void testSearchHelper(char* a1, char*a2) {
    zstr az = zstr_create(a1);
    zstr wa = zstr_create(a2);
    int res = zstr_index(az, wa);
    printf("Result: %d\n", res);
}

void testSearchHelper2(char* a1, char*a2) {
    zstr az = zstr_create(a1);
    zstr wa = zstr_create(a2);
    int res = zstr_count(az, wa);
    printf("Result: %d\n", res);
}

void testSearch() {
    testSearchHelper("abcd", "abcd");
    testSearchHelper("dwa", "d");
    testSearchHelper("dda", "a");
    testSearchHelper("dda", "ddaddaddawadw");
    testSearchHelper2("abcd", "abcd");
    testSearchHelper2("dwa", "d");
    testSearchHelper2("dda", "a");
    testSearchHelper2("dddddddda", "dd");
}
void compareHelper(char* a1, char*a2) {
    zstr az = zstr_create(a1);
    zstr wa = zstr_create(a2);
    int res = zstr_compare(az, wa);
    printf("Result: %d\n", res);    
}

void testCompare() {
    compareHelper("a", "a");
    compareHelper("a", "b");
    compareHelper("a", "c");
    compareHelper("z", "d");
    compareHelper("aaaa", "aaaa");
    compareHelper("", "");
    compareHelper("abc", "abe");
    compareHelper("apples", "apple");
}

void subHelper(char* z, int beg, int end) {
    zstr az = zstr_create(z);
    zstr res = zstr_substring(az, beg, end);
    printf("%s\n", res);
}

void testSubStr() {
    subHelper("DeoxyribonucleicAcid", 16, 20);
    subHelper("DeoxyribonucleicAcid", 16, 24);
    subHelper("DeoxyribonucleicAcid", -4, 3);
    subHelper("Deo", 0, 0);
    subHelper("Deo", 0, -1);
    subHelper("Deo", 0, 4);
}

int main(int argc, char** argv) {
    testCreate();
    // testDestroy();
    testAppend();
    // testSearch();
    // testCompare();
    // testSubStr();
}

