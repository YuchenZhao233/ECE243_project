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

#define BLACK_STONE 0
#define WHITE_STONE 1
#define BLACK_HIGHLIGHT_STONE 2
#define WHITE_HIGHLIGHT_STONE 3

#define TIE 3
#define CONTINUE 4
#define INVALID 5
#define BLACK_WON 6
#define WHITE_WON 7
#define START 8

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
	0xe5f0, 0xd56f, 0xe5f3, 0xacb0, 0x49a6, 0x1042, 0x1022, 0x1883, 0x49a6, 0xa44f, 0xddf3, 0xd56f, 0xe5cf, 0xd56f, 0xddf2, 0x72c7, 0x1800, 0x1000, 0x0800, 0x0000, 0x0000, 0x1000, 0x1000, 0x6245, 0xd590, 0xe5af, 0xe634, 0x72c7, 0x1000, 0x0800, 0x0820, 0x0000, 0x0000, 0x0841, 0x0800, 0x0800, 0x1800, 0x72a6, 0xcd30, 0xac90, 0x1000, 0x0800, 0x1060, 0x0000, 0x0860, 0x0000, 0x0000, 0x0000, 0x1040, 0x1000, 0x1000, 0x9bed, 0x49e7, 0x0800, 0x0840, 0x0000, 0x0020, 0x0080, 0x0020, 0x0020, 0x0020, 0x0000, 0x0800, 0x0800, 0x4185, 0x1883, 0x0821, 0x0000, 0x0020, 0x0080, 0x0020, 0x0020, 0x0080, 0x0061, 0x0000, 0x0000, 0x0801, 0x0800, 0x0001, 0x0001, 0x0000, 0x0020, 0x0020, 0x0020, 0x0080, 0x0020, 0x0000, 0x0021, 0x0001, 0x0001, 0x0000, 0x1863, 0x0000, 0x0000, 0x0020, 0x0040, 0x0020, 0x0020, 0x0020, 0x0021, 0x0001, 0x0802, 0x0000, 0x0000, 0x49c6, 0x1040, 0x0000, 0x0000, 0x0040, 0x0061, 0x0000, 0x0042, 0x0001, 0x0001, 0x0800, 0x0800, 0x3123, 0xa48f, 0x0800, 0x0800, 0x1040, 0x0000, 0x0000, 0x0022, 0x0001, 0x0001, 0x0801, 0x0800, 0x0800, 0xa4af, 0xd5b2, 0x6a86, 0x2060, 0x0800, 0x0000, 0x0801, 0x0822, 0x0001, 0x1842, 0x0800, 0x1820, 0x5a25, 0xd5d2, 0xddb0, 0xd590, 0x6a66, 0x1000, 0x0800, 0x0800, 0x0001, 0x0801, 0x0800, 0x1000, 0x5a25, 0xddf2, 0xd58f, 0xddae, 0xee11, 0xd591, 0x9c0d, 0x49c6, 0x1021, 0x0800, 0x0800, 0x5206, 0x9c4d, 0xde13, 0xe5f1, 0xd56e
};		

//Pixel array of white chess
const unsigned short WHITE_GO[169]={
	0xe5cc, 0xe5cd, 0xddd0, 0x9c0d, 0x9c30, 0xa4b4, 0xa4d5, 0xa4d4, 0x9430, 0xa46f, 0xcd91, 0xddb0, 0xedf0, 0xe5ce, 0xc4eb, 0x9bea, 0xde56, 0xffdf, 0xf77f, 0xffdf, 0xf75e, 0xff9d, 0xcdf5, 0xa44c, 0xc4ee, 0xe5d0, 0xddd1, 0xa44b, 0xf718, 0xfffd, 0xffff, 0xffff, 0xe73d, 0xffff, 0xfffe, 0xfffd, 0xe696, 0x9c0b, 0xcd2f, 0x9c2d, 0xcdb4, 0xfffd, 0xfffe, 0xffdf, 0xffff, 0xffff, 0xffff, 0xf7bd, 0xfffd, 0xfffc, 0xcdb3, 0xb4ef, 0x9451, 0xffff, 0xffff, 0xffff, 0xffff, 0xefbe, 0xf7ff, 0xefbd, 0xffff, 0xfffe, 0xff9c, 0xffbc, 0x9c2e, 0x9cb4, 0xffff, 0xffff, 0xf7df, 0xf7ff, 0xf7ff, 0xeffe, 0xf7ff, 0xffff, 0xffff, 0xfffe, 0xfffe, 0x9c4f, 0xb598, 0xffff, 0xf7bf, 0xf7ff, 0xf7ff, 0xf7ff, 0xeffe, 0xf7ff, 0xffff, 0xf79e, 0xffff, 0xffff, 0xbd74, 0x9cd4, 0xf7bf, 0xffff, 0xf7ff, 0xefde, 0xf7ff, 0xf7ff, 0xffff, 0xffff, 0xffff, 0xff9f, 0xffdf, 0xacf2, 0x9450, 0xffff, 0xfffe, 0xf7dd, 0xffff, 0xefde, 0xf7de, 0xffff, 0xffff, 0xff7f, 0xffdf, 0xff9e, 0x93ed, 0x9c6f, 0xd656, 0xfffd, 0xfffd, 0xfffe, 0xffff, 0xffdf, 0xffbf, 0xff9f, 0xffdf, 0xff9e, 0xc554, 0xb4d0, 0xcdb1, 0x93ea, 0xf738, 0xfffc, 0xef3a, 0xfffe, 0xffbe, 0xffdf, 0xffdf, 0xffbf, 0xe658, 0x9bed, 0xddb2, 0xddd0, 0xcd4f, 0x93ea, 0xcdd3, 0xfffd, 0xfffe, 0xffdf, 0xffbe, 0xff7d, 0xc513, 0xac2e, 0xcd30, 0xe5f1, 0xddae, 0xe5f0, 0xcd70, 0xac8d, 0x940d, 0xacb1, 0xbd54, 0xacf2, 0x9c4f, 0xb4af, 0xd571, 0xe5d1, 0xcccc
};

const unsigned short BLACK_HIGHLIGHT_GO[196] = {
	0xe570, 0xe5ad, 0xddc7, 0xde04, 0xf726, 0xffa8, 0xef48, 0xef48, 0xffca, 0xe6c8, 0xe689, 0xd5a8, 0xd54a, 0xfe30, 0xe5af, 0xe5ac, 0xf6ac, 0xff0b, 0xbd44, 0x5240, 0x3980, 0x3160, 0x5240, 0xace4, 0xf72d, 0xf6ad, 0xe5ec, 0xd52a, 0xe60c, 0xe62c, 0xe64c, 0x6240, 0x1000, 0x1000, 0x0800, 0x0800, 0x1020, 0x1040, 0x49e0, 0xe68d, 0xf6ae, 0xddcb, 0xd608, 0xeeac, 0x62a0, 0x1840, 0x1000, 0x0802, 0x0806, 0x0004, 0x0001, 0x0800, 0x0820, 0x41c0, 0xff0f, 0xe62b, 0xf78b, 0xad46, 0x0820, 0x0800, 0x0800, 0x0002, 0x0003, 0x0023, 0x0001, 0x1082, 0x1880, 0x0820, 0xa4c6, 0xf70d, 0xe74a, 0x4220, 0x1080, 0x0820, 0x0000, 0x18e6, 0x0002, 0x0002, 0x0043, 0x0000, 0x0000, 0x18a0, 0x41c0, 0xffae, 0xe76d, 0x2980, 0x0020, 0x10a0, 0x0000, 0x0001, 0x0021, 0x0042, 0x0002, 0x0042, 0x0860, 0x0820, 0x2920, 0xe6eb, 0xdf0b, 0x2980, 0x0840, 0x0000, 0x0041, 0x0001, 0x0020, 0x0020, 0x0001, 0x0000, 0x0000, 0x1040, 0x2920, 0xef0c, 0xefab, 0x3a00, 0x10a0, 0x0000, 0x0020, 0x0000, 0x0020, 0x0040, 0x0000, 0x0000, 0x0820, 0x0800, 0x3980, 0xf74e, 0xe709, 0x9cc3, 0x1060, 0x0820, 0x0000, 0x0860, 0x0000, 0x0000, 0x0000, 0x0000, 0x0800, 0x1000, 0xaca6, 0xf6ec, 0xe68b, 0xe6ad, 0x3980, 0x0800, 0x1060, 0x0000, 0x0802, 0x0802, 0x0801, 0x1020, 0x0800, 0x3940, 0xeeac, 0xe64a, 0xd5cb, 0xee8e, 0xeeae, 0x4180, 0x1020, 0x0800, 0x0800, 0x0800, 0x0800, 0x1860, 0x3100, 0xe66b, 0xfeed, 0xe62a, 0xddcd, 0xddab, 0xee8d, 0xeeed, 0xa4a4, 0x4a20, 0x20c0, 0x2100, 0x39a0, 0x8c24, 0xf6ee, 0xf6cd, 0xddca, 0xcd08, 0xedef, 0xddac, 0xddc9, 0xe689, 0xef2a, 0xffee, 0xdeeb, 0xf76d, 0xff8e, 0xef2d, 0xd609, 0xddea, 0xdd8a, 0xf62d
};

const unsigned short WHITE_HIGHLIGHT_GO[196] = {
	0xed90, 0xe58d, 0xe5c9, 0xe647, 0xeee5, 0xf766, 0xf787, 0xf789, 0xf74a, 0xe6ca, 0xde29, 0xddc9, 0xe5ab, 0xedcc, 0xe58d, 0xe5cd, 0xee6c, 0xff0d, 0xffae, 0xffef, 0xfff1, 0xfff2, 0xfff2, 0xff91, 0xf6ee, 0xe64c, 0xe5eb, 0xe5ab, 0xdde9, 0xee6c, 0xff31, 0xffb6, 0xfff9, 0xfffc, 0xfffd, 0xfffd, 0xfffc, 0xfff9, 0xffd4, 0xff2f, 0xee4c, 0xddca, 0xde88, 0xf72d, 0xffd5, 0xfffa, 0xffde, 0xffbf, 0xffbf, 0xffbf, 0xffdf, 0xfffd, 0xfff8, 0xffd3, 0xf70d, 0xe649, 0xe709, 0xffaf, 0xfff7, 0xfffb, 0xfffe, 0xffff, 0xffff, 0xffff, 0xffdf, 0xffff, 0xfffb, 0xfff6, 0xff8e, 0xeee9, 0xef8b, 0xfff1, 0xfff8, 0xfffd, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xfffd, 0xfff8, 0xfff0, 0xf76a, 0xf7ed, 0xfff2, 0xfffb, 0xffff, 0xf7df, 0xf7ff, 0xf7ff, 0xf7ff, 0xf7ff, 0xffff, 0xfffe, 0xfff9, 0xfff2, 0xffad, 0xefcd, 0xfff3, 0xfffb, 0xffff, 0xffff, 0xffff, 0xf7ff, 0xf7fe, 0xf7ff, 0xf7ff, 0xfffe, 0xfffa, 0xfff3, 0xffae, 0xef6c, 0xfff2, 0xfff9, 0xfffc, 0xffde, 0xffff, 0xffff, 0xf7ff, 0xffff, 0xffff, 0xfffe, 0xfffa, 0xfff2, 0xf74c, 0xe6eb, 0xffb1, 0xfff7, 0xfffb, 0xfffd, 0xffff, 0xffff, 0xffdf, 0xffdf, 0xffde, 0xfffc, 0xfff8, 0xff91, 0xeecb, 0xde4b, 0xf710, 0xffd5, 0xfff9, 0xfffd, 0xffdf, 0xffdf, 0xffdf, 0xffdf, 0xfffc, 0xfff8, 0xffd4, 0xf6ef, 0xe62b, 0xddec, 0xe66e, 0xff31, 0xffd5, 0xfff9, 0xfffc, 0xffdf, 0xffde, 0xfffc, 0xfff9, 0xfff4, 0xff30, 0xe64d, 0xddcb, 0xe5af, 0xe5ed, 0xee8b, 0xff4d, 0xffd0, 0xfff3, 0xfff6, 0xfff6, 0xfff4, 0xffd2, 0xf72e, 0xe66b, 0xe5eb, 0xe5cc, 0xe5b0, 0xddad, 0xde08, 0xe6a7, 0xf749, 0xff8c, 0xffb0, 0xffb0, 0xf76e, 0xef0c, 0xde69, 0xde09, 0xddcb, 0xe5cd
};

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
	volatile int * char_ctrl_ptr = (int *)0xFF203030;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
	character_buffer_start = *char_ctrl_ptr;
	//clean_screen();
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
            plot_pixel (x, y, 0xED8E);
	
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
	place_go(1, 1, 2);
	place_go(2, 1, 2);
	place_go(3, 1, 2);
	place_go(1, 2, 3);
	
	draw_player_info();
	plot_board_coordinates();
}

void plot_initial_animation(){
	int y, x;
	//plot the background
	for (x = 0; x < 320; x++)
        for (y = 0; y < 240; y++)
            plot_pixel (x, y, 0xED8E);
	
	//draw horizontal lines
	
	int i = 18;
	while(i < 188){
		draw_line(22, i, 302, i, 0);
		i = i + 14;
	}
	
	//draw vertical lines
	i = 14;
	while(i < 300){
		draw_line(8 + i, 18, 8+i, 185, 0);
		i = i + 14;
	}
}

void plot_initial_stones(){
	for(int i = 1; i < 6; ++i){
		place_go(-1, i, 0);
		place_go(4, i, 1);
		place_go(7, i, 1);
		place_go(9, i, 0);
		place_go(11, i, 0);
		place_go(13, i, 0);
		place_go(15, i, 1);
		place_go(19, i, 1);
		place_go(4, 6 + i, 0);
		place_go(9, 6+i, 1);
		place_go(13, 6+i, 1);
	}
	place_go(1, 3, 0);
	for(int i = 0; i < 3; ++i){
		place_go(i, 1, 0);
		place_go(i, 5, 0);
		place_go(2, 3+i, 0);
		place_go(10+i, 1, 0);
		place_go(5+i, 9-i, 0);
		place_go(5+i, 9+i, 0);
		place_go(4+i, 1, 1);
		place_go(4+i, 5, 1);
		place_go(16+i, 1, 1);
		place_go(16+i, 5, 1);
		place_go(10+i, 11, 1);
	}
}

// draw creators' names and start message
void draw_creator_info(){
	draw_text(28, 50, 'C');
	draw_text(29, 50, 'r');
	draw_text(30, 50, 'e');
	draw_text(31, 50, 'a');
	draw_text(32, 50, 't');
	draw_text(33, 50, 'e');
	draw_text(34, 50, 'd');
	draw_text(36, 50, 'b');
	draw_text(37, 50, 'y');
	draw_text(39, 50, 'M');
	draw_text(40, 50, 'a');
	draw_text(41, 50, 'r');
	draw_text(42, 50, 'k');
	draw_text(44, 50, 'a');
	draw_text(45, 50, 'n');
	draw_text(46, 50, 'd');
	draw_text(48, 50, 'R');
	draw_text(49, 50, 'i');
	draw_text(50, 50, 'c');
	draw_text(51, 50, 'h');
	draw_text(52, 50, 'a');
	draw_text(53, 50, 'r');
	draw_text(54, 50, 'd');
	
	draw_text(31, 53, 'p');
	draw_text(32, 53, 'r');
	draw_text(33, 53, 'e');
	draw_text(34, 53, 's');
	draw_text(35, 53, 's');
	draw_text(37, 53, 'E');
	draw_text(38, 53, 'n');
	draw_text(39, 53, 't');
	draw_text(40, 53, 'e');
	draw_text(41, 53, 'r');
	draw_text(43, 53, 't');
	draw_text(44, 53, 'o');
	draw_text(46, 53, 's');
	draw_text(47, 53, 't');
	draw_text(48, 53, 'a');
	draw_text(49, 53, 'r');
	draw_text(50, 53, 't');
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

void plot_go(int x, int y, int color){
	int i, j;
	if(color == BLACK_STONE){
		for(i = 0; i < 13; ++i){
			for(j = 0; j < 13; ++j){
				plot_pixel(x + i, y + j, BLACK_GO[i*13+j]);
			}
		}
	}
	if(color == WHITE_STONE){
		for(i = 0; i < 13; ++i){
			for(j = 0; j < 13; ++j){
				plot_pixel(x + i, y + j, WHITE_GO[i*13+j]);
			}
		}
	}
	if(color == BLACK_HIGHLIGHT_STONE){
		for(i = 0; i < 14; ++i){
			for(j = 0; j < 14; ++j){
				plot_pixel(x + i, y + j, BLACK_HIGHLIGHT_GO[i*14+j]);
			}
		}
	}
	if(color == WHITE_HIGHLIGHT_STONE){
		for(i = 0; i < 14; ++i){
			for(j = 0; j < 14; ++j){
				plot_pixel(x + i, y + j, WHITE_HIGHLIGHT_GO[i*14+j]);
			}
		}
	}
}

void place_go(int x_coord, int y_coord, int color){
	plot_go(44 + (x_coord - 1) * 14, 12 + (y_coord - 1) * 14, color);
}

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
	draw_text(67, 33, 'S');
	draw_text(68, 33, 't');
	draw_text(69, 33, 'a');
	draw_text(70, 33, 't');
	draw_text(71, 33, 'u');
	draw_text(72, 33, 's');
	draw_text(73, 33, ':');
	clear_status();
	draw_current_status(TIE);
}

// Draw current game status on the board
void draw_current_status(int i){
	if(i == START){
		draw_text(67, 36, 'G');
		draw_text(68, 36, 'a');
	  	draw_text(69, 36, 'm');
	  	draw_text(70, 36, 'e');
	 	draw_text(72, 36, 'S');
	 	draw_text(73, 36, 't');
	  	draw_text(74, 36, 'a');
		draw_text(75, 36, 'r');
		draw_text(76, 36, 't');
		draw_text(77, 36, '!');
	}
	if(i == CONTINUE){
		draw_text(67, 36, 'C');
		draw_text(68, 36, 'o');
	  	draw_text(69, 36, 'n');
	  	draw_text(70, 36, 't');
	 	draw_text(72, 36, 'i');
	 	draw_text(73, 36, 'n');
	  	draw_text(74, 36, 'u');
		draw_text(75, 36, 'e');
	}
	if(i == BLACK_WON){
		draw_text(67, 36, 'P');
		draw_text(68, 36, 'l');
		draw_text(69, 36, 'a');
		draw_text(70, 36, 'y');
		draw_text(71, 36, 'e');
		draw_text(72, 36, 'r');
		draw_text(74, 36, '1');
		draw_text(76, 36, 'w');
		draw_text(77, 36, 'o');
		draw_text(78, 36, 'n');
		draw_text(79, 36, '!');
	}
	if(i == WHITE_WON){
		draw_text(67, 36, 'P');
		draw_text(68, 36, 'l');
		draw_text(69, 36, 'a');
		draw_text(70, 36, 'y');
		draw_text(71, 36, 'e');
		draw_text(72, 36, 'r');
		draw_text(74, 36, '2');
		draw_text(76, 36, 'w');
		draw_text(77, 36, 'o');
		draw_text(78, 36, 'n');
		draw_text(79, 36, '!');
	}
	if(i == TIE){
		draw_text(67, 36, 'T');
		draw_text(68, 36, 'i');
		draw_text(69, 36, 'e');
		draw_text(71, 36, '!');
	}
	if(i == INVALID){
		draw_text(67, 36, 'I');
		draw_text(68, 36, 'n');
		draw_text(69, 36, 'v');
		draw_text(70, 36, 'a');
		draw_text(71, 36, 'l');
		draw_text(72, 36, 'i');
		draw_text(73, 36, 'd');
		draw_text(75, 36, 'm');
		draw_text(76, 36, 'o');
		draw_text(77, 36, 'v');
		draw_text(78, 36, 'e');
	}
}

//Clear current game status
void clear_status(){
	clear_text(67, 80, 36);
}

//  clean a string at a given x, y coordinate (80 * 60)
void clear_text(int x1, int x2, int y){
	for(int i = x1; i <= x2; ++i){
		clear_one_char(i, y);
	}
}

// clean one character at a given x, y coordinate (80 * 60)
void clear_one_char(int x, int y){
	*(char *)(character_buffer_start + (y << 7) + x) = 0;
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
	
	