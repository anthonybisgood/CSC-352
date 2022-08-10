/**
 * @file basketball.c
 * @author Anthony Bisgood
 * @brief CSC 352. Sorts and compares players in the NBA to eachother based on the games they've played
 * This compares the most/least consistent stats, and highest and lowest points of each 
 * respective area, (scored, rebounds, and assists)
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "arrayz.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

/**
 * @brief prints the results from the program
 * 
 * @param players the names of the player that made the cut
 */
void printResults(char players[12][50]) {
    printf("most consistent scorer: %s\n", players[0]);
    printf("most inconsistent scorer: %s\n", players[1]);
    printf("highest scorer: %s\n", players[2]);
    printf("lowest scorer: %s\n", players[3]);
    printf("most inconsistent rebounder: %s\n", players[4]);
    printf("most consistent rebounder: %s\n", players[5]);
    printf("highest rebounder: %s\n", players[6]);
    printf("lowest rebounder: %s\n", players[7]);
    printf("most inconsistent assister: %s\n", players[8]);
    printf("most consistent assister: %s\n", players[9]);
    printf("lowest assister: %s\n", players[10]);
    printf("highest assister: %s\n", players[11]);
}

/**
 * @brief Checks if the stats of the player passed are better than the current best stats
 * 
 * @param scored a long array of all the players scores
 * @param rebounds a long array of all the player rebounds
 * @param assists a long array of all the player assists
 * @param stats the current best stats
 * @param players the players that corespond to stats by index
 */
void checkIfBetter(long* scored, long* rebounds, long* assists,
double* stats, char players[12][50], char* name) {
    double stdevScored = sdev(scored);
    double stdevRebounds = sdev(rebounds);
    double stdevAssists = sdev(assists);
    double meanScored = mean(scored);
    double meanRebounds = mean(rebounds);
    double meanAssists = mean(assists);
    if (stdevScored < stats[0])     {stats[0] = stdevScored;    strcpy(players[0], name);}
    if (stdevScored > stats[1])     {stats[1] = stdevScored;    strcpy(players[1], name);}
    if (meanScored  > stats[2])     {stats[2] = meanScored;     strcpy(players[2], name);}
    if (meanScored  < stats[3])     {stats[3] = meanScored;     strcpy(players[3], name);}
    if (stdevRebounds > stats[4])   {stats[4] = stdevRebounds;  strcpy(players[4], name);}
    if (stdevRebounds < stats[5])   {stats[5] = stdevRebounds;  strcpy(players[5], name);}
    if (meanRebounds > stats[6])    {stats[6] = meanRebounds;   strcpy(players[6], name);}
    if (meanRebounds < stats[7])    {stats[7] = meanRebounds;   strcpy(players[7], name);}
    if (stdevAssists > stats[8])    {stats[8] = stdevAssists;   strcpy(players[8], name);}
    if (stdevAssists < stats[9])    {stats[9] = stdevAssists;   strcpy(players[9], name);}
    if (meanAssists < stats[10])    {stats[10] = meanAssists;   strcpy(players[10], name);}
    if (meanAssists > stats[11])    {stats[11] = meanAssists;   strcpy(players[11], name);}
}

/**
 * @brief Returns a char array of the "next game" so "Devin Booker[25,5,7]" will return "25,5,7"
 * 
 * @param buffer 
 * @return char* 
 */
void getNextGame(char* buffer, int index, char* nextGame) {
    int i = 0;
    index += 1;
    // until we hit the next game
    while (buffer[index] != ']') {
        nextGame[i] = buffer[index];
        i += 1;
        index += 1;
    }
    nextGame[i] = '\0';
}

/**
 * @brief takes in a buffer containing a single players stats, put those stats into the 3
 * arrays scored, rebounds, and assits. Perform calculations on those arrays in arrayz.c
 * if the stats are > or < the stats in stats, we replace the info in stats with the current 
 * player.
 * 
 * @param buffer 
 * @param stats 
 * @param players 
 */
void parsePlayerStats(char* buffer, double* stats, char players[12][50]) {
    long scored[101];
    long rebounds[101];
    long assists[101];
    char player[30];
    int statsIndex = 1;
    int i = 0;
    // copies down the name of the player
    while (buffer[i] != '[') {
        player[i] = buffer[i];
        i += 1;
    }
    player[i] = '\0';
    // iterates through the stats
    while (buffer[i] != '\0') {
        // only copies stats when we hit the '[', indicating a new score set
        if (buffer[i] == '[') {
            char nextGame[20];
            getNextGame(buffer, i, nextGame);
            char *token;
            token = strtok(nextGame, ",");
            int tokenIndex = 0;
            while (token != NULL) {
                if (tokenIndex == 0) {scored[statsIndex] = atoi(token);}
                if (tokenIndex == 1) {rebounds[statsIndex] = atoi(token);}
                if (tokenIndex == 2) {assists[statsIndex] = atoi(token);}
                token = strtok(NULL, ",");
                tokenIndex += 1;
            }
            statsIndex += 1;
        }
        i += 1;
    }
    // sets first index to how many games there were
    scored[0] = statsIndex - 1;
    rebounds[0] = statsIndex - 1;
    assists[0] = statsIndex - 1;
    checkIfBetter(scored, rebounds, assists, stats, players, player);
}

/**
 * @brief Reads the input file line by line and calls another method using those lines
 * 
 * @param filename 
 * @param stats 
 * @param players 
 */
void readFile(char* filename, double* stats, char players[12][50]){
    FILE* file = fopen(filename,"r");
    if(file==NULL){
        printf("Error: Could not open file \n");
        exit(1);
    }
    char buffer[1024];
    while (fgets(buffer,1024,file)) {
        parsePlayerStats(buffer, stats, players);
    }
    fclose(file);
}

/**
 * @brief Set the stats array to either INT_MAX or INT_MIN
 * 
 * @param stats 
 */
void setStats(double* stats) {
    stats[0] = INT_MAX;
    stats[1] = INT_MIN;
    stats[2] = INT_MIN;
    stats[3] = INT_MAX;
    stats[4] = INT_MIN;
    stats[5] = INT_MAX;
    stats[6] = INT_MIN;
    stats[7] = INT_MAX;
    stats[8] = INT_MIN;
    stats[9] = INT_MAX;
    stats[10] = INT_MAX;
    stats[11] = INT_MIN;
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("expects 1 command line argument\n");
        exit(1);
    }
    // stats holds the stats for the data we have to print out at the end
    double stats[12];
    setStats(stats);
    /* 2D char array, 12 slots, 30 chars for each slot
       Holds the names of the players relative to the index of stats*/
    char players[12][50];
    readFile(argv[1], stats, players);
    printResults(players);
}
