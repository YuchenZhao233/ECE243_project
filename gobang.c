#include "address_map_arm.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00
#define BLACK 0x0000

// Absolute value
#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

#define FALSE 0
#define TRUE 1

// Define board size
#define ROW 10
#define COL 10

// Define eight directional macros
#define UP  20
#define DOWN 21
#define RIGHT 22
#define LEFT 23
#define RIGHT_UP 24
#define RIGHT_DOWN 25
#define LEFT_UP 26
#define LEFT_DOWN 27

// Define game state macros
#define DRAW 3
#define CONTINUE 4
#define END 5
#define ERROR 6
#define VALID 7

// Global Variable
volatile int pixel_buffer_start; 
volatile int chess_board[ROW][COL];  // 0 for empty, 1 for blue piece, 2 for white piece
volatile int x_position;
volatile int y_position;

// Functions for drawing on the VGA Display
void plot_pixel(int, int, short int);
void draw_line(int, int, int, int, int);
void clear_screen();
void swap(int*, int*);
void wait_for_vsync();

// Game logic functions
int check_move_legality(int x, int y);
void init_board();
void draw_cmd_board();
int game_state();
int calculate_pieces(int direction);




// Some AI function possibly

int main(void){
    



    return 0;
}

// Check the validity of player's move. Return ERROR for not legal move; return valid for legal move
int check_move_legality(int x, int y){
    if (x < 0 || x > ROW - 1 || y < 0 || y > COL - 1)
        return ERROR;
    
    if (chess_board[x][y] != 0)
        return ERROR;

    return VALID;
}

// Initialize the board to empty, 0 means empty; also reset the current position of chess to (0, 0)
void init_board(){
    x_position = 0;
    y_position = 0;
    int i = 0;
    for (; i < ROW; i++){
        int j = 0;
        for (; j < COL; j++){
            chess_board[i][j] = 0;
        }
    }
}

// Draw command line board, just for testing game logic purpose, do not use it for actual the actual game
void draw_cmd_board(){
    printf(" ");
    int i = 0;
    for (; i < ROW; i++){
        printf(" %d ", i);
    }
    printf("\n");
    i = 0;
    for (; i < ROW; i++){
        printf("%d", i);
        int j = 0;
        for (; j < COL; j++){
            if (chess_board[i][j] == 0){
                printf(" . ");
            }
            else{
                printf(" %d ", chess_board[i][j]);
            }
        }
        printf("\n");
    }   
}

// Calculate the number of pieces in the given direction
int calculate_pieces(int direction){
    int new_x = x_position, new_y = y_position, count = 0;
    while(TRUE){
        if (direction == UP)
            new_x--;
        else if (direction == DOWN)
            new_x++;
        else if (direction == LEFT)
            new_y--;
        else if (direction == RIGHT)
            new_y++;
        else if (direction == LEFT_UP){
            new_x--;
            new_y--;
        }
        else if (direction == LEFT_DOWN){
            new_x++;
            new_y--;
        }
        else if (direction == RIGHT_UP){
            new_x--;
            new_y++;
        }
        else if (direction == RIGHT_DOWN){
            new_x++;
            new_y--;
        }

        if (new_x < 0 || new_x > ROW - 1 || new_y < 0 || new_y > COL - 1){
            break;
        }
        else {
            if (chess_board[x_position][y_position] == chess_board[new_x][new_y])
                count++;
            else
                break;
        }
    
    }

    return count;
}


void wait_for_vsync(){
    volatile int * pixel_ctrl_ptr = (int *) 0xFF203020;
    int status;

    * pixel_ctrl_ptr = 1;
    status = *(pixel_ctrl_ptr + 3);
    // Wait Until The S Bit In The Status Register Turns Into Zero
    while ((status & 0x00000001) != 0){
        status = *(pixel_ctrl_ptr + 3);
    }
}

// code not shown for clear_screen() and draw_line() subroutines

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void draw_line(int x0, int y0, int x1, int y1, int color){
    bool is_steep = ABS(y1 - y0) > ABS(x1 - x0);
    if (is_steep){
        swap(&x0, &y0);
        swap(&x1, &y1);
    }

    if (x0 > x1){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }
    
    int deltax = x1 - x0;
    int deltay = ABS(y1 - y0);
    int error = -(deltax / 2);
    int y = y0;
    int y_step;

    if (y0 < y1)
        y_step = 1;
    else
        y_step = -1;

    int x = x0;
    for (x = x0; x <= x1; x++){
        if (is_steep)
            plot_pixel(y, x, color);
        else
            plot_pixel(x, y, color);

        error = error + deltay;
        if (error > 0){
            y = y + y_step;
            error = error - deltax;
        }
    }
}

void clear_screen(){
    int x = 0;
    int y = 0;
    for (x = 0; x < RESOLUTION_X; x++) {
        for (y = 0; y < RESOLUTION_Y; y++) {
            plot_pixel(x, y, BLACK);
        }
    }
}

void swap(int* num1, int* num2){
    int temp = 0;
    temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}