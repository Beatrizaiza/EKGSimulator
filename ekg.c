/*
 * ekg.c
 *
 *  Created on: 6 Jan 2023
 *      Author: Beatriz Aiza, Leon Welker
 */
#include <msp430fr6989.h>
#include <math.h>
#include <stdio.h>
#include "lcd.h"

extern unsigned int ecgData[];
extern unsigned int dataLen;

unsigned int x = 0; // x[n]
unsigned int xnach = 0; // X[n-1]
unsigned int xvor = 0;// x[n+1]
unsigned int y1 = 0;// y[n]
unsigned int y2 = 0;//y[n+1]
unsigned int n=0;
unsigned int isTrue = 0;
unsigned  int oldIndex=0;
unsigned  int newIndex=0;

unsigned int abstand; //wie wird es berechnet?

// Algorithmus AF3 -??????
int af3 (unsigned int a){

    for(a=1;a≤dataLen;a++){

        xvor = ecgData[a-1];

        xnach = ecgData[a+1];

        y1= xnach-xvor;

        n++;

        xvor = ecgData[a-1];

        xnach = ecgData[a+1];

        y2= xnach-xvor;

        if (y1>800 && y2>800)
        {
            isTrue= 1;
        }
        else
            isTrue = 0;

    }

    return isTrue;
}

void main(void){
    WDTCTL = WDTPW+WDTHOLD;
    SFRIE1 |= WDTIE; // WDT-interrupt freischalten
    _EINT(); // global interrupt enable
    LCD_Init();
    ecgData_Init();

    int ekg = af3(n);

    if (ekg ==1)
    {
        if(newIndex > oldIndex+12)
        {
            LCD_Symbol(LCD_HRT_ID,LCD_SYMBOL_ON);
            abstand = newIndex-oldIndex;
            int puls= 60000/abstand;
            LCD_displayShort(puls);
            newIndex = oldIndex;
        }
    }

}

#pragma vector=WDT_VECTOR
__interrupt void tick(void){
    if (n<dataLen){
        newIndex = n;
        n++;
    }
    else
        n=0;
}
