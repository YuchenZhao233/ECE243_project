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
volatile int chess_board[ROW][COL];  // 0 for empty, 1 for black piece, 2 for white piece
volatile int x_position;
volatile int y_position;
bool draw = false;
unsigned char byte1 = 0;
unsigned char byte2 = 0;
unsigned char byte3 = 0;

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
void play_game();
void draw_layout();

// Initialize arrays of images 


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
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the
    // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer

    clear_screen();

    while(1){
        // Erase whole thing from the screen
        clear_screen();
        draw_layout();

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
        }

        if (byte3 == 0x1C && byte2 == 0xF0 && byte1 == 0x1C){ // A, move left
            printf("Left\n");
        }

        if (byte3 == 0x23 && byte2 == 0xF0 && byte1 == 0x23){ // D, move right
            printf("Right\n");
        }

        if (byte3 == 0x1D && byte2 == 0xF0 && byte1 == 0x1D){ // W, move up
            printf("Up\n");
        }

        if (byte3 == 0x1B && byte2 == 0xF0 && byte1 == 0x1B){ // S, move down
            printf("Down\n");
        }

        if (byte3 == 0x29 && byte2 == 0xF0 && byte1 == 0x29){ // Space, play the chess
            printf("Play\n");
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



