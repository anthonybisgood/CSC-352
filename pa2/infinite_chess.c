  GNU nano 4.8                                                          infinite_chess.c                                                           Modified
#include <stdlib.h>
#include <stdio.h>

int brigadier(int moves);
int king(int moves);
int pawn(int moves);

int main() {
        char piece;
        int moves;
        printf("Enter piece type (k, b, p):\n");
        scanf("%c", &piece);
        printf("Enter number of moves:\n");
        scanf("%d", &moves);
        int res;
        if (piece == 'k'){
                res = king(moves);
        } else if (piece == 'b'){
                res = brigadier(moves);
        } else if (piece == 'p'){
                res = pawn(moves);
        } else {
                printf("Enter a valid piece. \n");
        }
        printf("possible locations: %d \n", res);
        return 0;
}

int brigadier(int moves){
        int total = 0;
        for (int i = 1; i < moves + 1; i++){
                if(i == 0){
                        printf("here");
                        total += 1;
                } else{
                        printf("there");
                        total += 4*(i-1);
                }
                printf("over here");
        }
        return total;
}

int king(int moves) {
        int res = (2*moves + 1) * (2*moves + 1);
        return res;
}

int pawn(int moves) {
        int res = moves + 1;
        return res;
}




