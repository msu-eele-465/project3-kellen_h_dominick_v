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

//Main Loop
int main() {

    //Stop Watchdog timer
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;

    //Setup all devices
    keypadSetup();
    RGBLEDSetup();
    LEDBarSetup();
    heartbeatSetup();

    //Initialize variables (global)
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

    //Set light bar pattern to all zeros
    setPattern(pattern);

    //Poll keypad forever
    while(true) {
        //Set LED to red to start
        redLED();
        while(locked) {
            //If a key has been pressed, set LED to yellow
            if(input_arr[0] != 'e') yellowLED();
            for(i = 0; i < 4; i++) {
                input_arr_pre[i] = input_arr[i];
            }
            //poll row 1
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
            //poll row 2
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
            //poll row 3
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
            //poll row 4
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
            
            //if 4 keys have been pressed, check if equal to unlock code
            int correct = 0;
            if(counter == 4) {
                for(i = 0; i < 4; i++) {
                    if(input_arr[i] != unlock_code[i] ) {
                        counter = 0;
                        for(j = 0; j < 4; j++) {
                            input_arr[j] = 'e';
                        }
                        redLED();
                        break;
                    }
                    correct++;
                }
                if(correct == 4) locked = false;
            }
            else if(!arraysEqual(input_arr, input_arr_pre)) counter++;
        }

        //If unlock code was entered, set LED to blue
        blueLED();

        //Loop checking for pattern code
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
                setPattern(pattern);
                locked = true;
                break;
            }
            P4OUT |= BIT5;

            //If 0 was pressed, display pattern 0
            if(patternNum == 0) {
                greenLED();
                patternZero(patternZeroArr);
            }
            //If 1 was pressed, display pattern 1 starting at saved state
            else if(patternNum == 1) {
                darkRedLED();
                for(i = 0; i < baseTransitionPeriod; i++) {
                __delay_cycles(250000);
                }
                patternOne(prevPatterns[0], patternOneArr);
                step++;
                if(step == 2) {
                    step = 0;
                }
                prevPatterns[0] = step;
            }
            //If 2 was pressed, display pattern 2 starting at saved state
            else if(patternNum == 2) {
                purpleLED();
                for(i = 0; i < baseTransitionPeriod; i++) {
                __delay_cycles(125000);
                }
                patternTwo(prevPatterns[1], patternTwoArr);
                step++;
                if(step == 256) {
                    step = 0;
                }
                prevPatterns[1] = step;
            }
            //If 3 was pressed, display pattern 3 starting at saved state
            else if(patternNum == 3) {
                whiteLED();
                for(i = 0; i < baseTransitionPeriod; i++) {
                __delay_cycles(125000);
                }
                patternThree(prevPatterns[2], patternThreeArr);
                step++;
                if(step == 6) {
                    step = 0;
                }
                prevPatterns[2] = step;
            }
        }
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

//Setup function for hearbeat LED
void heartbeatSetup() {
    P6DIR |= BIT6;
    P6OUT &= ~BIT6;

    TB3CTL = TBSSEL__ACLK | MC__UP | TBCLR;

    TB3CCR0 = 16384;
    TB3CCTL0 |= CCIE;
    TB3CCTL0 &= ~CCIFG;
}

//ISR for toggling LED2 for heartbeat LED
#pragma vector = TIMER3_B0_VECTOR
__interrupt void ISR_TB3_CCR0(void) {
    P6OUT ^= BIT6;
    TB3CCTL0 &= ~CCIFG;
}

//Setup function for light bar
void LEDBarSetup() {
    P2DIR |= BIT1;              //LED1
    P6DIR |= BIT0;              //LED2
    P6DIR |= BIT1;              //LED3
    P6DIR |= BIT2;              //LED4
    P6DIR |= BIT3;              //LED5
    P6DIR |= BIT4;              //LED6
    P3DIR |= BIT7;              //LED7
    P2DIR |= BIT4;              //LED8
}

//Setup function for RGB LED
void RGBLEDSetup() {
    P5DIR |= BIT3 | BIT1 | BIT0;   // Set red, green, and blue pins to outputs

    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB1CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB2CTL = TBSSEL__ACLK | MC__UP | TBCLR;
    TB0CCR0 = 255;
    TB1CCR0 = 255;
    TB2CCR0 = 255;

    TB0CCR1 = 254;                  // RED
    TB1CCR1 = 254;                  // GREEN
    TB2CCR1 = 254;                  // BLUE

    TB0CCTL0 |= CCIE;
    TB0CCTL1 |= CCIE;
    TB1CCTL0 |= CCIE;
    TB1CCTL1 |= CCIE;
    TB2CCTL0 |= CCIE;
    TB2CCTL1 |= CCIE;
    __enable_interrupt();
    TB0CCTL0 &= ~CCIFG;
    TB0CCTL1 &= ~CCIFG;
    TB1CCTL0 &= ~CCIFG;
    TB1CCTL1 &= ~CCIFG;
    TB2CCTL0 &= ~CCIFG;
    TB2CCTL1 &= ~CCIFG;
}

//Sets patternZeroArr to pattern zero and sends it off to setPattern
void patternZero(int patternZeroArr[]) {
    patternZeroArr[0] = 1;
    patternZeroArr[1] = 0;
    patternZeroArr[2] = 1;
    patternZeroArr[3] = 0;
    patternZeroArr[4] = 1;
    patternZeroArr[5] = 0;
    patternZeroArr[6] = 1;
    patternZeroArr[7] = 0;
    setPattern(patternZeroArr);
}

//Sets patternOneArr to pattern 1 at step n and sends it off to setPattern
void patternOne(int step, int patternOneArr[]) {
    if(step == 0) {
        patternOneArr[0] = 1;
        patternOneArr[1] = 0;
        patternOneArr[2] = 1;
        patternOneArr[3] = 0;
        patternOneArr[4] = 1;
        patternOneArr[5] = 0;
        patternOneArr[6] = 1;
        patternOneArr[7] = 0;
        setPattern(patternOneArr);
    }
    else {
        patternOneArr[0] = 0;
        patternOneArr[1] = 1;
        patternOneArr[2] = 0;
        patternOneArr[3] = 1;
        patternOneArr[4] = 0;
        patternOneArr[5] = 1;
        patternOneArr[6] = 0;
        patternOneArr[7] = 1; 
        setPattern(patternOneArr);
    }
}

//Sets patternTwoArr to pattern 2 at step n and sends it off to setPattern
void patternTwo(int step, int patternTwoArr[]) {
    int num = step;
    unsigned int mask = 1 << 7;
    int k;
    for(k = 0; k < 8; k++) {
        if(num & mask) {
            patternTwoArr[k] = 1;
        }
        else {
            patternTwoArr[k] = 0;
        }
        num <<= 1;
    }

    setPattern(patternTwoArr);
}

//Sets patternThreeArr to pattern 3 at step n and sends it off to setPattern
void patternThree(int step, int patternThreeArr[]) {
    if(step == 0) {
        patternThreeArr[0] = 0;
        patternThreeArr[1] = 0;
        patternThreeArr[2] = 0;
        patternThreeArr[3] = 1;
        patternThreeArr[4] = 1;
        patternThreeArr[5] = 0;
        patternThreeArr[6] = 0;
        patternThreeArr[7] = 0;
        setPattern(patternThreeArr);
    }
    else if(step == 1) {
        patternThreeArr[0] = 0;
        patternThreeArr[1] = 0;
        patternThreeArr[2] = 1;
        patternThreeArr[3] = 0;
        patternThreeArr[4] = 0;
        patternThreeArr[5] = 1;
        patternThreeArr[6] = 0;
        patternThreeArr[7] = 0;
        setPattern(patternThreeArr);
    }
    else if(step == 2) {
        patternThreeArr[0] = 0;
        patternThreeArr[1] = 1;
        patternThreeArr[2] = 0;
        patternThreeArr[3] = 0;
        patternThreeArr[4] = 0;
        patternThreeArr[5] = 0;
        patternThreeArr[6] = 1;
        patternThreeArr[7] = 0;
        setPattern(patternThreeArr);
    }
    else if(step == 3) {
        patternThreeArr[0] = 1;
        patternThreeArr[1] = 0;
        patternThreeArr[2] = 0;
        patternThreeArr[3] = 0;
        patternThreeArr[4] = 0;
        patternThreeArr[5] = 0;
        patternThreeArr[6] = 0;
        patternThreeArr[7] = 1;
        setPattern(patternThreeArr);
    }
    else if(step == 4) {
        patternThreeArr[0] = 0;
        patternThreeArr[1] = 1;
        patternThreeArr[2] = 0;
        patternThreeArr[3] = 0;
        patternThreeArr[4] = 0;
        patternThreeArr[5] = 0;
        patternThreeArr[6] = 1;
        patternThreeArr[7] = 0;
        setPattern(patternThreeArr);
    }
    else if(step == 5) {
        patternThreeArr[0] = 0;
        patternThreeArr[1] = 0;
        patternThreeArr[2] = 1;
        patternThreeArr[3] = 0;
        patternThreeArr[4] = 0;
        patternThreeArr[5] = 1;
        patternThreeArr[6] = 0;
        patternThreeArr[7] = 0;
        setPattern(patternThreeArr);
    }
}

//Sets values of lightbar
void setPattern(int pattern[]) {
    if(pattern[0] == 1) {
        P2OUT |= BIT1;
    }
    else {
        P2OUT &= ~BIT1;
    }
    if(pattern[1] == 1) {
        P6OUT |= BIT0;
    }
    else {
        P6OUT &= ~BIT0;
    }
    if(pattern[2] == 1) {
        P6OUT |= BIT1;
    }
    else {
        P6OUT &= ~BIT1;
    }
    if(pattern[3] == 1) {
        P6OUT |= BIT2;
    }
    else {
        P6OUT &= ~BIT2;
    }
    if(pattern[4] == 1) {
        P6OUT |= BIT3;
    }
    else {
        P6OUT &= ~BIT3;
    }
    if(pattern[5] == 1) {
        P6OUT |= BIT4;
    }
    else {
        P6OUT &= ~BIT4;
    }
    if(pattern[6] == 1) {
        P3OUT |= BIT7;
    }
    else {
        P3OUT &= ~BIT7;
    }
    if(pattern[7] == 1) {
        P2OUT |= BIT4;
    }
    else {
        P2OUT &= ~BIT4;
    }
}

//ISR for red period
#pragma vector = TIMER0_B0_VECTOR
__interrupt void ISR_TB0_CCR0(void) {
    P5OUT &= ~BIT3;
    TB0CCTL0 &= ~CCIFG;
}

//ISR for red pulse width
#pragma vector = TIMER0_B1_VECTOR       
__interrupt void ISR_TB0_CCR1(void) {
    P5OUT |= BIT3;
    TB0CCTL1 &= ~CCIFG;
}

//ISR for green period
#pragma vector = TIMER1_B0_VECTOR
__interrupt void ISR_TB1_CCR0(void) {
    P5OUT &= ~BIT1;
    TB1CCTL0 &= ~CCIFG;
}

//ISR for green pulse width
#pragma vector = TIMER1_B1_VECTOR       
__interrupt void ISR_TB1_CCR1(void) {
    P5OUT |= BIT1;
    TB1CCTL1 &= ~CCIFG;
}

//ISR for blue period
#pragma vector = TIMER2_B0_VECTOR
__interrupt void ISR_TB2_CCR0(void) {
    P5OUT &= ~BIT0;
    TB2CCTL0 &= ~CCIFG;
}

//ISR for blue pulse width
#pragma vector = TIMER2_B1_VECTOR       
__interrupt void ISR_TB2_CCR1(void) {
    P5OUT |= BIT0;
    TB2CCTL1 &= ~CCIFG;
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

//Check if key has been unpressed
void keyReleased(volatile unsigned char* pin, unsigned char bit) {
    while((*pin & bit) == 0) {}
    return;
}

//Sets red
void redLED() {
    colorChange(59, 193, 226);
}

//Sets yellow
void yellowLED() {
    colorChange(59, 109, 226);
}

//Sets blue
void blueLED() {
    colorChange(226, 93, 59);
}

//Sets green
void greenLED() {
    colorChange(255, 1, 255);
}

//Sets purple
void purpleLED() {
    colorChange(1, 255, 1);
}

//Sets white
void whiteLED() {
    colorChange(1, 2, 2);
}

//Sets dark red
void darkRedLED() {
    colorChange(1, 255, 255);
}

//Sets CCR1 for timers B0,1,2 to reflect correct color
void colorChange(int red, int green, int blue) {
    TB0CCR1 = 255 - red;
    TB1CCR1 = 255 - green;
    TB2CCR1 = 255 - blue;
}
