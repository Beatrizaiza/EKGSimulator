/*
 * ekg.c
 *
 *  Created on: 10.01.2023
 *      Author: leonwelker
 */
#include <msp430fr6989.h>
#include <math.h>
#include <stdio.h>
#include "lcd.h"
#define USE_ADC

#ifndef USE_ADC
//xNew = ecgData[index++];
#else
// AD-Wandler starten,
// xNew den neuen AD-Wert zuweisen
#endif

extern unsigned int ecgData[];
extern unsigned int dataLen;

unsigned int x0 = 0; // x[n] - aktuelle gelesene wert = a 0ms
unsigned int x1 = 0; // x[n-1] = a nach 8ms
unsigned int x2 = 0;// x[n-2] = a nach 16ms
unsigned int x3 = 0;// x[n-3] = a nach 24 ms
unsigned int y1 = 0;// y[n] = x[n-2] -x[n] = x2-x0
unsigned int y2 = 0;//y[n+1] = x[n-3]-x[n-1] = x3-x1
unsigned int n=0; // index
unsigned int ekg = 0;
unsigned int timeStamp = 0, lastQRS = 4;
unsigned int abstand, puls; //abstand
unsigned int count_100ms = 0;

<<<<<<< HEAD
//int xVal=0,xNew=0,  xMiddle=0, xOld=0;
=======
unsigned int abstand; //wie wird es berechnet?

// Algorithmus AF3 -??????
int af3 (unsigned int a){

    for (a=1;a<=dataLen;a++){

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
>>>>>>> 2585ff2cdae4f6d82ef467b09bf9a624c9d63782

/*int isEcg (unsigned int x, unsigned int y){
    unsigned int isTrue = 0;
    if (x>800 && y>800)
    {
        isTrue = 1;
    }
    else
        isTrue = 0;
    return isTrue;
}
 */
void main(void){
    WDTCTL = WDT_MDLY_8; // WD auf 8ms
    SFRIE1 |= WDTIE; // WDT-interrupt freischalten
    _EINT(); // global interrupt enable
    LCD_Init();

    PM5CTL0 &= ~LOCKLPM5;


    while(1) {
        LPM0;
        x3 = x2;
        x2 = x1;
        x1 = x0;

        y1 = x3-x1;
        y2 = x2-x0;


        if (y1> 800 && y2>800)
        {
            LCD_Symbol(LCD_HRT_ID,LCD_SYMBOL_ON);
            if(count_100ms > lastQRS+13)
            {

            abstand = (count_100ms-lastQRS);
            puls= 7500/abstand; // 7500 = 60000/8 ms
            LCD_displayShort(puls);
            lastQRS = count_100ms;
            }
        }
    }
}

#pragma vector=WDT_VECTOR
__interrupt void tick(void){
    x0 = ecgData[n];
    if (n<dataLen){ //
        n++;
    }
    else
        n=0;
    count_100ms++;
    // Time Stamp zu Abstand zwischen QRS berechnen
    /*count_100ms++;
    if (count_100ms==12)
    {
        count_100ms = 0;
        timeStamp++;
    }
*/
    // im interrupt Daten abfragen und abspeichern alle
    LPM0_EXIT;
}
