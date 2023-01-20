/*
 * ekg.c
 *
 * Created on: 17.01.2023
 * Author: beatrizaiza & leonwelker
 *
 */
#include <msp430fr6989.h>
#include <math.h>
#include <stdio.h>
#include "lcd.h"
#define USE_ADC

extern unsigned int ecgData[];
extern unsigned int dataLen;

int xValue; // neuster Wert
int x0 = 0; // x[n]   =Wert1
int x1 = 0; // x[n-1] = Wert2
int x2 = 0; // x[n-2] = Wert3
int x3 = 0; // x[n-3] = Wert4
int y1 = 0; // y[n]   = x3 -x1
int y2 = 0; // y[n+1] = x2-x0
unsigned int n=0; // index
unsigned int ekg = 0;
unsigned int lastQRS = 0;//letzter QRS-Komplex
unsigned int abstand, puls; //Abstand zwischen QRS-Komplexen
unsigned int count_100ms = 0; //paraleller Zähler für den Zyklus

void ADC_Init(void) {
    P9SEL0 |= BIT2;  //Bits aktivieren
    P9SEL1 |= BIT2;  //Bits aktivieren
    // interne Referenzspannung 2.5V, Temp. Sensor enable
    REFCTL0 |= REFVSEL_2 // 2.5V
            | REFON;
    ADC12CTL0 = ADC12SHT1_8
            // Sample-and-hold-time: channels 8-23:
            // 256 cycles
            | ADC12SHT0_2
            // Sample-and-hold-time:
            // channels 0-7,24-31: 16 cycles;
            // ADC12ENC=ADC12ON=ADC12SC=0
            ;
    ADC12CTL1 = ADC12PDIV_1 // Predivider 1
            | ADC12SHS_0 // sample-and-hold source select:
            // ADC12SC
            | ADC12SHP
            | ADC12CONSEQ_0 // Single channel/single
            // conversion,
            | ADC12DIV_0 // clock divider 1
            | ADC12SSEL_0;
    // clock source: ADC12OSC (5 MHz)
    // signal not inverted
    ADC12CTL2 = ADC12RES__12BIT; //defines resolution
    ADC12CTL3 = ADC12CSTARTADD_0; // Ergebnis in ADC12MEM0 in KANAL A10
    ADC12MCTL0 =
            // Comparator window disable , single-ended
            ADC12VRSEL_1
            // VR+ = VREF buffered, VR- = AVSS
            // (analog ground)
            | ADC12INCH_10; //A10

    ADC12CTL0 |= ADC12ON + ADC12ENC;
    // Enable conversion + ADC ON
}

void main(void){
    WDTCTL = WDT_MDLY_8; // WD auf 8ms
    SFRIE1 |= WDTIE;     // WDT-interrupt freischalten
    _EINT();             // global interrupt enable
    LCD_Init();
    ADC_Init();

    PM5CTL0 &= ~LOCKLPM5;

    while(1) {
        LPM0;
        x3 = x2;
        x2 = x1;
        x1 = x0;
        x0 = xValue;

        y1 = x3-x1;
        y2 = x2-x0;


        if (y1>800 && y2>800 && (count_100ms-lastQRS) > 13) //100/8 =12,5 ->(13 Zyklen)
        {
            abstand = (count_100ms-lastQRS);
            puls= 7500/abstand; // 7500 = (1 min = 60s = 60.000ms) /8ms  60.000/8ms*distanceQRS
            LCD_displayShort(puls);
            LCD_Symbol(LCD_HRT_ID,LCD_SYMBOL_ON);
            lastQRS = count_100ms;
        }
    }
}

#pragma vector=WDT_VECTOR
__interrupt void tick(void){ //alle 8ms - 100/8= 12,5 -> nächste QRS ist erst nach 12 zyklen möglich

#ifndef USE_ADC
    xValue = ecgData[n];
    count_100ms++; // 1 count = 8ms --> 12counts -- 96ms
    if (n<dataLen){
        n++;
    }
    else
        n=0;

#else
    // AD-Wandler starten,
    ADC12CTL0 |= ADC12SC; // start conversion
    // x0 den neuen AD-Wert zuweisen
    while (ADC12CTL1 & ADC12BUSY); // wait until conversion is made
    xValue = ADC12MEM0;
    count_100ms++; //Zyklenzähler hochzählen alle 8ms
#endif
    LPM0_EXIT;
}
