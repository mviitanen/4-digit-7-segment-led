/*
 * File:   blink.c
 * Author: viitanenm
 *
 * Created on April 5, 2013, 11:45 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24FJ64GB002.h>

#define DELAY 1000

#define DS_low()  LATB &=~0x8000
#define DS_high()  LATB |=0x8000

#define ST_CP_low() LATB &=~0x4000
#define ST_CP_high() LATB |=0x4000

#define SH_CP_low() LATB &=~0x2000
#define SH_CP_high() LATB |=0x2000

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


#define DIG1 0x8000
#define DIG2 0x4000
#define DIG3 0x2000
#define DIG4 0x1000

void doStep(unsigned int pattern);
void setLEDs(unsigned int pattern);
void selfTest();
void iterateAllDigits(unsigned int digit);

/*
 *
 */
int main(int argc, char** argv) {

    TRISB = 0x0000;
    T1CON = 0x8030;

    // reset everything
    LATB = 0x0000;
    doStep(0x00);
    while (1) {
        selfTest();
    } // main loop
    return (EXIT_SUCCESS);
}

void selfTest() {
    iterateAllDigits(DIG1);
    iterateAllDigits(DIG2);
    iterateAllDigits(DIG3);
    iterateAllDigits(DIG4);
}

void iterateAllDigits(unsigned int digit) {
    doStep(digit | 0x0003);
    doStep(digit | 0x009F);
    doStep(digit | 0x0025);
    doStep(digit | 0x000D);

    doStep(digit | 0x0099);
    doStep(digit | 0x0049);
    doStep(digit | 0x00C1);
    doStep(digit | 0x001F);

    doStep(digit | 0x0001);
    doStep(digit | 0x0019);
    doStep(digit | 0x00FE);
    doStep(digit | 0x0000);
    doStep(digit | 0x0000);
    doStep(0x0800);
    doStep(0x0800);
    doStep(0x0200);
    doStep(0x0200);

}

void doStep(unsigned int pattern) {
    TMR1 = 0;
    setLEDs(pattern);
    while (TMR1 < DELAY) {
    }
}

void setLEDs(unsigned int pattern) {
    ST_CP_low();
    
    int i;
    for (i = 0; i < 16; i++) {
        if (CHECK_BIT(pattern, i))
            DS_high();
        else
            DS_low();

        SH_CP_high();
        SH_CP_low();
    }
    SH_CP_low();
    ST_CP_high();
}
