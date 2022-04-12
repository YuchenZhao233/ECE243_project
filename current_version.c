/* This files provides address values that exist in the system */

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

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

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
	
volatile int pixel_buffer_start;
volatile int character_buffer_start; // global variable

//Pixel array of black chess
/*const unsigned short BLACK_GO[169]={
	0xeeef, 0xf6ef, 0xff50, 0xb54c, 0x5265, 0x18c1, 0x0840, 0x18a1, 0x5245, 0xb52b, 0xff50, 0xf70f, 0xeeef, 0xf6ef, 0xf710, 0x6b07, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x62c6, 0xf6ef, 0xf70f, 0xff50, 0x6b07, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5aa6, 0xff30, 0xb52b, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xaceb, 0x5245, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4a04, 0x10a1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18a1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x10a1, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18a1, 0x4a45, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x4a04, 0xb52b, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xaceb, 0xff50, 0x6b06, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5aa6, 0xff30, 0xf6ef, 0xf710, 0x6b07, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5ac6, 0xeeef, 0xf710, 0xeeef, 0xf6ef, 0xff50, 0xb52b, 0x5265, 0x2922, 0x18c1, 0x2922, 0x4a45, 0xad0b, 0xff30, 0xf710, 0xeeef
};*/

const unsigned short BLACK_GO[169]={
	0xc42b, 0xbc2b, 0xc42b, 0xa369, 0x6a66, 0x3964, 0x2924, 0x4185, 0x6a66, 0xa369, 0xbc0b, 0xbbea, 0xbc0a, 0xbc2b, 0xc44b, 0x82c8, 0x41e7, 0x1904, 0x1904, 0x2104, 0x1904, 0x18c3, 0x2904, 0x7287, 0xabaa, 0xabca, 0xc44b, 0x7ac7, 0x2966, 0x4a49, 0x2124, 0x2124, 0x2104, 0x2104, 0x2124, 0x2104, 0x18e3, 0x6a46, 0xa369, 0xabca, 0x2924, 0x2125, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2924, 0x82c8, 0x7287, 0x18e3, 0x2965, 0x2945, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x1904, 0x59e6, 0x49c5, 0x1904, 0x2945, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x3985, 0x3985, 0x2104, 0x2124, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2965, 0x3165, 0x3985, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x2944, 0x72a8, 0x4228, 0x2945, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x18e4, 0x49c5, 0xa369, 0x4a28, 0x39c6, 0x2965, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x20e3, 0x2124, 0x2104, 0x7267, 0xc44b, 0x7aa8, 0x1904, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x10c3, 0x49c6, 0x9b49, 0xbc0b, 0xc42b, 0x72a8, 0x2104, 0x3186, 0x2124, 0x2104, 0x2104, 0x1904, 0x2965, 0x5206, 0xa369, 0xabaa, 0xc42b, 0xbc2b, 0xc44b, 0x9b69, 0x49e6, 0x2124, 0x2104, 0x2104, 0x3985, 0x82e8, 0xbbea, 0xbbea, 0xbc0b
};

//Pixel array of white chess
const unsigned short WHITE_GO[169]={
	0xeeef, 0xf710, 0xeeef, 0xd60e, 0xce13, 0xce57, 0xce79, 0xce57, 0xce13, 0xd60e, 0xeeef, 0xf710, 0xeeef, 0xf710, 0xeeae, 0xc5f1, 0xe73c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe73c, 0xc5f1, 0xeeae, 0xf710, 0xeeef, 0xc5f1, 0xf7bf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf7bf, 0xc5f1, 0xeeef, 0xce0e, 0xe73c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe73c, 0xce0e, 0xce34, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce34, 0xce78, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce78, 0xd699, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd699, 0xce78, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce78, 0xce34, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce34, 0xce0f, 0xef5d, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xef5d, 0xce0f, 0xeece, 0xc5f2, 0xffdf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffdf, 0xc5f2, 0xeece, 0xf710, 0xe6ae, 0xc5f2, 0xef5e, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xef5e, 0xc5f2, 0xe6ae, 0xf710, 0xeeef, 0xf710, 0xeece, 0xce0f, 0xce35, 0xe6fa, 0xdefa, 0xe6fa, 0xce35, 0xce0f, 0xeece, 0xf710, 0xeeef
};

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	volatile int * char_ctrl_ptr = (int *)0xFF203030;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
	character_buffer_start = *char_ctrl_ptr;
   	plot_whole_board();
}

void clear_screen()
{
    int y, x;
    for (x = 0; x < 320; x++)
        for (y = 0; y < 240; y++)
            plot_pixel (x, y, 0);
}

void plot_whole_board()
{
    int y, x;
	//plot the white background
	
	for (x = 0; x < 320; x++)
        for (y = 0; y < 240; y++)
            plot_pixel (x, y, 0);
			
	
	//plot the board
    for (x = 30; x < 260; x++)
        for (y = 1; y < 227; y++)
            plot_pixel (x, y, 0xBCAD);
	
	//draw horizontal lines
	int i = 18;
	while(i < 216){
		draw_line(50, i, 246, i, 0);
		i = i + 14;
	}
	
	//draw vertical lines
	i = 14;
	while(i < 211){
		draw_line(36 + i, 18, 36+i, 213, 0);
		i = i + 14;
	}
	place_go(7, 8, 0);
	place_go(1, 9, 1);
	place_go(3, 4, 1);
	place_go(3, 5, 1);
	plot_go(58, 54, 0);
	plot_go(58, 68, 1);
	
	draw_player_info();
	plot_board_coordinates();
}


// Plot x, y coordinates (1-15) of the board
void plot_board_coordinates(){
	int i;
	//draw horizontal 1
	for(i = 0; i < 7; ++i){
		plot_pixel(47 + i, 10, 0);
		plot_pixel(50, 1 + i+2, 0);
	}
	for(i = 0; i < 4; ++i){
		plot_pixel(47 + i, 5 - i, 0);
	}
	
	//draw vertical 1
	for(i = 0; i < 7; ++i){
		plot_pixel(35 + i, 23, 0);
		plot_pixel(38, 1 + 15 + i, 0);
	}
	for(i = 0; i < 4; ++i){
		plot_pixel(35 + i, 18 - i, 0);
	}
	
	//draw horizontal 2
	for(i = 0; i < 7; ++i){
		plot_pixel(61+i, 2, 0);
		plot_pixel(61+i, 6, 0);
		plot_pixel(61+i, 10, 0);
	}
	for(i = 0; i < 3; ++i){
		plot_pixel(67, i+3, 0);
		plot_pixel(61, i+7, 0);
	}
	
	//draw vertical 2
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 28, 0);
		plot_pixel(35+i, 32, 0);
		plot_pixel(35+i, 36, 0);
	}
	for(i = 0; i < 3; ++i){
		plot_pixel(41, i+29, 0);
		plot_pixel(35, i+33, 0);
	}
	
	//draw horizontal 3
	for(i = 0; i < 7; ++i){
		plot_pixel(75+i, 2, 0);
		plot_pixel(75+i, 6, 0);
		plot_pixel(75+i, 10, 0);
		plot_pixel(81, 3 + i, 0);
	}
	
	//draw horizontal 4
	for(i = 0; i < 7; ++i){
		plot_pixel(89+i, 6, 0);
	}
	for(i = 0; i < 4; ++i){
		plot_pixel(89, 5-i, 0);
	}
	for(i = 0; i < 9; ++i){
		plot_pixel(92, 2+i, 0);
	}
	
	//draw vertical 3
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 42, 0);
		plot_pixel(35+i, 46, 0);
		plot_pixel(35+i, 50, 0);
		plot_pixel(41, 43 + i, 0);
	}
	
	//draw horizontal 5
	for(i = 0; i < 7; ++i){
		plot_pixel(103+i, 2, 0);
		plot_pixel(103+i, 6, 0);
		plot_pixel(103+i, 10, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(103, 3 + i, 0);
		plot_pixel(109, 7 + i, 0);
	}
	
	//draw horizontal 6
	for(i = 0; i < 7; ++i){
		plot_pixel(117+i, 2, 0);
		plot_pixel(117+i, 6, 0);
		plot_pixel(117+i, 10, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(117, 3 + i, 0);
		plot_pixel(117, 7 + i, 0);
		plot_pixel(123, 7 + i, 0);
	}
	
	//draw horizontal 7
	for(i = 0; i < 7; ++i){
		plot_pixel(131+i, 2, 0);
	}
	
	for(i = 0; i < 8; ++i){
		plot_pixel(137, 3 + i, 0);
	}
	
	//draw horizontal 8
	for(i = 0; i < 7; ++i){
		plot_pixel(145+i, 2, 0);
		plot_pixel(145+i, 6, 0);
		plot_pixel(145+i, 10, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(145, 3 + i, 0);
		plot_pixel(151, 3 + i, 0);
		plot_pixel(145, 7 + i, 0);
		plot_pixel(151, 7 + i, 0);
	}
	
	//draw horizontal 9
	for(i = 0; i < 7; ++i){
		plot_pixel(159+i, 2, 0);
		plot_pixel(159+i, 6, 0);
		plot_pixel(159+i, 10, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(159, 3 + i, 0);
		plot_pixel(165, 3 + i, 0);
		plot_pixel(165, 7 + i, 0);
	}
	
	//draw horizontal 10
	for(i = 0; i < 9; ++i){
		plot_pixel(173, 2+i, 0);
		plot_pixel(176, 2+i, 0);
		plot_pixel(179, 2+i, 0);
	}
	
	for(i = 0; i < 2; ++i){
		plot_pixel(177+i, 2, 0);
		plot_pixel(177+i, 10, 0);
	}
	
	//draw vertical 10
	for(i = 0; i < 9; ++i){
		plot_pixel(35, 140+i, 0);
		plot_pixel(38, 140+i, 0);
		plot_pixel(41, 140+i, 0);
	}
	
	for(i = 0; i < 2; ++i){
		plot_pixel(39+i, 140, 0);
		plot_pixel(39+i, 148, 0);
	}
	
	//draw horizontal 11
	for(i = 0; i < 9; ++i){
		plot_pixel(187, 2+i, 0);
		plot_pixel(192, 2+i, 0);
	}
	
	//draw vertical 11
	for(i = 0; i < 9; ++i){
		plot_pixel(35, 154+i, 0);
		plot_pixel(40, 154+i, 0);
	}
	
	//draw horizontal 12
	for(i = 0; i < 9; ++i){
		plot_pixel(201, 2+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(204+i, 2, 0);
		plot_pixel(204+i, 6, 0);
		plot_pixel(204+i, 10, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(204, 7+i, 0);
		plot_pixel(207, 3+i, 0);
	}
	
	//draw vertical 12
	for(i = 0; i < 9; ++i){
		plot_pixel(35, 168+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(38+i, 168, 0);
		plot_pixel(38+i, 172, 0);
		plot_pixel(38+i, 176, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(38, 173+i, 0);
		plot_pixel(41, 169+i, 0);
	}
	
	//draw horizontal 13
	for(i = 0; i < 9; ++i){
		plot_pixel(215, 2+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(218+i, 2, 0);
		plot_pixel(218+i, 6, 0);
		plot_pixel(218+i, 10, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(221, 7+i, 0);
		plot_pixel(221, 3+i, 0);
	}
	
	//draw vertical 13
	for(i = 0; i < 9; ++i){
		plot_pixel(35, 182+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(38+i, 182, 0);
		plot_pixel(38+i, 186, 0);
		plot_pixel(38+i, 190, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(41, 187+i, 0);
		plot_pixel(41, 183+i, 0);
	}
	
	//draw horizontal 14
	for(i = 0; i < 9; ++i){
		plot_pixel(229, 2+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(232+i, 6, 0);
		plot_pixel(234, 2+i, 0);
		plot_pixel(232, 2+i, 0);
		plot_pixel(234, 7+i, 0);
	}
	
	//draw vertical 14
	for(i = 0; i < 9; ++i){
		plot_pixel(35, 196+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(38+i, 200, 0);
		plot_pixel(40, 196+i, 0);
		plot_pixel(38, 196+i, 0);
		plot_pixel(40, 201+i, 0);
	}
	
	//draw horizontal 15
	for(i = 0; i < 9; ++i){
		plot_pixel(243, 2+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(246+i, 2, 0);
		plot_pixel(246+i, 6, 0);
		plot_pixel(246+i, 10, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(249, 7+i, 0);
		plot_pixel(246, 3+i, 0);
	}
	
	//draw vertical 15
	for(i = 0; i < 9; ++i){
		plot_pixel(35, 210+i, 0);
	}
	
	for(i = 0; i < 4; ++i){
		plot_pixel(38+i, 210, 0);
		plot_pixel(38+i, 214, 0);
		plot_pixel(38+i, 218, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(41, 215+i, 0);
		plot_pixel(38, 211+i, 0);
	}
	
	//draw vertical 4
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 60, 0);
	}
	for(i = 0; i < 4; ++i){
		plot_pixel(35, 59-i, 0);
	}
	for(i = 0; i < 9; ++i){
		plot_pixel(38, 56+i, 0);
	}
	
	//draw vertical 5
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 70, 0);
		plot_pixel(35+i, 74, 0);
		plot_pixel(35+i, 78, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(35, 71 + i, 0);
		plot_pixel(41, 75 + i, 0);
	}
	
	//draw vertical 6
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 84, 0);
		plot_pixel(35+i, 88, 0);
		plot_pixel(35+i, 92, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(35, 85 + i, 0);
		plot_pixel(35, 89 + i, 0);
		plot_pixel(41, 89 + i, 0);
	}
	
	//draw vertical 7
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 99, 0);
	}
	
	for(i = 0; i < 8; ++i){
		plot_pixel(41, 99 + i, 0);
	}
	
	//draw vertical 8
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 112, 0);
		plot_pixel(35+i, 116, 0);
		plot_pixel(35+i, 120, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(35, 113 + i, 0);
		plot_pixel(41, 113 + i, 0);
		plot_pixel(35, 117 + i, 0);
		plot_pixel(41, 117 + i, 0);
	}
	
	//draw vertical 9
	for(i = 0; i < 7; ++i){
		plot_pixel(35+i, 126, 0);
		plot_pixel(35+i, 130, 0);
		plot_pixel(35+i, 134, 0);
	}
	
	for(i = 0; i < 3; ++i){
		plot_pixel(35, 127 + i, 0);
		plot_pixel(41, 127 + i, 0);
		plot_pixel(41, 131 + i, 0);
	}
}

// plot a stone at a given position with given color
void plot_go(int x, int y, int color){
	int i, j;
	if(color == 0){
		for(i = 0; i < 13; ++i){
			for(j = 0; j < 13; ++j){
				plot_pixel(x + i, y + j, BLACK_GO[i*13+j]);
			}
		}
	}
	if(color == 1){
		for(i = 0; i < 13; ++i){
			for(j = 0; j < 13; ++j){
				plot_pixel(x + i, y + j, WHITE_GO[i*13+j]);
			}
		}
	}
}

// Place a stone at given x, y coordinate with given color
void place_go(int x_coord, int y_coord, int color){
	plot_go(44 + (x_coord - 1) * 14, 12 + (y_coord - 1) * 14, color);
}

// Draw all player info on the board
void draw_player_info(){
	
	//draw player 1
	draw_text(67, 10, 'P');
	draw_text(68, 10, 'l');
	draw_text(69, 10, 'a');
	draw_text(70, 10, 'y');
	draw_text(71, 10, 'e');
	draw_text(72, 10, 'r');
	draw_text(74, 10, '1');
	draw_text(75, 10, ':');
	
	//draw black stones
	draw_text(67, 13, 'B');
	draw_text(68, 13, 'l');
	draw_text(69, 13, 'a');
	draw_text(70, 13, 'c');
	draw_text(71, 13, 'k');
	draw_text(73, 13, 's');
	draw_text(74, 13, 't');
	draw_text(75, 13, 'o');
	draw_text(76, 13, 'n');
	draw_text(77, 13, 'e');
	draw_text(78, 13, 's');
	
	//draw player 2
	draw_text(67, 20, 'P');
	draw_text(68, 20, 'l');
	draw_text(69, 20, 'a');
	draw_text(70, 20, 'y');
	draw_text(71, 20, 'e');
	draw_text(72, 20, 'r');
	draw_text(74, 20, '2');
	draw_text(75, 20, ':');
	
	//draw white stones
	draw_text(67, 23, 'W');
	draw_text(68, 23, 'h');
	draw_text(69, 23, 'i');
	draw_text(70, 23, 't');
	draw_text(71, 23, 'e');
	draw_text(73, 23, 's');
	draw_text(74, 23, 't');
	draw_text(75, 23, 'o');
	draw_text(76, 23, 'n');
	draw_text(77, 23, 'e');
	draw_text(78, 23, 's');
	
	//draw status
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void draw_text(int x, int y, char c)
{
    *(char *)(character_buffer_start + (y << 7) + x) = c;
}


void swap(int* x, int* y){
	int temp = *x;
	*x = *y;
	*y = temp;
}

void draw_line(int x0, int y0, int x1, int y1, short int line_color){
	bool is_steep = abs(y1 - y0) > abs(x1 - x0);
	if(is_steep){
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if(x0 > x1){
		swap(&x0, &x1);
		swap(&y0, &y1);
	}
	
	int deltax = x1 - x0;
	int deltay = abs(y1 - y0);
	int error = -(deltax / 2);
	int y = y0;
	int y_step = 0;
	if(y0 < y1){
		y_step = 1;
	}
	else{
		y_step = -1;
	}
	int x;
	for(x = x0; x < x1 + 1; x++){
		if(is_steep){
			plot_pixel(y, x, line_color);
		}
		else{
			plot_pixel(x, y, line_color);
		}
		error = error + deltay;
		if(error > 0){
			y = y + y_step;
			error = error - deltax;
		}	
	}
}
	
	