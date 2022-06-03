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


int16_t main(void)
{
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
   
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();
    init_ms_t1();

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
    while(1)
    {
        //PWM(100,40);
        
        int n = 0;
        int c = 0;
        while (n<= 100){
            while (c < 3){
                PWM(121.212,n);
                c++;
            
            }
            c = 0;
            n++;
        
        }
        LED1 = !LED1;
        n = 100;
        while (n>=0){
            while (c < 3){
                PWM(121.212, n);
                c++;
            }
            c = 0;
            n--;

        }
         
    }
}
