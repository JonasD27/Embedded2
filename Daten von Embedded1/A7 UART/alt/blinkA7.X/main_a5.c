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
#define HEARTBEAT_MS 1
#define MESSZEIT_MS 1000 //gilt nur für HEARTBEAT_MS=10
#define BLINKZEIT 500
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


uint16_t T_blink0 = 600;
uint16_t T_blink1 = 1000;
uint16_t T_blink2 = 1400;
uint16_t T_blink3 = 2200;

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

typedef void *(*StateFunc)();     // Typ für den Zeiger auf eine Statefunktion

// Prototypen der benötigten Statefunktionen
void *FSM1_Idle(void);
void *FSM1_BlinkR_An(void);
void *FSM1_BlinkR_Aus(void);
void *FSM1_BlinkL_An(void);
void *FSM1_BlinkL_Aus(void);

uint16_t blinkvorgaengeL_AN;
uint16_t blinkvorgaengeL_AUS;
uint16_t blinkvorgaengeR_AN;
uint16_t blinkvorgaengeR_AUS;



void *FSM1_Idle(void)
{
    LED0 = LED1 = LED2 = LED3 = 0;
    if (T3 == 1) 
      return FSM1_BlinkL_An;
    else if (T0 == 1) 
      return FSM1_BlinkR_An;
    return FSM1_Idle;
}

void *FSM1_BlinkL_An(void){
    static uint16_t counter = 0;
    counter++;
    if (T0 == 1){
        counter = 0;
        blinkvorgaengeL_AN = 0;
        blinkvorgaengeL_AUS = 0;
        LED3 = LED2 = 0;
        return FSM1_BlinkR_An;
    }
    if ((T3 == 1) || blinkvorgaengeL_AN<3){
        LED3 = LED2 = 1;
        }
    else{
        if (blinkvorgaengeL_AN > 3){
            counter = 0;
            blinkvorgaengeL_AN = 0;
            blinkvorgaengeL_AUS = 0;
            return FSM1_Idle;
        }
        
    }
    if (counter >= BLINKZEIT){
        blinkvorgaengeL_AN++;
        counter = 0;
        return FSM1_BlinkL_Aus; 
        }
    return FSM1_BlinkL_An;

}
void *FSM1_BlinkL_Aus(void){
    static uint16_t counter = 0;
    counter++;
    if (T0 == 1){
        counter = 0;
        blinkvorgaengeL_AUS = 0;
        blinkvorgaengeL_AN = 0;
        LED3 = LED2 = 0;
        return FSM1_BlinkR_An;
    }
    LED3 = LED2 = 0;
    if (T3 == 0){
        if (blinkvorgaengeL_AUS > 3){
            counter = 0;
            blinkvorgaengeL_AUS = 0;
            blinkvorgaengeL_AN = 0;
            return FSM1_Idle;
        }
    }
    if (counter >= BLINKZEIT)
    {
        blinkvorgaengeL_AUS++;
        counter = 0;
        return FSM1_BlinkL_An; 
    }
    return FSM1_BlinkL_Aus;

}
void *FSM1_BlinkR_An(void){
    static uint16_t counter = 0;
    counter++;
    if (T3 == 1){
        counter = 0;
        blinkvorgaengeR_AN = 0;
        blinkvorgaengeR_AUS = 0;
        LED1 = LED0 = 0;
        return FSM1_BlinkL_An;
    }
    if (T0 == 1 || blinkvorgaengeR_AN<3)
        LED1 = LED0 = 1;
    else{
        if (blinkvorgaengeR_AN > 3){
            counter = 0;
            blinkvorgaengeR_AN = 0;
            blinkvorgaengeR_AUS = 0;
            return FSM1_Idle;       
        }
        
    }
    if (counter >= BLINKZEIT){
        blinkvorgaengeR_AN++;
        counter = 0;
        return FSM1_BlinkR_Aus; 
        }
    return FSM1_BlinkR_An;
}
void *FSM1_BlinkR_Aus(void){
    static uint16_t counter = 0;
    counter++;
    if (T3 == 1){
        counter = 0;
        blinkvorgaengeR_AUS = 0;
        blinkvorgaengeR_AN = 0;
        LED1 = LED0 = 0;
        return FSM1_BlinkL_An;
    }
    LED1 = LED0 = 0;
    if (T0 == 0){
        if (blinkvorgaengeR_AUS > 3){
            counter = 0;
            blinkvorgaengeR_AUS = 0;
            blinkvorgaengeR_AN = 0;
            return FSM1_Idle;
        }
    }
    if (counter >= BLINKZEIT)
    {
        blinkvorgaengeR_AUS++;
        counter = 0;
        return FSM1_BlinkR_An; 
    }
    return FSM1_BlinkR_Aus;
}

void KomfortBlink_FSM1(void)
{
     static StateFunc statefunc = FSM1_Idle;
     
     statefunc = (StateFunc)(*statefunc)();
}


int16_t main(void)
{
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
   
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();
    init_ms_t1();
    //init_t2_t3();

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
    _TRISG12 = 1;
    _TRISG13 = 1;
    //Pull-up Widerstände einschalten
    _CNPUG14 = 1;
    _CNPUG15 = 1;
    _CNPUG12 = 1;
    _CNPUG13 = 1;

    uint16_t Count = 0;
    
    //Timer Input auf RP96 mappen
    //_T2CKR = 96;
    
    
    
    while(1)
    {
     //_LATF0 = 0; 
      
        if (_T1IF) {
            _T1IF = 0; //Flag clearen
            Count++;
            if (Count >= HEARTBEAT_MS){ 
                //_LATF0 = 1;
                Count = 0;
                KomfortBlink_FSM1();
                
 
            }
        } 
    }
}
