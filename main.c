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

// Function Prototypes
void swDelay(char numLoops);

// Declare globals here

// Main
void main(void)

{
    //unsigned char ret_val = 0x0F;
    unsigned char currKey=0, dispSz = 3;
    unsigned char dispThree[3];

    // Define some local variables
    float flt = 190.68;
    int  tst = 0x0400, i=0;   // In C prefix 0x means the number that follows is in hex
    long unsigned X= 200022;   // No prefix so number is assumed to be in decimal
    unsigned char myGrade='A';
    unsigned char initial='J';
    int state = 0;
    int oldmac[60] = {587,587,587,440,494,494,440,740,740,659,659,440,587,587,587,440,494,494,
                      440,740,740,659,659,587,440,440,587,587,587,440,440,587,587,587,587,587,
                      587,587,587,587,587,587,587,587,587,587,587,587,587,440,494,494,440,740,
                      740,659,659,587};

    unsigned int somethingFun = 0x2121;

    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

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

    setLeds(15);
    swDelay(10);
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
            for (i = 0; i < 60; i++) {
                int note = oldmac[i];
                BuzzerOnNote(note);
                swDelay(1);
                BuzzerOff();
            }
            state = 0;
        }
        }
    }


    // *** Intro Screen ***

    /*
    Graphics_clearDisplay(&g_sContext); // Clear the display

    // Write some text to the display
    Graphics_drawStringCentered(&g_sContext, "Guitar Hero", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Press *", AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);
    //Graphics_drawStringCentered(&g_sContext, "ECE2049!", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);


    // Draw a box around everything
    Graphics_Rectangle box = {.xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
    Graphics_drawRectangle(&g_sContext, &box);

    // We are now done writing to the display.  However, if we stopped here, we would not
    // see any changes on the actual LCD.  This is because we need to send our changes
    // to the LCD, which refreshes the display.
    // Since this is a slow operation, it is best to refresh (or "flush") only after
    // we are done drawing everything we need.
    Graphics_flushBuffer(&g_sContext);
    swDelay(5);

    while(1) {
               int temp = readButton();
               if (temp == 8){
                   setLeds(8);
               }
               else if (temp == 4) {
                   setLeds(4);
               }
               else if (temp == 2) {
                   setLeds(2);
               }
               else if (temp == 1) {
                   BuzzerOnNote(523);
                   swDelay(2);
                   BuzzerOff();
                   BuzzerOnNote(587);
                   swDelay(2);
                   BuzzerOff();
                   BuzzerOnNote(659);
                   swDelay(2);
                   BuzzerOff();
                   BuzzerOnNote(698);
                   swDelay(2);
                   BuzzerOff();
                   BuzzerOnNote(784);
                   swDelay(2);
                   BuzzerOff();
                   BuzzerOnNote(880);
                   swDelay(2);
                   BuzzerOff();
                   BuzzerOnNote(494);
                   swDelay(2);
                   BuzzerOff();
               }
           }

    */

    dispThree[0] = ' ';
    dispThree[2] = ' ';

    while (1)    // Forever loop
    {
        // Check if any keys have been pressed on the 3x4 keypad
        currKey = getKey();
        if ((currKey >= '0') && (currKey <= '9'))
            setLeds(currKey - 0x30);
        if (currKey == '*')
            BuzzerOn();
        if (currKey == '#')
            BuzzerOff();

        if (currKey)
        {
            dispThree[1] = currKey;
            // Draw the new character to the display
            Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 48, 55, OPAQUE_TEXT);

            // Refresh the display so it shows the new data
            Graphics_flushBuffer(&g_sContext);

            // wait awhile before clearing LEDs
            swDelay(1);
            setLeds(0);
        }


    }  // end while (1)
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
