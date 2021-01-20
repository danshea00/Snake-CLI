#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#include "input.h"

#define BOARD_X 20
#define BOARD_Y 10

typedef enum direction Direction;
typedef struct apple Apple;
typedef struct snake Snake;

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct apple {
    int x;
    int y;
};

struct snake {
    int x;
    int y;
    Direction d;
    Snake *next;
};

void printBoard(char board[BOARD_Y][BOARD_X]);
Snake *updateBoard(char board[BOARD_Y][BOARD_X], Snake *s, Apple *a);
void initBoard(char board[BOARD_Y][BOARD_X]);
bool isWall(int y, int x);
void gameOver(Snake *s, Apple *a);
void freeSnake(Snake *head);
Snake *updateSnake(Snake *s, bool appleEaten);

int main(void) {

    char board[BOARD_Y][BOARD_X];
    
    int c;
    kbsetup();
    srand(time(0));

    Apple *a = malloc(sizeof(Apple));
    
    a->x = rand() % (BOARD_X - 1);
    if (a->x == 0) a->x++;
    a->y = rand() % (BOARD_Y - 1);
    if (a->y == 0) a->y++;

    Snake *s = malloc(sizeof(Snake));

    s->x = rand() % (BOARD_X - 3);
    if (s->x == 0) s->x++;
    s->y = rand() % (BOARD_Y - 3);
    if (s->x == 0) s->y++;

    s->d = rand() % 4;
    s->next = NULL;

    
    while(true) {
        usleep(600000);
        if ((c = getkey()) != '\0') {
            if ( c == 'q' || c == 'Q' ) gameOver(s, a);
            else if ( c =='w' || c == 'W') s->d = UP;
            else if ( c =='s' || c == 'S') s->d = DOWN;
            else if ( c =='a' || c == 'A') s->d = LEFT;
            else if ( c =='d' || c == 'D') s->d = RIGHT;
        }

        s = updateBoard(board, s, a);
        printBoard(board);

    }
}

void initBoard(char board[BOARD_Y][BOARD_X]) {
    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            if (isWall(i, j)) {
                board[i][j] = '#';
            } else {
                board[i][j] = ' ';
            }
        }
    }
}

void printBoard(char board[BOARD_Y][BOARD_X]) {
    // Clear terminal
    printf("\e[1;1H\e[2J");

    for (int i = 0; i < BOARD_Y; i++) {
        for (int j = 0; j < BOARD_X; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

Snake *updateBoard(char board[BOARD_Y][BOARD_X], Snake *s, Apple *a) {


    bool appleEaten = false;

    initBoard(board);


    // Check for apple
    if (s->x == a->x && s->y == a->y) {
        a->x = rand() % (BOARD_X - 1);
        if (a->x == 0) a->x++;
        a->y = rand() % (BOARD_Y - 1);
        if (a->y == 0) a->y++;
        appleEaten = true;
    }

    s = updateSnake(s, appleEaten);

    // Draw apple
    board[a->y][a->x] = '*';

    // Draw snake

    Snake *curr = s->next;
    while (curr != NULL) {
        switch (curr->d) {
            case (UP):
                board[curr->y][curr->x] = '|';
                break;
            case (DOWN):
                board[curr->y][curr->x] = '|';
                break;
            case (LEFT):
                board[curr->y][curr->x] = '-';
                break;
            case (RIGHT):
                board[curr->y][curr->x] = '-';
                break;
        }
        curr = curr->next;
    }

    switch (s->d) {
        case (UP):
            board[s->y][s->x] = '^';
            break;
        case (DOWN):
            board[s->y][s->x] = 'v';
            break;
        case (LEFT):
            board[s->y][s->x] = '<';
            break;
        case (RIGHT):
            board[s->y][s->x] = '>';
            break;
    }

    curr = s->next;
    while (curr != NULL) {
        if (s->x == curr->x && s->y == curr->y) gameOver(s, a);
        curr = curr->next;
    }

    // Check for collisions
    if (isWall(s->y, s->x)) {
        gameOver(s, a);
    }

    return s;

}

bool isWall(int y, int x) {
    return y == 0 || y == BOARD_Y - 1 || x == 0 || x == BOARD_X - 1;
}

void gameOver(Snake *s, Apple *a) {
    freeSnake(s);
    free(a);
    exit(0);
}

void freeSnake(Snake *head) {
    Snake *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

Snake *updateSnake(Snake *s, bool appleEaten) {
    Snake *new = malloc(sizeof(Snake));
    new->next = NULL;
    new->y = s->y;
    new->x = s->x;
    new->d = s->d;
    switch (s->d) {
        case (UP):
            new->y--;
            break;
        case (DOWN):
            new->y++;
            break;
        case (LEFT):
            new->x--;
            break;
        case (RIGHT):
            new->x++;
            break;
    }
    new->next = s;
    s = new;
    // Remove last segment
    if (!appleEaten) {
        Snake *curr = s;
        Snake *prev = NULL;
        while(curr->next != NULL) {
            prev = curr;
            curr = curr->next;
        }
        free(curr);
        prev->next = NULL;
    }
    return s;
}