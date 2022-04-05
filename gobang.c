#include "address_map_arm.h"

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

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


volatile int pixel_buffer_start; // global variable

// Functions for drawing on the VGA Display
void plot_pixel(int, int, short int);
void draw_line(int, int, int, int, int);
void clear_screen();
void swap(int*, int*);
void wait_for_vsync();

int main(void){
    



    return 0;
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