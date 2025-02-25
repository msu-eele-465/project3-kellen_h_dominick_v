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
    RGBLEDSetup();
    LEDBarSetup();
    heartbeatSetup();

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
   
    return 0;
}

//Setup function for keypad
void keypadSetup() {

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
   
}

//Checks if a key on the keypad has been unpressed
void keyReleased(volatile unsigned char* pin, unsigned char bit) {
    
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
