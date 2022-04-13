//#include "address_map_arm.h"
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
#define ROW 15
#define COL 15

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
#define TIE 3
#define CONTINUE 4
#define END 5
#define ERROR 6
#define VALID 7

#define PLAYER1 1
#define PLAYER2 2

// Global Variable
volatile int pixel_buffer_start; 
volatile int character_buffer_start;
volatile int chess_board[ROW][COL];  // 0 for empty, 1 for black piece, 2 for white piece
volatile int x_position = 0;
volatile int y_position = 0;
bool draw = false;
unsigned char byte1 = 0;
unsigned char byte2 = 0;
unsigned char byte3 = 0;
int status = CONTINUE;
int turn = PLAYER1;

// Interupt set up
void set_A9_IRQ_stack (void);
void config_GIC (void);
void config_KEYs (void);
void config_PS2 (void);
void config_interrupt (int, int);
void enable_A9_interrupts (void);
void disable_A9_interrupts (void);
void pushbutton_ISR(void);
void keyboard_ISR(void);

// Functions for drawing on the VGA Display
void draw_board();
void draw_chess();
void draw_chess_on_board();
void draw_text(int x, int y, char c);
void plot_whole_board();
void plot_board_coordinates();
void plot_go(int x, int y, int color);
void place_go(int x_coord, int y_coord, int color);
void draw_player_info();

void plot_pixel(int, int, short int);
void draw_line(int , int , int , int , short int);
void clear_screen();
void swap(int*, int*);
void wait_for_vsync();
void draw_postion();

// Game logic functions
int check_move_legality(int x, int y);
void init_board();
void draw_cmd_board();
int game_state();
int calculate_pieces(int direction);
void play_game();
void draw_layout();

// Initialize arrays of images 
const unsigned short BLACK_GO[169]={
	0xc42b, 0xbc2b, 0xc42b, 0xa369, 0x6a66, 0x3964, 0x2924, 0x4185, 0x6a66, 0xa369, 0xbc0b, 0xbbea, 0xbc0a, 0xbc2b, 0xc44b, 0x82c8, 0x41e7, 0x1904, 0x1904, 0x2104, 0x1904, 0x18c3, 0x2904, 0x7287, 0xabaa, 0xabca, 0xc44b, 0x7ac7, 0x2966, 0x4a49, 0x2124, 0x2124, 0x2104, 0x2104, 0x2124, 0x2104, 0x18e3, 0x6a46, 0xa369, 0xabca, 0x2924, 0x2125, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2924, 0x82c8, 0x7287, 0x18e3, 0x2965, 0x2945, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x1904, 0x59e6, 0x49c5, 0x1904, 0x2945, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x3985, 0x3985, 0x2104, 0x2124, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2965, 0x3165, 0x3985, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x2944, 0x72a8, 0x4228, 0x2945, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x18e4, 0x49c5, 0xa369, 0x4a28, 0x39c6, 0x2965, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x20e3, 0x2124, 0x2104, 0x7267, 0xc44b, 0x7aa8, 0x1904, 0x2124, 0x2124, 0x2104, 0x2104, 0x2104, 0x2104, 0x2124, 0x10c3, 0x49c6, 0x9b49, 0xbc0b, 0xc42b, 0x72a8, 0x2104, 0x3186, 0x2124, 0x2104, 0x2104, 0x1904, 0x2965, 0x5206, 0xa369, 0xabaa, 0xc42b, 0xbc2b, 0xc44b, 0x9b69, 0x49e6, 0x2124, 0x2104, 0x2104, 0x3985, 0x82e8, 0xbbea, 0xbbea, 0xbc0b
};

//Pixel array of white chess
const unsigned short WHITE_GO[169]={
	0xeeef, 0xf710, 0xeeef, 0xd60e, 0xce13, 0xce57, 0xce79, 0xce57, 0xce13, 0xd60e, 0xeeef, 0xf710, 0xeeef, 0xf710, 0xeeae, 0xc5f1, 0xe73c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe73c, 0xc5f1, 0xeeae, 0xf710, 0xeeef, 0xc5f1, 0xf7bf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xf7bf, 0xc5f1, 0xeeef, 0xce0e, 0xe73c, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xe73c, 0xce0e, 0xce34, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce34, 0xce78, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce78, 0xd699, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xd699, 0xce78, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce78, 0xce34, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xce34, 0xce0f, 0xef5d, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xef5d, 0xce0f, 0xeece, 0xc5f2, 0xffdf, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffdf, 0xc5f2, 0xeece, 0xf710, 0xe6ae, 0xc5f2, 0xef5e, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xef5e, 0xc5f2, 0xe6ae, 0xf710, 0xeeef, 0xf710, 0xeece, 0xce0f, 0xce35, 0xe6fa, 0xdefa, 0xe6fa, 0xce35, 0xce0f, 0xeece, 0xf710, 0xeeef
};


// Define the IRQ exception handler
void __attribute__ ((interrupt)) __cs3_isr_irq (void){
    // Read the ICCIAR from the CPU Interface in the GIC
    int interrupt_ID = *((int *) 0xFFFEC10C);
    if (interrupt_ID == 73) // check if interrupt is from the KEYs
        pushbutton_ISR ();
    else if (interrupt_ID == 79) // check if interrupt is from the keyboard
	    keyboard_ISR();
    else
        while (1); // if unexpected, then stay here
    // Write to the End of Interrupt Register (ICCEOIR)
    *((int *) 0xFFFEC110) = interrupt_ID;
}

// Define the remaining exception handlers
void __attribute__ ((interrupt)) __cs3_reset (void){
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_undef (void){
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi (void){
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort (void){
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort (void){
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_fiq (void){
    while(1);
}

void draw_layout(){
    if (draw == true){
        int x = 0;
        int y = 0;
        for (x = 0; x < RESOLUTION_X; x++) {
            for (y = 0; y < RESOLUTION_Y; y++) {
                plot_pixel(x, y, ORANGE);
            }
        }
    }
}



// Some AI function possibly

int main(void){
    /*int play;
    while (true){
        printf("Do you want to play this gobang game? 1 for yes, 2 for no. \n");
        scanf("%d", &play);
        if (play == 1){
            play_game();
        } 
        else if (play == 2){
            printf("Ok\n");
            break;
        }
    }

    printf("That's it.\n");*/
    disable_A9_interrupts(); // disable interrupts in the A9 processor
    set_A9_IRQ_stack(); // initialize the stack pointer for IRQ mode
    config_GIC(); // configure the general interrupt controller
    config_KEYs(); // configure KEYs to generate interrupts
    config_PS2(); // configure keyboard to generate interrupts
    enable_A9_interrupts(); // enable interrupts in the A9 processor

    volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
    volatile int *char_ctrl_ptr = (int *)0xFF203030;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the
    // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    character_buffer_start = *char_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer

    clear_screen();



    while(1){
        // Erase whole thing from the screen
        clear_screen();
        plot_whole_board();
        // draw_postion();
        draw_chess_on_board();
        wait_for_vsync();
        pixel_buffer_start = *(pixel_ctrl_ptr + 1);
    }
}

void play_game(){
    init_board();
    int turn = PLAYER1;
    int result = 0;
    while (true){
        draw_cmd_board();
        while (true){
            printf("%d, Please enter the position:\n", turn);
            scanf("%d%d", &x_position, &y_position);
            if (check_move_legality(x_position, y_position) == VALID){
                chess_board[x_position][y_position] = turn;
                break;
            } else{
                printf("Not valid.\n");
                continue;
            }
        }
        result = game_state();
        if (result != CONTINUE)
            break;
        
        if (turn == PLAYER1) turn = PLAYER2;
        else turn = PLAYER1;
    }

    draw_cmd_board();
    if (result == PLAYER1) printf("1 wins\n");
    else if (result == PLAYER2) printf("2 wins\n");
    else if (result == TIE) printf("A draw.\n");

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

// Check the status of the game, decide the winner, or continue the game
int game_state(){
    // See whether 5 pieces is connected, return the winner
    if (calculate_pieces(UP) + calculate_pieces(DOWN) >= 4 || calculate_pieces(LEFT) + calculate_pieces(RIGHT) >= 4 ||
        calculate_pieces(LEFT_DOWN) + calculate_pieces(RIGHT_UP) >= 4 || calculate_pieces(LEFT_UP) + calculate_pieces(RIGHT_DOWN) >= 4)
        return chess_board[x_position][y_position];

    // If there are still empty place on the board, the game should continue
    int i = 0;
    for (; i < ROW; i++){
        int j = 0;
        for (; j < COL; j++){
            if (chess_board[i][j] == 0)
                return CONTINUE;
        }
    }

    return TIE;

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
            new_y++;
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

void clear_screen()
{
    int y, x;
    for (x = 0; x < 320; x++)
        for (y = 0; y < 240; y++)
            plot_pixel (x, y, 0);
}

void draw_postion(){
    place_go(y_position + 1, x_position + 1, 0);
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
	// place_go(7, 8, 0);
	// place_go(1, 9, 1);
	// place_go(3, 4, 1);
	// place_go(3, 5, 1);
	// plot_go(58, 54, 0);
	// plot_go(58, 68, 1);
	
	// draw_player_info();
	plot_board_coordinates();
}

void draw_chess_on_board(){
    int i = 0;
    for (; i < ROW; i++){
        int j = 0;
        for (; j < COL; j++){
            if (chess_board[i][j] == 1)
                place_go(j + 1, i + 1, 0);
            else if (chess_board[i][j] == 2)
                place_go(j + 1, i + 1, 1);
        }
    }

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

/* setup the KEY interrupts in the FPGA */
void config_KEYs(){
    volatile int * KEY_ptr = (int *) 0xFF200050; // KEY base address
    *(KEY_ptr + 2) = 0xF; // enable interrupts for all four KEYs
}

/* setup the keyboard interrupts in the FPGA */
void config_PS2 (void){
    volatile int * PS2_ptr = (int *) 0xFF200100; // PS2 base address
	*(PS2_ptr + 1) = 0x00000001; // set the last bit to 1 to enable interrupts
}

/*
* Turn off interrupts in the ARM processor
*/
void disable_A9_interrupts(void){
    int status = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

/*
* Initialize the banked stack pointer register for IRQ mode
*/
void set_A9_IRQ_stack(void){
    int stack, mode;
    stack = 0xFFFFFFFF - 7; // top of A9 onchip memory, aligned to 8 bytes
    /* change processor to IRQ mode with interrupts disabled */
    mode = 0b11010010;
    asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
    /* set banked stack pointer */
    asm("mov sp, %[ps]" : : [ps] "r" (stack));
    /* go back to SVC mode before executing subroutine return! */
    mode = 0b11010011;
    asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

/*
* Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void){
    int status = 0b01010011;
    asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}


/*
* Configure the Generic Interrupt Controller (GIC)
*/
void config_GIC(void){
    config_interrupt (73, 1); // configure the FPGA KEYs interrupt (73)
    config_interrupt (79, 1); // configure the FPGA PS2 port interrupt (79)
    // Set Interrupt Priority Mask Register (ICCPMR). Enable all priorities
    *((int *) 0xFFFEC104) = 0xFFFF;
    // Set the enable in the CPU Interface Control Register (ICCICR)
    *((int *) 0xFFFEC100) = 1;
    // Set the enable in the Distributor Control Register (ICDDCR)
    *((int *) 0xFFFED000) = 1;
}

// Configure interupt based on interupt id
void config_interrupt (int N, int CPU_target){
    int reg_offset, index, value, address;
    /* Configure the Interrupt Set-Enable Registers (ICDISERn).
     * reg_offset = (integer_div(N / 32) * 4; value = 1 << (N mod 32) */
    reg_offset = (N >> 3) & 0xFFFFFFFC;
    index = N & 0x1F;
    value = 0x1 << index;
    address = 0xFFFED100 + reg_offset;
    /* Using the address and value, set the appropriate bit */
    *(int *)address |= value;
    /* Configure the Interrupt Processor Targets Register (ICDIPTRn)
     * reg_offset = integer_div(N / 4) * 4; index = N mod 4 */
    reg_offset = (N & 0xFFFFFFFC);
    index = N & 0x3;
    address = 0xFFFED800 + reg_offset + index;
    /* Using the address and value, write to (only) the appropriate byte */
    *(char *)address = (char) CPU_target;
}

// Handler when there is input from the keyboard
void keyboard_ISR(){
    volatile int *PS2_ptr = (int *) 0xFF200100; // Points to PS2 Base
    int PS2_data, RVALID;
    PS2_data = *(PS2_ptr);	// read the Data register in the PS/2 port
	RVALID = (PS2_data & 0x8000);	// extract the RVALID field, the 15 bit - read data valid

    // Clear interrupt
    int keyboard_status;
	keyboard_status = *(PS2_ptr + 1 ); 
	*(PS2_ptr+1) = keyboard_status; 

    if (RVALID != 0){
        byte1 = byte2;
        byte2 = byte3;
        byte3 = (PS2_data & 0xFF);  // the 31 to 16 bits stored information from the key board

        if (byte3 == 0x5A && byte2 == 0xF0 && byte1 == 0x5A){ //enter is pressed
            printf("Game start\n");
            init_board();

        }

        if (byte3 == 0x1C && byte2 == 0xF0 && byte1 == 0x1C){ // A, move left
            if (y_position != 0)
                y_position--;


        }

        if (byte3 == 0x23 && byte2 == 0xF0 && byte1 == 0x23){ // D, move right
            if (y_position != ROW - 1)
                y_position++;
        }

        if (byte3 == 0x1D && byte2 == 0xF0 && byte1 == 0x1D){ // W, move up
            if (x_position != 0)
                x_position--;
        }

        if (byte3 == 0x1B && byte2 == 0xF0 && byte1 == 0x1B){ // S, move down
            if (x_position != COL - 1)
                x_position++;
        }

        if (byte3 == 0x29 && byte2 == 0xF0 && byte1 == 0x29){ // Space, play the chess
            status = check_move_legality(x_position, y_position);
            if (status == VALID){
                chess_board[x_position][y_position] = turn;
                status = game_state();
                if (status == CONTINUE){
                    if (turn == PLAYER1) turn = PLAYER2;
                    else turn = PLAYER1;
                }
                else if (status == 1)
                    printf("Player 1 wins\n");
                else if (status == 2)
                    printf("Player 2 wins\n");
                else
                    printf("Tie\n");
                    
                

            
            }
            
        

        }


        
		
    }

    return;
}

// Handler when a push button is clicked
void pushbutton_ISR(void){
    volatile int *key_ptr = (int *) 0xFF200050; // Points to KEY_base

    // Clear interrupt
    int press;
    press = *(key_ptr + 3);
    *(key_ptr + 3) = press;

    if (press & 0x1){
        if(draw == true)
            draw = false;
        else
            draw = true;
    }

    return;

}



