/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "standard.h" /* includes the use of functions rand()*/

/*DATA & VARIABLES*/
/* -Written by Leo Långberg & Ammar Alzeno   */
/*#########################################################################################*/
int mytime = 0x0000;
int timeoutcount = 0;       
int LED = 0; 
char textstring[] = "text, more text, and even more text!"; 

/*movement*/
#define LEFT   1
#define RIGHT  2
#define UP     3
#define DOWN   4
#define STALE  0
int direction = STALE;

/*POSITION [16x16]*/
int y = 7;
int x = 8;

int appleY;
int appleX;

/*##SNAKE##*/
/*datastorage*/
int len = 2;
int xarray[256]; //128*8=1024 pixels --> 2x2 = maximum 256 
int yarray[256];
int score;
int stages;


/*FUNCTIONS*/
/*#########################################################################################*/

/*Inserts y,x values into their data arrays
- Written by Leo Långberg & Ammar Alzeno */
void arrayextend ( int a[], int newvalue)
{
  int i;
  for(i = len; i > 0; i--)
  {
    a[i] = a[i - 1];
   
  }
  a[0] = newvalue;
}

/*Simple "to the power of" function (positive values only)
  - Written by Leo Långberg & Ammar Alzeno */
int powerof (int number, int exponent)
{
  int z = 1;
  int i;
  for( i = 1; i <= exponent; i++)
  {
    z = z * number;
  }
  return z;

}

/*   |16x16 MATRIX EQUATION|  
 * 
 * Formula for manipulating bits into becoming part of a 16x16 matrix:
 *  By using uint16_t 2 columns of 8 bits will be combined into 1 chunk.
 *  This chunk is then divided into 4 blocks
 *  which means that in every y-value there    Top  1.(0x0303) -- (771 * 4^0 = 771)
 *  is actually in itself 4 individual              2.(0x0C0C) -- (771 * 4^1 = 3084)
 *  y-values. bitvalue for the 4 highest            3.(0x3030) -- (771 * 4^2 = 12336)
 *  pixels is (0x0303) = (771).                Bot  4.(0xC0C0) -- (771 * 4^3 = 49344)
 *  Using OR will then add 
 *  new values without deleting previous.
 *  The reverse goes for removing 2x2 blocks 
 *  using NAND instead of OR.
 *  The formula while then use 
 *  these 2x2 blocks to create a clean 16x16 matrix easy to use.
 *  
/* sets a square of 2x2 pixels to 1 (black) 
    -Written by Leo Långberg */
void set_2x2(int y, int x, uint16_t *matrix)
{
  int column = ((y) / 4) * 16 + x;                   // 16 columns and y/4 (4st 2x2)
  int bitpos = powerof(4,(y % 4));
  int bitvalue = 771 * bitpos;                   // 256*3=768 4^y (0x0303 -> 0x0C0C)    
  
  matrix[column] = (matrix[column]) | (bitvalue); //OR OR --> set bit for bit  
}

/* set a square of 2x2 pixels to 0 (white)
    -Written by Leo Långberg */
void remove_2x2(int y, int x, uint16_t *matrix)
{
  int column = ((y) / 4) * 16 + x;
  int bitpos = powerof(4,(y % 4));         
  int bitvalue = (771 *bitpos);           
  
  matrix[column] = (matrix[column]) & (~(bitvalue)); //NAND NAND  --> remove bit for bit (0) (white)
}



/*Combination of functions that stores and prints the snake
  - Written by Leo Långberg */
void SNAKE_update ( void )
{
  /*  "TAIL" (chases snake at last position and clears 2x2 blocks) */
  remove_2x2(yarray[len],xarray[len], whitefont);
    
  /*STORES Y AND X VALUE*/
  arrayextend(yarray,y);
  arrayextend(xarray,x);

  /*SNAKE*/
  int j;
  for(j = 0; j < len; j++)
  {
    set_2x2(yarray[j],xarray[j],whitefont);
  }
      
}

/*Unchanged since Lab 3*/
void LEDS( void )
{
  volatile int * portE = (volatile int *) 0xbf886110;
  
  *portE = LED;
  
  return;
}

/*Places an apple with random coordinates 
  - Written by Ammar Alzeno */
void apple(){

  srand(mytime); // Seed the random number generator with the current time
  appleX = (rand() % 15);
  appleY = (rand() % 15);

  set_2x2(appleY, appleX, whitefont);
  
}

/*If the apple is eaten then increase score, snake-length and spawn new apple. 
  -Written by Ammar Alzeno */
void munch(){
  int i;
  for(i = 1; i < len; i++) //prevents bug where apple spawns inside snake
  {
    if((yarray[i] == appleY) && (xarray[i] == appleX)) 
    {
      apple();
    }
  }

  if(x == appleX && y == appleY){
    score++;
    len++;
    LED = powerof(2,(i % 8)); // score = green lights  >;)
    LEDS();
    remove_2x2(appleY, appleX, whitefont);
    apple();
  }
}



/*CLEANING SERVICE   **   HOUSEKEEPING*/
/*########################################*/
/* Cleans Matrix 
  -Written by Leo Långberg & Ammar Alzeno */
void cleaner( void )
{
 int i;
 int j;
 
 for(i = 0; i < 16; i++)
 {
  for(j = 0; j < 16; j++)
  {
    remove_2x2(i,j,whitefont);
  }
 }
}
/* Removes previous values in Data-Arrays 
  -Written by Leo Långberg & Ammar Alzeno */
void arrayflush ( int a[] )
{
  int i;
  for(i = 0; i < len; i++)
  {
    a[i] = -1; //fills them with arbitrary value 
  }
}
/* Resets variables and Arrays to starting values 
  -Written by Leo Långberg & Ammar Alzeno */
void reset ( void )
{
  mytime = 0x0000;
  direction = STALE;
  y = 7;
  x = 8;
  len = 2;
  stages = 2;
  LED = 0;
 
  arrayflush(yarray);
  arrayflush(xarray);
  cleaner();
  SNAKE_update();
  apple();
}
/* Snake collides with itself if x- and y-values align with array-values 
  -Written by Leo Långberg & Ammar Alzeno */
void ArrayCollision(int y, int x)
{
  int i;
  for(i = 0; i < len; i++)
  {
    if((yarray[i] == y) && (xarray[i] == x))  //if HEAD (x,y) == Array(x,y) -> head hits snake 
    {
      reset();
    }
  }
}
/*Boundaries in 16x16 matrix
  -Written by Ammar Alzeno*/
void wall ( void )
{
  /*Upperbound*//*southbound*/
  if( y < 0 || y > 15)
  {
    reset();
  }
  /*westbound*//*eastbound*/
  if( x < 0 || x > 15) 
  {
    reset();
  }
}



/* MOVEMENT */
/*########################################*/
/*AUTOPILOT (1 = L) (2 = DOWN) (3 = UP) (4 = R) 
  -Written by Leo Långberg & Ammar Alzeno   */
void autopilot ( void )
{
  
  if(direction == LEFT)   
  {
    x--;
  }
  if(direction == RIGHT)
  {
    x++;
  }
  if(direction == UP)
  {
    y--;
  }
  if(direction == DOWN)
  {
    y++;
  }
}

/*BUTTON INPUTS
    -Written by Ammar Alzeno   */
void buttonstate ( void )
{
  
  if(getbtns() == 0x4){
    direction = LEFT;
  }

  if(getbtns() == 0x2){ 
  direction = DOWN;
  }
 
  if(getbtns() == 0x1){
    direction = UP;
  }
  if(btn1() == 0x2)
  {
    direction = RIGHT;  
  }
}




/* Interrupt Service Routine */
/*########################################*/
/*  -Written by Leo Långberg & Ammar Alzeno   */
void user_isr( void ) 
{

  switch (stages)
  {
  /*** PREFACE ***/
  case 0:
      display_update();
      display_string(0, "WELCOME TO SNAKE");
      display_string(1, "PRESS ANY BUTTON");
      display_string(2, "TO START GAME");
      apple();
      
      if(getbtns() == 0x4 || getbtns() == 0x2 || getbtns() == 0x1 || btn1() == 0x2){
        reset();
        stages = 1;
        }
  break;

  /*** GAMEPLAY ***/
  case 1:
    display_update();
    display_string( 0,"Score: ");
    display_string( 1, itoaconv( score ));
    display_string( 2, "Kista");
    display_string( 3, "Chaser" );
    
      /*SCANNERS*/
      wall();
      buttonstate();
      autopilot();
      ArrayCollision(y,x);
  
      /*SNAKE*/
      SNAKE_update();
      munch();

      /*DISPLAY*/
      display_update();
      display_image(64, whitefont);
  break;

  /*** END SCREEN ***/
  case 2:
      display_update();
      display_string( 0,"Game over!");
      display_string( 1, "Score: ");
      display_string( 2, itoaconv(score));
      display_string( 3, "         ");
      
      
      if(getbtns() == 0x4 || getbtns() == 0x2 || getbtns() == 0x1 || btn1() == 0x2){
          reset();
          score = 0;
          stages = 1;
        }
   break;
  }

  /* SWITCH 4 = Manual Reset */
  if((IFS(0) & 0x80000))    //bit 19      0000 0000 0000 1000 0000 0000 0000 0000
  {
    IFSCLR(0) = 0x80000;
    reset();
  }
  tick( &mytime );
  IFSCLR(0) = 0x100;
}


/* Lab-specific initialization goes here 
  -Unchanged since lab 3   #(except for Arrayflush)# */
void labinit( void )
{
  /*LEDSINITIALIZE*/
  volatile int * trisE = (volatile int *) 0xbf886100;
  *trisE = (*trisE & 0xFF00);  //sätter bit 0 - 7 till Output (0)
  volatile int * portE = (volatile int *) 0xbf886110;
  *portE = 0x00;            //sätter value i portE --> 1 för varje lampa 1111 1111
  //Initialized to 0 currently


 
  /*INPUTINITIALIZE*/
  TRISD = (TRISD & 0xFEF); //btn 2-4
  TRISF = (TRISF & 0x2);   //btn1


  /*TIMERINITIALIZE*/
  T2CON = 0x70; // 1000 0000 0110 0000     (ON = 0) prescale = 1:256 (annars får det inte plats bitwise)
  PR2 = (((80000000) / 256) / 10); //  80 000 000 / 256 / 100 = 1/100s --->  80 000 000 / 256 / 10 = 100 ms = 1/10 s
  TMR2 = 0; //reset
  T2CONSET = 0x8070; //(ON = 1) //0x8070
  //TIMER2 = IFS(8) IEC(8)
  

  /*ENABLE INTERRUPTS*/
  IEC(0) = (1<<8);   //enable for timer2
  IPC(2) = 0x1F;    //4:2  1:0 priority
  IECSET(0) = 0x80000;
  IPCSET(4) = 0x1F000000;//0001 1111 0000 0000 0000 0000 0000 0000 28-24
  enable_interrupt();


  arrayflush(xarray);
	arrayflush(yarray);
 

  return;
}
/* This function is called repetitively from the main program */
void labwork( void ) {

}
  





  

