#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "autocomplete.h"



void read_loop(LookupTreeNode *root);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "SYNTAX: %s <dictionaryFile>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Could not open the dictionary file '%s'\n", argv[1]);
        return 2;
    }


    WordList *words = build_wordlist_from_file(fp);
    if (words == NULL)
    {
        fprintf(stderr, "Could not build the word list.\n");
        return 2;
    }
    fclose(fp);

    printf("%d words in the input list.\n", words->count);


    LookupTreeNode *root = build_tree_from_words(words);
    if (root == NULL)
    {
        fprintf(stderr, "Could not build the lookup tree.\n");
        return 2;
    }
    printf("%d nodes constructed from the word list.\n", node_count(root));
    printf("%d word-results found in the tree.\n", result_count(root));
    printf("\n");


    read_loop(root);


    /* clean up the entire tree.  Then, iterate through the global word list
     * and free each individual word.  Then free the list itself.
     */
    ltn_destroy(root);

    for (int i=0; i<words->count; i++)
        free(words->words[i]);

    wl_destroy(words);


    return 0;
}



/* why is this in its own function, instead of main()?  I originally planned
 * to hhave the students write it.  I don't feel like porting it to go back
 * into main().
 *
 * And to be honest, when you look at main(), this actually isn't a bad
 * organization, IMHO.  It just feels a tiny bit weird, in this case, since
 * most of the steps are implemented in autocomplete.c, and only one of the
 * steps is implemented here.
 */
void read_loop(LookupTreeNode *root)
{
    char buf[256];
    while (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        int len = strlen(buf);

        /* remove the newline, if any */
        if (buf[len-1] == '\n')
        {
            buf[len-1] = '\0';
            len--;
        }
        if (len == 0)
            continue;   // skip blank lines

        printf("User typed: %s\n", buf);

        int ok = 1;
        for (int i=0; i<len; i++)
        {
            char c = buf[i];
            if (c < 'A' || (c > 'Z' && c < 'a') || c > 'z')
            {
                ok = 0;
            }
        }
        if (ok == 0)
        {
            fprintf(stderr, "The user string '%s' contained non-alphabetic characetrs, skipping!\n", buf);
            fprintf(stderr, "\n");
            continue;
        }

        LookupTreeNode *result = lookup(root,buf);
        if (result == NULL)
        {
            printf("The word '%s' was not matched by any word in the input list.\n", buf);
            printf("\n");
            continue;
        }

        print_words(result, buf);
        printf("\n");
    }
}


