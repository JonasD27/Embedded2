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
uint32_t u32_uptime_seconds = 0;
uint32_t u32_inactive_seconds = 0;

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

//Interrupt Service Routine
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
    {
        _T1IF = 0; //Clear Timer1 interrupt flag
        u32_uptime_seconds++;
        u32_inactive_seconds++;
        if (u32_inactive_seconds >= 10){
        Sleep();
        }
        
        
    }

 void __attribute__((interrupt,auto_psv)) _CNInterrupt(void)               
 {                                                                          
     _CNIF = 0;
  
     u32_inactive_seconds = 0;
 }

void initBetriebszeit(uint32_t betriebszeit){
    u32_uptime_seconds = betriebszeit;
    __builtin_write_OSCCONL(0b00000011); //SOSC aktivieren
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCS = 1; // Select external clock
    T1CONbits.TSYNC = 0; // Disable Synchronization
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR1 = 0x00; // Clear timer register
    PR1 = 32767; // Load the period value, Quarztakt
    
    IPC0bits.T1IP = 2; // Set Timer 1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T1CONbits.TON = 1; // Start Timer
}

void setBetriebszeit(uint32_t betriebszeit){
    _T1IE = 0;
    u32_uptime_seconds = betriebszeit;
    _T1IE = 1;
}

uint32_t getBetriebszeit(void){
    _T1IE = 0;
    uint32_t temp = u32_uptime_seconds;
    _T1IE = 1;
    return temp;
}

void ShowBetreibszeitLED(){
    static uint16_t counter;
    counter++;
    if (counter == 980){
        LATB = ((0<<8) | (0<<9) | (0<<10) | (0<<11));
        LATB |= ((!(bool)(u32_uptime_seconds & 0b00000100) << 8) | (!(bool)(u32_uptime_seconds & 0b00001000) << 9) | (!(bool)(u32_uptime_seconds & 0x10) << 10) | (!(bool)(u32_uptime_seconds & 0x20) << 11)); 

    }
    if (counter == 1000){
        counter =0;
        LATB = ((0<<8) | (0<<9) | (0<<10) | (0<<11));
        LATB |= (((bool)(u32_uptime_seconds & 0b00000100) << 8) | ((bool)(u32_uptime_seconds & 0b00001000) << 9) | ((bool)(u32_uptime_seconds & 0x10) << 10) | ((bool)(u32_uptime_seconds & 0x20) << 11)); 
    }

}

int16_t main(void)
{
    //Sleep();
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
   
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();
    //init_ms_t1();
    init_ms_t2();
    initBetriebszeit(0);

    
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
    //Taster INC G9 als digital
    _ANSG9 = 0;
    //Taster als Eingang
    _TRISG14 = 1;
    _TRISG15 = 1;
    _TRISG12 = 1;
    _TRISG13 = 1;
    _TRISG9 = 1;
    //Pull-up Widerstände einschalten
    _CNPUG14 = 1;
    _CNPUG15 = 1;
    _CNPUG12 = 1;
    _CNPUG13 = 1;
    _CNPUG9 = 1;

    uint16_t Count = 0;
    
    //Timer Input auf RP96 mappen
    //_T2CKR = 96;
    
    _CNIE = 1; //Change Notification Interrupt Enable 
    _CNIP = 1; // Interupt priority heruntersetzten
    _CNIEG14 = 1; //CN für Taster aktivieren
    _CNIEG15 = 1;
    _CNIEG12 = 1;
    _CNIEG13 = 1;
    _CNIEG9 = 1;
    while(1)
    {
     //_LATF0 = 0; 
      
        if (_T2IF) {
            _T2IF = 0; //Flag clearen
            Count++;
            if (Count >= HEARTBEAT_MS){ 
                //_LATF0 = 1;
                Count = 0;
                ShowBetreibszeitLED();
                //doIdle();
            }
        } 
    }
}
