/*
Amy Feldman and Marisa Tyms
5/15/19
Pi Group 11
Pi Project2
Dice Simulator
Main.c file uses i2c device, gyroscope, and framebuffer
When the Raspberry Pi shakes, it draws four die in each quadrant(3X3).
And the program changes color when the joystick is pressed and redraws
the current screen in a different color.
*/


#include<sense/sense.h>
#include<signal.h>
#include <stdio.h>
#include <unistd.h>
#include "math.h"
#include "framebuffer.h"
#include <stdlib.h>
#include <linux/input.h>

int run = 1;

float x=0;
float y=0;
uint16_t color;

pi_framebuffer_t* fb;

/*
interruptHandler method
exits the program when the user presses control-C
*/
void interruptHandler(){
 run =0;
}
/*
drawDie method
pi_framebuffer_t *fb - framebuffer device
int num - number from random generator
int row - number of row on bitmap
int col - number of col on bitmap
int color- color of bitmap when dice is drawn

This function draws the dice in accordance with the number of
the random number generator. It prints the number of dots to the screen.
*/


void drawDie(pi_framebuffer_t *fb, int num, int row, int col,int color){
if (num == 1){
 fb->bitmap->pixel[1+row][1+col]=color;
}
if (num ==2){
 fb->bitmap->pixel[0+row][0+col]=color;
 fb->bitmap->pixel[2+row][2+col]=color;
}
if(num ==3){
 fb->bitmap->pixel[0+row][0+col]=color;
 fb->bitmap->pixel[1+row][1+col]=color;
 fb-> bitmap->pixel[2+row][2+col]=color;
}
if (num == 4) {
 fb->bitmap->pixel[0+row][0+col]=color;
 fb->bitmap->pixel[0+row][2+col]=color;
 fb->bitmap->pixel[2+row][0+col]=color;
 fb->bitmap->pixel[2+row][2+col]=color;
}
if (num == 5){
 fb->bitmap->pixel[0+row][0+col]=color;
 fb->bitmap->pixel[2+row][2+col]=color;
 fb->bitmap->pixel[1+row][1+col]=color;
 fb->bitmap->pixel[0+row][2+col]=color;
 fb->bitmap->pixel[2+row][0+col]=color;
}
if(num == 6){
 fb->bitmap->pixel[0+row][0+col]=color;
 fb->bitmap->pixel[0+row][1+col]=color;
 fb->bitmap->pixel[0+row][2+col]=color;
fb->bitmap->pixel[2+row][0+col]=color;
 fb->bitmap->pixel[2+row][1+col]=color;
 fb->bitmap->pixel[2+row][2+col]=color;
}

}

/*
callbackFn method
unsigned int code
This method calls the joystick and when
the joystick is moved it changes the color
of the pixels.
*/

void callbackFn(unsigned int code){
        clearBitmap(fb->bitmap, 0);
        //printf("%s","test");
        //setPixel(fb->bitmap,x,y,0);
        switch(code){
                case KEY_UP:
                        y=y==0?7:y-1;
                        break;
                case KEY_DOWN:
                        y=y==7?0:y+1;
                        break;
                case KEY_RIGHT:
                        x=x==7?0:x+1;
                        break;
		case KEY_LEFT:
                        x=x==7?0:x-1;
                        break;
                default:
                        run = 0;
                }

//uses function getColor to get the random color
//draws the die in different colors using the coordinates of the bitmap
        color=getColor(rand()%255, rand()%255, rand()%255);
        drawDie(fb, rand()%6 +1, 0, 0, color);
        drawDie(fb, rand()%6+1, 0, 5, color);
        drawDie(fb, rand()%6+1, 5, 0, color);
        drawDie(fb, rand()%6+1, 5, 5, color);
        }

int main(){

fb= getFBDevice();
pi_i2c_t* i2c = geti2cDevice();
pi_joystick_t* joy = getJoystickDevice();


//calls signal interruptHandler and exits when user presses control-C
signal(SIGINT, interruptHandler);


coordinate_t data;

	configureAccelGyro(i2c);
        while(run){

                usleep(2000);


                 getAccelData(i2c, &data);

 //checks if the raspberry pi is shaking

               if (fabs(data.x)+fabs(data.y)+fabs(data.z)>4){

                        clearBitmap(fb->bitmap, 0);
        //create 4 random numbers between 1 & 6
                        int rand1 = rand() %6+1;
                        int rand2 = rand() %6+1;
                        int rand3 = rand()%6+1;
                        int rand4 = rand() %6+1;
                        color = getColor( 255, 0, 0);
                        drawDie(fb, rand1, 0, 0, color);
                        drawDie(fb, rand2, 0, 5, color);
                        drawDie(fb, rand3, 5, 0, color);
                        drawDie(fb, rand4, 5, 5, color);
                        }
        //calls joystick and changes the color
                pollJoystick(joy, callbackFn, 1000);
        }



/*
frees data from bitmap, gyroscope, joystick, and framebuffer
*/
clearBitmap(fb->bitmap, 0);
freei2cDevice(i2c);
freeJoystick(joy);
freeFrameBuffer(fb);
}


























