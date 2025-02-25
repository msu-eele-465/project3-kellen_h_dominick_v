#include "intrinsics.h"
#include "msp430fr2355.h"
#include <msp430.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//Function Declarations
void unlockLoop();
void keypadSetup();
void heartbeatSetup();
bool arraysEqual(char[], char[]);
void keyReleased(volatile unsigned char*, unsigned char);
void LEDBarSetup();
void RGBLEDSetup();
void redLED();
void yellowLED();
void blueLED();
void greenLED();
void purpleLED();
void whiteLED();
void darkRedLED();
void colorChange(int, int, int);
void setPattern(int[]);
void patternZero(int[]);
void patternOne(int, int[]);
void patternTwo(int, int[]);
void patternThree(int, int[]);

//Main loop
int main() {
    
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    keypadSetup();

    char input_arr[4] = {'e', 'e', 'e', 'e'};
    char input_arr_pre[4];
    char unlock_code[4] = {'1', '2', '3', '4'};
    int pattern[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int patternZeroArr[8];
    int patternOneArr[8];
    int patternTwoArr[8];
    int patternThreeArr[8];
    bool locked = true;
    unsigned int counter = 0;
    int baseTransitionPeriod = 4;          
    int patternNum = -1;
    int prevPatterns[3] = {0, 0, 0};
    int step = 0;
    int i;
    int j;

    while(true) {
        while(locked) {
            if(input_arr[0] != 'e') 
            for(i = 0; i < 4; i++) {
                input_arr_pre[i] = input_arr[i];
            }
            P1OUT &= ~BIT2;
            if((P5IN & BIT2) == 0) {
                input_arr[counter] = '1';
                keyReleased(&P5IN, BIT2);
            } else if((P3IN & BIT6) == 0) {
                input_arr[counter] = '2';
                keyReleased(&P3IN, BIT6);
            } else if((P1IN & BIT7) == 0) {
                input_arr[counter] = '3';
                keyReleased(&P1IN, BIT7);
            } else if((P1IN & BIT6) == 0) {
                input_arr[counter] = 'A';
                keyReleased(&P1IN, BIT6);
            }
            P1OUT |= BIT2;
            P1OUT &= ~BIT3;
            if((P5IN & BIT2) == 0) {
                input_arr[counter] = '4';
                keyReleased(&P5IN, BIT2);
            } else if((P3IN & BIT6) == 0) {
                input_arr[counter] = '5';
                keyReleased(&P3IN, BIT6);
            } else if((P1IN & BIT7) == 0) {
                input_arr[counter] = '6';
                keyReleased(&P1IN, BIT7);
            } else if((P1IN & BIT6) == 0) {
                input_arr[counter] = 'B';
                keyReleased(&P1IN, BIT6);
            }
            P1OUT |= BIT3;
            P3OUT &= ~BIT4;
            if((P5IN & BIT2) == 0) {
                input_arr[counter] = '7';
                keyReleased(&P5IN, BIT2);
            } else if((P3IN & BIT6) == 0) {
                input_arr[counter] = '8';
                keyReleased(&P3IN, BIT6);
            } else if((P1IN & BIT7) == 0) {
                input_arr[counter] = '9';
                keyReleased(&P1IN, BIT7);
            } else if((P1IN & BIT6) == 0) {
                input_arr[counter] = 'C';
                keyReleased(&P1IN, BIT6);
            }
            P3OUT |= BIT4;
            P4OUT &= ~BIT5;
            if((P5IN & BIT2) == 0) {
                input_arr[counter] = '*';
                keyReleased(&P5IN, BIT2);
            } else if((P3IN & BIT6) == 0) {
                input_arr[counter] = '0';
                keyReleased(&P3IN, BIT6);
            } else if((P1IN & BIT7) == 0) {
                input_arr[counter] = '#';
                keyReleased(&P1IN, BIT7);
            } else if((P1IN & BIT6) == 0) {
                input_arr[counter] = 'D';
                keyReleased(&P1IN, BIT6);
            }
            P4OUT |= BIT5;
            
            int correct = 0;
            if(counter == 4) {
                for(i = 0; i < 4; i++) {
                    if(input_arr[i] != unlock_code[i] ) {
                        counter = 0;
                        for(j = 0; j < 4; j++) {
                            input_arr[j] = 'e';
                        }
                        break;
                    }
                    correct++;
                }
                if(correct == 4) locked = false;
            }
            else if(!arraysEqual(input_arr, input_arr_pre)) counter++;
        }

        while(true) {
            //Poll row 1
            P1OUT &= ~BIT2;
            if((P5IN & BIT2) == 0) {
                if(patternNum == 1) {
                    prevPatterns[0] = 0;
                }
                patternNum = 1;
                step = prevPatterns[0];
                keyReleased(&P5IN, BIT2);
            } else if((P3IN & BIT6) == 0) {
                if(patternNum == 2) {
                    prevPatterns[1] = 0;
                }
                patternNum = 2;
                step = prevPatterns[1];
                keyReleased(&P3IN, BIT6);
            } else if((P1IN & BIT7) == 0) {
                if(patternNum == 3) {
                    prevPatterns[2] = 0;
                }
                patternNum = 3;
                step = prevPatterns[2];
                keyReleased(&P1IN, BIT7);
            } else if((P1IN & BIT6) == 0) {
                if(baseTransitionPeriod > 1) baseTransitionPeriod -= 1;           //If A is clicked, decrease BTP by 0.25s
                keyReleased(&P1IN, BIT6);
            }
            P1OUT |= BIT2;
            //Poll row 2
            P1OUT &= ~BIT3;
            if((P1IN & BIT6) == 0) {
                if(baseTransitionPeriod < 8) baseTransitionPeriod += 1;           //If B is clicked, increase BTP by 0.25s
                keyReleased(&P1IN, BIT6);
            }
            P1OUT |= BIT3;
            //Poll row 4
            P4OUT &= ~BIT5;
            if((P3IN & BIT6) == 0) {
                patternNum = 0;
                step = 0;
                keyReleased(&P3IN, BIT6);
            } else if((P1IN & BIT6) == 0) {
                for(i = 0; i < 4; i++) {
                    input_arr[i] = 'e';
                    input_arr_pre[i] = 'e';
                }
                for(i = 0; i < 3; i++) {
                    prevPatterns[i] = 0;
                }
                patternNum = -1;
                locked = true;
                break;
            }
            P4OUT |= BIT5;           
    }
    return 0;
}

//Setup function for keypad
void keypadSetup() {
    
    P1DIR &= ~BIT6;         // all columns set as inputs
    P1DIR &= ~BIT7;
    P3DIR &= ~BIT6;
    P5DIR &= ~BIT2;

    P1REN |= BIT6;          // enable resistors for columns
    P1REN |= BIT7;
    P3REN |= BIT6;
    P5REN |= BIT2;
    P1OUT |= BIT6;          // set column resistors as pull-ups
    P1OUT |= BIT7;
    P3OUT |= BIT6;
    P5OUT |= BIT2;

    P4DIR |= BIT5;          // all rows set as outputs
    P3DIR |= BIT4;
    P1DIR |= BIT3;
    P1DIR |= BIT2;

    P4OUT |= BIT5;          // initialize row outputs HI
    P3OUT |= BIT4;
    P1OUT |= BIT3;
    P1OUT |= BIT2;
}

//Setup function for heartbeat LED
void heartbeatSetup() {
  
}


//ISR for heartbeat LED
#pragma vector = TIMER3_B0_VECTOR
__interrupt void ISR_TB3_CCR0(void) {
 
}


//Setup function for LED light bar
void LEDBarSetup() {
  
}

Setup function for RGB LED
void RGBLEDSetup() {

}

//Function for setting patternZeroArr to pattern zero
void patternZero(int patternZeroArr[]) {
   
}

//Function for setting patternOneArr to pattern one depending on step number
void patternOne(int step, int patternOneArr[]) {
    
}

//Function for setting patternTwoArr to pattern two depending on step number
void patternTwo(int step, int patternTwoArr[]) {
    
}

//Function for setting patternThreeArr to pattern three depending on step number
void patternThree(int step, int patternThreeArr[]) {
    
}

//Function for actually setting the pattern by driving 8 GPIO pins connected to LED light bar
void setPattern(int pattern[]) {
   
}

//ISR for period of red channel for RGB LED
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void) {
   
}

//ISR for pulse width of red channel for RGB LED
#pragma vector = TIMER0_B1_VECTOR      
__interrupt void ISR_TB0_CCR1(void) {
  
}

//ISR for period of green channel for RGB LED
#pragma vector = TIMER1_B0_VECTOR
__interrupt void ISR_TB1_CCR0(void) {
   
}

//ISR for pulse width of green channel for RGB LED
#pragma vector = TIMER1_B1_VECTOR       
__interrupt void ISR_TB1_CCR1(void) {
   
}

//ISR for period of blue channel for RGB LED
#pragma vector = TIMER2_B0_VECTOR
__interrupt void ISR_TB2_CCR0(void) {
   
}

//ISR for pulse width of blue channel for RGB LED
#pragma vector = TIMER2_B1_VECTOR       
__interrupt void ISR_TB2_CCR1(void) {
  
}

//Checks if two char arrays are equal
bool arraysEqual(char arr1[], char arr2[]) {
   int i;
    for(i = 0; i < 4; i++) {
        if(arr1[i] != arr2[i]) {
            return false;
        }
    }
    return true;
}

//Checks if a key on the keypad has been unpressed
void keyReleased(volatile unsigned char* pin, unsigned char bit) {
    while((*pin & bit) == 0) {}
    return;
}

//Sets RGB LED color to red
void redLED() {
    
}


//Sets RGB LED color to yellow
void yellowLED() {
    
}

//Sets RGB LED color to blue
void blueLED() {
    
}

//Sets RGB LED color to green
void greenLED() {
    
}

//Sets RGB LED color to purple
void purpleLED() {
    
}

//Sets RGB LED color to white
void whiteLED() {
  
}

//Sets RGB LED color to dark red
void darkRedLED() {
  
}

//Changes CCR1 on timers to reflect appropriate colors
void colorChange(int red, int green, int blue) {

}
