/************** ECE2049 DEMO CODE ******************/
/**************  20 August 2016   ******************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"
# include <stdio.h>
# include <math.h>

#define E1 330
#define Fsh1 370
#define Gsh1 415
#define A1 440
#define Bb1 466
#define B1 494
#define C1 523
#define Csh1 554
#define D1 587
#define Eb2 622
#define E2 659
#define F2 698
#define Fsh2 740
#define G2 784
#define Gsh2 831
#define A2 880
#define Bb2 932
#define B2 988
#define C2 1046
#define Csh2 1109
#define D2 1175

#define whole 3.2
#define half 1.6
#define quarter 0.8
#define eighth 0.4
#define sixteenth 0.2
#define dHalf 2.4
#define dQuarter 1.2
#define dEighth 0.6
#define dSixteenth 0.3
#define halfEight 2.0


// Function Prototypes
void swDelay(char numLoops);
void startGame();
void startOver();
void A2_timer();
int ledNumber(int pitch);

unsigned long int timer;

//period = (80.92 + 1) * (1/32768) = 0.0025 seconds
//MAX_CNT = 80;
void A2_timer() {
    TA2CTL = TASSEL_1 + ID_0 + MC_1;
    TA2CCR0 = 80;   //it means we have 80 periods of 32 Khz each
    TA2CCTL0 = CCIE;
}

#pragma vector = TIMER2_A0_VECTOR

__interrupt void A2_ISRs() {
    timer++;
}

// Declare globals here

// Main
void main(void)

{
    //unsigned char ret_val = 0x0F;
    //unsigned char currKey=0, dispSz = 3;
    //unsigned char dispThree[3];

    // Define some local variables
    float flt = 190.68;
    int  tst = 0x0400, i=0;   // In C prefix 0x means the number that follows is in hex
    long unsigned X= 200022;   // No prefix so number is assumed to be in decimal
    unsigned char myGrade='A';
    unsigned char initial='J';
    int state = 0;

    int dQueenLength = 64;
    char dQueenNote [64][3] = {"E","Fsh","Gsh","Gsh","A","A","Gsh","A","A","B","A","Gsh","A","A","Gsh","A","A","Gsh","A","A","B","A","B","Csh","Csh","B","A","Csh","B","B","Csh","B","B","Csh","B","B","Csh","A","B","Gsh","A","B","Gsh","A","Gsh","Fsh","Csh","B","A","Gsh","A","A","B","A","Gsh","Gsh","A","A","B","A","Gsh","Gsh","A","A"};
    int dQueenPitch[64] = {E2,Fsh2,Gsh2,Gsh2,A2,A2,Gsh2,A2,A2,B2,A2,Gsh2,A2,A2,Gsh2,A2,A2,Gsh2,A2,A2,B2,A2,B2,Csh2,Csh2,B2,A2,Csh2,B2,B2,Csh2,B2,B2,Csh2,B2,B2,Csh2,A2,B2,Gsh2,A2,B2,Gsh2,A2,Gsh2,Fsh2,Csh2,B2,A2,Gsh2,A2,A2,B2,A2,Gsh2,Gsh2,A2,A2,B2,A2,Gsh2,Gsh2,A2,A2};
    float dQueenDuration[64] = {dEighth,dEighth,eighth,dEighth,dEighth,halfEight,dEighth,dEighth,quarter,quarter,eighth,dEighth,dEighth,halfEight,dEighth,dEighth,halfEight,dEighth,dEighth,quarter,quarter,eighth,dEighth,dEighth,dQuarter,quarter,whole,quarter,eighth,dQuarter,quarter,eighth,halfEight,dEighth,dEighth,eighth,dEighth,dEighth,eighth,sixteenth,sixteenth,dQuarter,dEighth,dEighth,eighth,dEighth,dEighth,halfEight,dEighth,dEighth,dQuarter,sixteenth,eighth,sixteenth,dEighth,dEighth,halfEight,sixteenth,eighth,sixteenth,dEighth,dEighth,halfEight};

    int oldmac[60] = {587,587,587,440,494,494,440,740,740,659,659,440,587,587,587,440,494,494,
                      440,740,740,659,659,587,440,440,587,587,587,440,440,587,587,587,587,587,
                      587,587,587,587,587,587,587,587,587,587,587,587,587,440,494,494,440,740,
                      740,659,659,587,880};
    int oldmaclength = 16;
    int oldmacpitch[16] = {440,494,523,587,659,698,784,880,440,494,523,587,659,698,784,880};
    float oldmacduration[16] = {1.0,1.0,0.5,0.5,0.2,0.2,2.0,2.0,1.0,1.0,0.5,0.5,0.2,0.2,2.0,2.0};

    unsigned int somethingFun = 0x2121;

    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer
    _BIS_SR(GIE);

    // Some utterly useless instructions -- Step through them
    // What size does the Code Composer MSP430 Compiler use for the
    // following variable types? A float, an int, a long integer and a char?
    flt = flt*tst;
    X = tst+X;
    tst = tst-myGrade;    // A number minus a letter?? What's actually going on here?
                          // What value stored in myGrade (i.e. what's the ASCII code for "A")?
                          // Thus, what is the new value of tst? Explain?

    // Useful code starts here
    initLeds();
    configDisplay();
    configKeypad();
    configButton();
    A2_timer();

    setLeds(15);
    swDelay(5);
    ledOff();

    Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);
    Graphics_flushBuffer(&g_sContext);

    while(1) {
        switch(state){
        case 0:
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "Guitar Hero", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            startGame();
            state++;
        case 1:
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "3!", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(1);
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "2!", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(1);
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "1!", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(1);
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "GO!!!", AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            swDelay(1);
            Graphics_clearDisplay(&g_sContext); // Clear the displays
            state++;
         break;
        case 2:{
            int i;
            int lostLives = 0;
            for (i = 0; i < dQueenLength; i++) {
                int flag = 0;
                char name[3] = dQueenNote[i];
                //Graphics_clearDisplay(&g_sContext); // Clear the display
                //Graphics_drawStringCentered(&g_sContext, name, AUTO_STRING_LENGTH, 48, 40, TRANSPARENT_TEXT);
                //Graphics_flushBuffer(&g_sContext);
                int note = dQueenPitch[i];
                float period = dQueenDuration[i]*400;
                int ledNum = ledNumber(note);
                if (ledNum > 4) {
                    ledNum = 4;
                }
                int notePeriod = timer + period;
                while (timer < notePeriod) {
                    BuzzerOnNote(note);
                    int ledState = 100;
                    if (ledNum == 1) {
                        ledState = 8;
                    }
                    else if (ledNum == 2) {
                        ledState = 4;
                    }
                    else if (ledNum == 3) {
                        ledState = 2;
                    }
                    else if (ledNum == 4){
                        ledState = 1;
                    }
                    setLeds(ledState);
                    if (readButton() == ledState) {
                        flag++;
                    }
                }
                if (flag == 0) {
                    lostLives++;
                }
                ledOff();
                BuzzerOff();
                unsigned long delayPeriod = timer + 30;
                while (timer < delayPeriod){
                }
                if (lostLives > 50) {
                   state = 3;
                   break;
                }
                else {
                    state = 4;
                }
            }
            break;
        }
        case 3:
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "WOW", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "You Suck", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "press #", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            setLeds(15);
            swDelay(3);
            ledOff();
            startOver();
            state = 0;
            break;
        case 4:
            Graphics_clearDisplay(&g_sContext); // Clear the display
            Graphics_drawStringCentered(&g_sContext, "You Won!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
            Graphics_drawStringCentered(&g_sContext, "press #", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
            Graphics_flushBuffer(&g_sContext);
            setLeds(15);
            swDelay(3);
            ledOff();
            startOver();
            state = 0;
            break;
        }
    }
}

int ledNumber(int pitch) {
    int led = 0;
    if (pitch == 659 || pitch == 880) {
        led = 1;
    }
    else if (pitch == 740) {
        led = 2;
    }
    else if (pitch == 831) {
        led = 3;
    }
    else if (pitch == 988 || pitch == 1109) {
        led = 4;
    }
    return led;
}



void swDelay(char numLoops)
{
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i,j;	// volatile to prevent optimization
			                            // by compiler

	for (j=0; j<numLoops; j++)
    {
    	i = 50000 ;					// SW Delay
   	    while (i > 0)				// could also have used while (i)
	       i--;
    }
}

void startGame(){
    while(1){
        unsigned char currKey = 0;
        currKey = getKey();
        if (currKey == '*'){
            return;
        }
    }
}

void startOver(){
    while(1){
        unsigned char currKey = 0;
        currKey = getKey();
        if (currKey == '#'){
            return;
        }
    }
}

