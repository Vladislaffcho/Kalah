#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "Rules.h"
//
#define hCon GetStdHandle(STD_OUTPUT_HANDLE)
#define infinity 10000
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

enum TURN {PRINT, CALCULATE};
enum LEVEL {EASY = 1, MEDIUM = 3, HARD = 5};
enum SIDE {A, B};

enum LEVEL maxDepth = EASY;

int setGame(void);    // select game parameters
void printDesk(int, int *); // print desk
void setStonesPosition(int); // prints stones (numbers) in cells
int makeTurn(int*, int, enum TURN); // scatters stones to cells
int isEmptySide(int*, enum TURN, int); // check if the end game condition has been reached
int bestTurn(int*); // initial function to start calculating best turn
int alphaBeta(int*, int, enum SIDE, int, int); // main algorythm to calculate the best turn for computer
int evaluate(int*); // evaluate terminal position
int maxA(int, int); // find alpha
int minB(int, int); // find beta
int getUserTurn(int * desk, enum SIDE, int gameType); // function to check if user entered a valid number
void dropStones(int * desk);
void Error (int type);
void dropRemainStones(int * desk);

int main(int argc, char *argv[])
{
    enum TURN action = PRINT;
    enum SIDE side = A;
    int moveNum = 0;
    int i = 0;
    int playerTurn = 0;
    int gameType = 0;
    int startDesk[14] = {6, 6, 6, 6, 6, 6, 0, 6, 6, 6, 6, 6, 6, 0};
    int desk[14];

    system("mode con cols=81 lines=20");

    while (1) {
        if (gameType == 0) {
            system("cls");
            gameType = setGame();
            system("cls");
            for (i = 0; i < 14; i++) desk[i] = startDesk[i];
            printDesk(gameType, desk);
            side = A;
            moveNum = 0;
        }

        if (!isEmptySide(desk, action, gameType)) {
            gameType = 0;
            continue;
        }

        playerTurn = getUserTurn(desk, side, gameType);

        if (playerTurn == -1) {
            gameType = 0;
            continue;
        } else if (moveNum == 0 && playerTurn == 1) {
            Error(6);
            continue;
        } else if (moveNum == 0) {
            moveNum = 1;
        }

        if (makeTurn(desk, playerTurn, action)) continue;

        if (side == A) side = B;
        else side = A;
    }
    return 0;
}

void dropStones(int * desk) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int i = 0;
    for (i = 0; i < 14; i++) {
        if (i < 7) SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
        else SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
        setStonesPosition(i);
        printf("%d ", desk[i]);
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int getUserTurn(int * desk, enum SIDE side, int gameType) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    char userTurn;
    int turn;

    while(1) {
        if (side == A) { // sets cursor in user A position
            gotoxy(78,8);
            printf("  ");
            gotoxy(78,8);
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
        } else if (side == B) { // sets cursor in user B position
            gotoxy(17,8);
            printf("  ");
            gotoxy(17,8);
            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
        
        if (gameType == 2 && side == B) { // computer turn
            userTurn = bestTurn(desk) + '0';
            printf("%c", userTurn);
            Sleep(1000);
        } else {                        // user A or B turn
            userTurn = getch();     
            printf("%c", userTurn);
        }

        if (userTurn == 27) {            // if user pressed Esc
            gotoxy(25,16);
            printf("Quit game? (Press Esc to quit the game)");
            userTurn = getch();
            if (userTurn != 27) {
                gotoxy(25,16);
                printf("                                                   ");
                continue;
            } 
            else  {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                return -1;
            }
        } else if (userTurn < 49 || userTurn > 54) {
            Error(1);
            continue;
        }

        turn = atoi(&userTurn);

        if (side == B) turn += 7;

        if(desk[turn-1] == 0) {
            Error(2);
            continue;
        }

        return turn;
    }
}

int setGame(void) {
    char gameType;
    char compLevel = 0;
    while (1) {
        gotoxy(35,4);
        printf("Kalah game");
        gotoxy(25,7);
        printf("1 - Start new Player VS Player game");
        gotoxy(25,8);
        printf("2 - Start new Player VS Computer game");
        gotoxy(25,9);
        printf("3 - Read Kalah rules");
        gotoxy(25,10);
        printf("Esc - Quit");
        gotoxy(25,6);
        printf("Make a choice: "); 
        gameType = getch();
        switch (gameType) {
            case '1': return 1;
            case '2': {
                while (1) {
                    gotoxy(25,12);
                    printf("Easy   (e)");
                    gotoxy(25,13);
                    printf("Medium (m)");
                    gotoxy(25,14);
                    printf("Hard   (h)\n");
                    gotoxy(25,11);
                    printf("Select opponent level: ");
                    compLevel = getch();
                    if (compLevel != 'e' && compLevel != 'm' && compLevel != 'h') {
                        if (compLevel == 27) {
                            system("cls");
                            break;
                        }
                        Error(4);
                        gotoxy(1,13);
                        printf("                                                   ");
                        continue;
                    } else if (compLevel == 'm') maxDepth = MEDIUM;
                    else if (compLevel == 'h') maxDepth = HARD;
                    return 2;
                }
                continue;
            }
            case '3': {
                printRules();
                continue;
            }
            case 27: {
                gotoxy(25,16);
                printf("Quit game? (Press Esc to quit the game)");
                gameType = getch();
                if (gameType != 27) {
                    gotoxy(25,16);
                    printf("                                                   ");
                    continue;
                }
                else exit(0);
            }
            default: {
                Error(3);
                continue;
            }
        }
    }
}

int makeTurn(int * desk, int move, enum TURN action) {

    int i, stones, repeatTurn = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    i = --move;
    stones = desk[i];
    desk[i] = 0;
    if (action == PRINT) {
        if (i < 7) SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
        else SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
        setStonesPosition(i);
        printf("%d ", desk[i]);
    }

    while(stones--) {
        i++;
        desk[i]++;
        if ((move > 6 && i == 6) || (move < 6 && i == 13)) { // moving around the table
            desk[i]--;
            stones++;
        } else if (action == PRINT) {
            if (i < 7) SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
            else SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
            setStonesPosition(i);
            Sleep(100);
            printf("%d ", desk[i]);
        }
        if (i == 13 && stones > 0) i = -1;
    }

    if ((desk[i] == 1 && desk[12-i]) && // check Kalah rule 1.3 - if last stone is in the mover's free cell
        ((move < 6 && i < 6) || (move > 6 && i > 6 && i < 13))) {  // and the opposite cell is not empty
        if (move > 6) stones = 13;
        else stones = 6;

        desk[stones] = desk[stones] + desk[i] + desk[12-i];
        desk[i] = desk[12-i] = 0;
        if (action == PRINT) {
            if (move < 6) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
                setStonesPosition(stones);
                printf("%d ", desk[stones]);
                setStonesPosition(i);
                printf("%d ", desk[i]);
                setStonesPosition(12-i);
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("%d ", desk[12-i]);
            }
            else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
                setStonesPosition(stones);
                printf("%d ", desk[stones]);
                setStonesPosition(i);
                printf("%d ", desk[i]);
                setStonesPosition(12-i);
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
                printf("%d ", desk[12-i]);
            }
        }
    }

    if (move < 6 && i == 6 || move > 6 && i == 13) repeatTurn = 1; // rule 1.2 - if last stone is in Kalah, make a move again
    return repeatTurn;
}

void printDesk(int gametype, int * desk) { // procedure to print desk on the s?reen
    int i = 0;
    int j = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD textPosition = {27,1};

    SetConsoleCursorPosition(hCon,textPosition);
    for(i = 6; i > 0; i--) {
        printf("%d    ", i);
    }

    textPosition.X = 20;
    textPosition.Y = 3;
    for(i = 0; i < 9; i++) {
        SetConsoleCursorPosition(hCon,textPosition);
        for (j = 0; j < 41; j++) {
            if (i == 0 || (i == 4 && (j > 4 && j < 35)) || i == 8 || j == 0 || j == 5 ||
                j == 10 || j == 15 || j == 20 || j == 25 || j == 30 || j == 35 || j == 40)
                printf("%c", 178);
            else printf(" ");
        }
        textPosition.Y++;
        printf("\n");
    }

    gotoxy(28,14);
    for(i = 0; i < 6; i++) {
        printf("%d    ", i+1);
    }
    
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
    gotoxy(65,8);
    printf("User A move: ");

    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
    if (gametype == 1) {
        gotoxy(4,8);
        printf("User B move: ");
    } 
    else {
        gotoxy(2,8);
        printf("Computer move: ");
    }

    dropStones(desk);
}

void setStonesPosition(int pool) { // sets stones position according to coordinates

    COORD stonePosition = {27,9};

    if (pool == 6 || pool == 13)
        stonePosition.Y = 7;
    else if (pool > 5)
        stonePosition.Y -= 4;

    switch (pool) {
        case 0:
        case 12: break;
        case 1:
        case 11: {
            stonePosition.X += 5;
            break;
        }
        case 2:
        case 10: {
            stonePosition.X += 10;
            break;
        }
        case 3:
        case 9: {
            stonePosition.X += 15;
            break;
        }
        case 4:
        case 8: {
            stonePosition.X += 20;
            break;
        }
        case 5:
        case 7: {
            stonePosition.X += 25;
            break;
        }
        case 6: {
            stonePosition.X += 30;
            break;
        }
        case 13: {
            stonePosition.X -= 5;
            break;
        }
    }
    SetConsoleCursorPosition(hCon, stonePosition);
}

int isEmptySide(int * desk, enum TURN action, int gameType) {
    int i;
    int j;
    char game;
    int fin = 0;
    int fin2 = 0;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for(i=0;i<6;i++) if(desk[i]) fin = 1;
    for(i=7;i<13;i++) if(desk[i]) fin2 = 1;

    if (!fin || !fin2) fin = 0;

    if (action == CALCULATE || fin) return fin;

    else {
        dropRemainStones(desk);
    }

    dropStones(desk);

    gotoxy(31,16);
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    if (desk[6] > desk[13]) printf("User A wins %d - %d", desk[6], desk[13]);
    else if (desk[6] < desk[13]) {
        if (gameType == 1)  printf("User B wins %d - %d", desk[13], desk[6]);
        else printf("Computer wins %d - %d", desk[13], desk[6]);
    } 
    else printf("The game is end in a draw %d - %d", desk[6], desk[13]);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    while (1) {
        gotoxy(31,18);
        printf("Play again? (y/n) ");
        game = getch();
        if (game == 'n' || game == 27) {
            exit(0);
        } else if (game == 'y') {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            return 0;
        } else {
            Error(5);
            continue;
        } 
    }
}

void dropRemainStones(int * desk) {
    int i = 0;
    int j = 0;
    for (i = 7, j = 0; i < 13 && j < 6; i++, j++) {
        desk[6] += desk[j]; desk[j] = 0;
        desk[13] += desk[i]; desk[i] = 0;
    }    
}

int evaluate(int * desk) {
    int Kc = 0;
    int Kp = 0;
    int i = 0;

    for (i = 0; i < 6; i++) Kp += desk[i];
    for (i = 7; i < 13; i++) Kc += desk[i];

    return ((desk[13] - desk[6]) * 100 + Kc - Kp);
	//return (desk[13] - desk[6]);
}

int bestTurn(int * desk) {
    int move = 0;
    int j = 0;
    int k = 0;
    int score;
    int best = -infinity;
    int virtualDesk[14];
    int bestMove = 0;
    
    for (move = 12; move > 6; move--) {
        for (j = 0; j < 14; j++) virtualDesk[j] = desk[j]; // copy desk
        if (virtualDesk[move] > 0) {
            k = makeTurn(virtualDesk, (move+1), CALCULATE);
            if (!isEmptySide(virtualDesk, CALCULATE, 2)) {
                dropRemainStones(virtualDesk);
                score = evaluate(virtualDesk);
            } 
            else {
                if (k == 1)  {
                    score = alphaBeta(virtualDesk, maxDepth, B, -infinity, infinity);
                }
                else  {
                    score = alphaBeta(virtualDesk, maxDepth-1, A, -infinity, infinity);
                }
            }
            if (score > best) {
                best = score;
                bestMove = move + 1;
            }
        }
    }
    return (bestMove - 7);
}

int alphaBeta (int * desk, int curDepth, enum SIDE side, int alpha, int beta) {
    int i = 0;
    int j = 0;
    int k = 0;
    int move = 0;
    int score = 0;
    int virtualDesk[14];
    int best = -infinity;
    
    if (curDepth==0) return evaluate(desk);

    if (side == B) {
        for (move = 12; move > 6; move--) {
            for (i = 0; i < 14; i++) virtualDesk[i] = desk[i]; // copy desk
            if (virtualDesk[move] > 0) {
                k = makeTurn(virtualDesk, (move+1), CALCULATE);
                if (!isEmptySide(virtualDesk, CALCULATE, 2)) {
                    dropRemainStones(virtualDesk);
                    score = evaluate(virtualDesk);
                } 
                else {
                    if (k == 1) score = alphaBeta(virtualDesk, curDepth, side, alpha, beta);
                    else score = alphaBeta(virtualDesk, curDepth-1, A, alpha, beta);
                }
                if (score > best) {
                    best = score;
                    alpha = maxA(alpha, best);
                    if (alpha >= beta) return alpha;
                }
            }
        }
    }

    else if (side == A) {
        best = infinity;
        for (move = 5; move >= 0; move--) {
            for (i = 0; i < 14; i++) virtualDesk[i] = desk[i]; // copy desk
            if (virtualDesk[move] > 0) {
                k = makeTurn(virtualDesk, (move+1), CALCULATE);
                if (!isEmptySide(virtualDesk, CALCULATE, 2)) {
                    dropRemainStones(virtualDesk);
                    score = evaluate(virtualDesk);
                }
                else {
                    if (k == 1) score = alphaBeta(virtualDesk, curDepth, side, alpha, beta);
                    else score = alphaBeta(virtualDesk, curDepth-1, B, alpha, beta);
                }
                if (score < best) {
                    best = score;
                    beta = minB(beta, best);
                    if (beta <= alpha) return beta;  
                }
            }
        }
    }
    return best;
}

int maxA(int alpha, int best) {
    if (alpha > best) return alpha;
    else return best;
}

int minB(int beta, int best) {
    if (beta < best) return beta;
    else return best;
}

void Error (int type) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    gotoxy(21,16);
    SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);

    switch(type) {
        case 1: {
            printf("Error: Wrong number. Press Enter to continue.");
            break;
        }
        case 2: {
            printf("Error: Cell is empty. Press Enter to continue.");
            break;
        }
        case 3: {
            printf("Error: Invalid command. Press Enter to continue.");
            break;
        }
        case 4: {
            printf("Error: Invalid level. Press Enter to continue.");
            break;
        }
        case 5: {
            printf("Error: Invalid command. Press Enter to continue.");
            break;
        }
        case 6: {
            printf("Error: Invalid first move. Press Enter to continue.");
            break;
        }
    }
    getch();
    gotoxy(21,16);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("                                                   ");
}


