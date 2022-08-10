#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * @author Anthony Bisgood 
 * 
 * @brief This c program takes in a command such 
 *      (cat (NAME_OF_FILE) | scut (-l/-w/-c) (integers 1-100))
 * and parses through the file, printing out the columns (second argument).
 * The user can decide to parse the file through the digits, split up the file 
 * by spaces, or by commas.
 */

/**
 * @brief Parses through the second command line argument to get the columns
 * that we are looking for in the files. Columns must only have numbers 1-100,
 * commas, and dashes. Dashes signify the range of numbers through a-b.
 *
 * @param columns the char array that the user inputs (ex. 1-5,6,7)
 * @param colArray the array of all total columns after parsing
 * @return int the total number of columns that we need
 */
int parseArgs(char *columns, int *colArray)
{
    // if first and last are not digits
    if (isdigit(columns[0]) == 0 || !isdigit(columns[strlen(columns) - 1]))
    {
        printf("Invalid selection.\n");
        exit(1);
    }
    int start = 0, colPointer = 0, end = 0;
    for (int i = 0; columns[i] != '\0'; i++)
    {
        // if arg is not a digit, comma, or dash (doesnt work with $ for some reason)
        if (isdigit(columns[i]) == 0 && columns[i] != ',' && columns[i] != '-')
        {
            printf("Invalid selection.\n");
            exit(1);
        }
        // check first, if we go in then start has an int in it, for loop to add to colArray
        if (columns[i] == '-')
        {
            // if single digit after dash
            if (isdigit(columns[i + 1]) && !isdigit(columns[i + 2]))
            {
                end += columns[i + 1] - '0'; // ones
                i += 1;
            }
            // if double digit after the dash
            else if (isdigit(columns[i + 2]))
            {
                end += 10 * (columns[i + 1] - '0'); // tens
                end += (columns[i + 2] - '0');      // ones
                i += 2;
            }
            // add all nums from start to end
            for (int j = start; j <= end; j++)
            {
                colArray[colPointer] = j;
                colPointer += 1;
            }
            // reset start and ends
            start = 0;
            end = 0;
        }
        // double digits
        else if (isdigit(columns[i]) && isdigit(columns[i + 1]))
        {
            start += 10 * (columns[i] - '0');
            start += columns[i + 1] - '0';
            // because we added the columns[i+1] since we know its a digit
            i += 1;
        }
        // single digits
        else if (isdigit(columns[i]) && !isdigit(columns[i + 1]))
        {
            start += columns[i] - '0';
        }
        // if end of file or we hit a comma then add current num to colArray 
        if ((columns[i] == ',' || columns[i + 1] == '\0' || columns[i + 2] == '\0') && start != 0)
        {
            colArray[colPointer] = start;
            colPointer += 1;
            start = 0;
        }
    }
    return colPointer;
}

/**
 * @brief Parses each line by character and prints out only the characters that
 * are found in the columns in colArray.
 *
 * @param columns the char array that the user inputs (ex. 1-5,6,7)
 * @param colArray the array of all total columns after parsing
 */
void byDigit(int columns, int *colArray)
{
    char buffer[100];
    // goes through, buffer is the line
    while (fgets(buffer, 127, stdin) != NULL)
    {
        int i = 0;
        int colPointer = 0;
        // goes digit by diget per line
        while (buffer[i] != '\0')
        {
            // if i(the current column) matches with colArray, then we print
            if (i == colArray[colPointer] - 1)
            {
                printf("%c ", buffer[i]);
                colPointer += 1;
            }
            i += 1;
        }
        printf("\n");
    }
}

/**
 * @brief Parses each line by Space and prints out only the characters that
 * are found in the columns in colArray.
 *
 * @param columns the char array that the user inputs (ex. 1-5,6,7)
 * @param colArray the array of all total columns after parsing
 */
void bySpace(int columns, int *colArray)
{
    char buffer[100];
    while (fgets(buffer, 127, stdin) != NULL)
    {
        char *token;
        // sets to split string at space and newlines
        token = strtok(buffer, " \n");
        int i = 0;
        int colPointer = 0;
        while (token != NULL)
        {
            if (i == colArray[colPointer] - 1)
            {
                printf("%s ", token);
                colPointer += 1;
            }
            // sets new token
            token = strtok(NULL, " \n");
            i += 1;
        }
        printf("\n");
    }
}

/**
 * @brief Parses each line by commas and prints out only the characters that
 * are found in the columns in colArray.
 *
 * @param columns the char array that the user inputs (ex. 1-5,6,7)
 * @param colArray the array of all total columns after parsing
 */
void byComma(int columns, int *colArray)
{
    char buffer[100];
    while (fgets(buffer, 127, stdin) != NULL)
    {
        char *token;
        // sets to split string at commas and newlines
        token = strtok(buffer, ",\n");
        int i = 0;
        int colPointer = 0;
        while (token != NULL)
        {
            if (i == colArray[colPointer] - 1)
            {
                printf("%s ", token);
                colPointer += 1;
            }
            // sets new token
            token = strtok(NULL, ",\n");
            i += 1;
        }
        printf("\n");
    }
}

/**
 * @param argc the number of command line arguments
 * @param argv the command line arguments with the first one being the
 * name of the program
 * @return int exit code
 */
int main(int argc, char *argv[])
{
    int colArray[100];
    int numOfColumns;
    // if not 2 arguments
    if (argc != 3)
    {
        printf("expected 2 command line arguments.\n");
        return 1;
        // strcmp returns 0 if strings are equal, 1 if not equal
    }
    else if ((argv[1][1] != 'l' || argv[1][1] != 'w' || argv[1][1] != 'c') && argv[1][0] != '-')
    {
        printf("Invalid delimiter type.\n");
        return 1;
    }
    else
    {
        numOfColumns = parseArgs(argv[2], colArray);
    }
    if (argv[1][1] == 'l')
    {
        byDigit(numOfColumns, colArray);
    }
    else if (argv[1][1] == 'w')
    {
        bySpace(numOfColumns, colArray);
    }
    else if (argv[1][1] == 'c')
    {
        byComma(numOfColumns, colArray);
    } else {
        printf("Invalid delimiter type.\n");
        return 1;
    }
    return 0;
}
