/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
#define HEARTBEAT_MS 10
#define MESSZEIT_MS 1000 //gilt nur für HEARTBEAT_MS=10
/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

/**
 * Software Delay mit for Schleife, Blockierung
 * 167 muss evtl. angepasst werden
 *
 * @param milliseconds Wartezeit, blockierend
 * @todo Anpassung an unterschiedliche Systemfrequenzen
 */

void delay_ms_t1(uint16_t milliseconds){
    int Count = 0;
    while(Count < milliseconds){
        while (! _T1IF){
            Nop();
            }
    
        _T1IF = 0;
        Count++;
        }   
    }


uint32_t DELAY_ANPASSUNG;

void delay_ms(uint16_t milliseconds) {    
    uint32_t i=0;
    for (i=0;i<(DELAY_ANPASSUNG*(uint32_t)milliseconds);i++){
         }
}

void PWM(float freq, float tast){
    if (tast==100) {
        LED0=1;
        PR1 = ((tast/(freq*100))*FCY/64)-1;
        delay_ms_t1(1);
    }   
    else if (tast == 0) {
        LED0=0;
        PR1 = ((100/(freq*100))*FCY/64)-1;
        delay_ms_t1(1);
    }
    else {
    LED0=1;
    PR1 = ((tast/(freq*100))*FCY/64)-1; 
    delay_ms_t1(1);
    LED0=0;
    PR1 = (((1/freq)*(1-tast/100))*FCY/64)-1;
    delay_ms_t1(1);
    }
}




uint16_t T_blink0 = 600;
uint16_t T_blink1 = 1000;
uint16_t T_blink2 = 1400;
uint16_t T_blink3 = 2200;

int c0,c1,c2,c3 = 0;
void Blink0(){ //T=600ms
    c0++;
    if (c0>=(T_blink0/(2*HEARTBEAT_MS))){
        TLED0();
        c0=0;
    }
}
void Blink1(){ //T=1000ms
    c1++;
    if (c1>=(T_blink1/(2*HEARTBEAT_MS))){
        TLED1();
        c1=0;
    }
}
void Blink2(){ //T=1400ms
    c2++;
    if (c2>=(T_blink2/(2*HEARTBEAT_MS))){
        TLED2();
        c2=0;
    }
}
void Blink3(){ //T=2200ms
    c3++;
    if (c3>=(T_blink3/(2*HEARTBEAT_MS))){
        TLED3();
        c3=0;
    }
}

void Taster0(){
    static uint16_t pressedtime = 0;
    static bool last_state = 0;
    if (T0) {
        last_state = 1;
        pressedtime = pressedtime + 10;          
        }
    if(T0 != last_state){
            last_state = 0;
            if (pressedtime > 30) {
                T_blink0 = 2*pressedtime;   
                }          
            pressedtime = 0;   
    }
}
void Taster1(){
    static uint16_t pressedtime = 0;
    static bool last_state = 0;
    if (T1) {
        last_state = 1;
        pressedtime = pressedtime + 10;          
        }
    if(T1 != last_state){
            last_state = 0;
            if (pressedtime > 30) {
                T_blink1 = 2*pressedtime;   
                }  
            pressedtime = 0;
        
    }
}
void Taster2(){
    static uint16_t pressedtime = 0;
    static bool last_state = 0;
    if (T2) {
        last_state = 1;
        pressedtime = pressedtime + 10;          
        }
    if(T2 != last_state){
            last_state = 0;
            if (pressedtime > 30) {
                T_blink2 = 2*pressedtime;   
                }  
            pressedtime = 0;
        
    }
}
void Taster3(){
    static uint16_t pressedtime = 0;
    static bool last_state = 0;
    if (T3) {
        last_state = 1;
        pressedtime = pressedtime + 10;          
        }
    if(T3 != last_state){
            last_state = 0;
            if (pressedtime > 30) {
                T_blink3 = 2*pressedtime;   
                }  
            pressedtime = 0;
        
    }
}

void measureProcesstime(){
    static uint16_t time = 0;
    time++;
    if (time >= MESSZEIT_MS){
        float Auslastung;
        uint16_t lsw = TMR2;
        uint16_t msw = TMR3HLD;
        
        float Auslastung1 = (float)lsw * 10;
        float Auslastung2 = (float)msw *65535*10;
        Auslastung = (Auslastung1 + Auslastung2) / (FCY);
        time = 0;
        TMR3HLD=0;
        TMR3 = 0; // Clear 32-bit Timer (msw)
        TMR2 = 0;
    }
}



int16_t main(void)
{
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
   
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();
    init_ms_t1();
    init_t2_t3();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    
    //RB8 als Ausgang, TRIS register auf 0 setzten, Adresse 0E10
    //TRISB = ~_TRISB_TRISB8_MASK 
    //TRISB = 0b1111111011111111;
    //TRISBbits.TRISB8 = 0;
    //Alle LEDs als Ausgang
    _TRISB8 = 0;
    _TRISB9 = 0;
    _TRISB10 = 0;
    _TRISB11 = 0;
    _TRISF0 = 0;
    //Alle LEDs als digital
    _ANSB8 = 0;
    _ANSB9 = 0;
    _ANSB10 = 0;
    _ANSB11 = 0;
    //Taster als Eingang
    _TRISG14 = 1;
    _TRISG15 = 1;
    //Pull-up Widerstände einschalten
    _CNPUG14 = 1;
    _CNPUG15 = 1;

    uint16_t Count = 0;
    
    //Timer Input auf RP96 mappen
    _T2CKR = 96;
    while(1)
    {
     _LATF0 = 0; 
      
        if (_T1IF) {
            _T1IF = 0; //Flag clearen
            Count++;
            if (Count >= HEARTBEAT_MS){ 
                _LATF0 = 1;
                Count = 0;
              
                Blink0();  
                Blink1();
                Blink2();
                Blink3();

                Taster0();
                Taster1();
                Taster2();
                Taster3();
                measureProcesstime();
                 
            }
        } 
    }
}
