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


#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "system.h"          /* variables/params used by system.c             */

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c.                                          */
/*                                                                            */
/******************************************************************************/

/* Refer to the device Family Reference Manual Oscillator section for
information about available oscillator configurations.  Typically
this would involve configuring the oscillator tuning register or clock
switching useing the compiler's __builtin_write_OSCCON functions.
Refer to the C Compiler for PIC24 MCUs and dsPIC DSCs User Guide in the
compiler installation directory /doc folder for documentation on the
__builtin functions.*/
#pragma config FNOSC = FRC
#pragma config IESO = ON
#pragma config FCKSM = CSECMD
#pragma config OSCIOFNC = OFF
#pragma config POSCMD = XT
#pragma config FWDTEN = OFF
/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void)
{
    if (SYS_FREQ>7370000L){//Nur umschalten auf Primary wenn h?here Frequenz erw?nscht
        switch (SYS_FREQ){
            case 8000000L:
                //PLL muss nicht konfiguriert werden
                // externer Quartz mit 8Mhz
                break;
            case 50000000L:
                CLKDIVbits.PLLPOST=2; //N2=4
                PLLFBD=48; //M=50
                CLKDIVbits.PLLPRE=0; //N1=2
                break;
            case 100000000L:
                CLKDIVbits.PLLPOST=0; //N2=2
                PLLFBD=123; //M=125
                CLKDIVbits.PLLPRE=3; //N1=5
                break;
            case 140000000L:
                CLKDIVbits.PLLPOST=0; //N2=2
                PLLFBD=173; //M=175
                CLKDIVbits.PLLPRE=3; //N1=5
                break;
            //default:
                //#error Tets
        }
        OSCTUN = 0;
        
        if (SYS_FREQ == 8000000L){
            __builtin_write_OSCCONH(0x02); //Switch auf Primary ohne PLL
            __builtin_write_OSCCONL(OSCCON | 0x01);
            OSCCONbits.LPOSCEN = 1; //Secondary Oscillator startetn f?r Timer1  
            while (OSCCONbits.COSC!= 0x02); //Warten bis gewechselt wurde
            }
        
        else {
            __builtin_write_OSCCONH(0x03); //Switch auf Primary mit PLL
            __builtin_write_OSCCONL(OSCCON | 0x01);
            OSCCONbits.LPOSCEN = 1; //Secondary Oscillator startetn f?r Timer1  
            while (OSCCONbits.COSC!= 0x3); //Warten bis gewechselt wurde
            while (OSCCONbits.LOCK!= 1);
        }

    }
}

void init_ms_t1(void){
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCS = 0; // Select internal clock
    T1CONbits.TSYNC = 0; // Disable Synchronization -> Asynchron
    T1CONbits.TCKPS = 0b10; // Select 1:64 Prescaler
    TMR1 = 0x00; // Clear timer register
    PR1 = (FCY/64000)-1; // Load the period value hier 1ms
   /*
    IPC0bits.T1IP = 0x01; // Set Timer 1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
   */
    T1CONbits.TON = 1; // Start Timer



}

void init_t2_t3(){
    T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
    T2CONbits.T32 = 1; // Enable 32-bit Timer mode
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TGATE = 1; // Enable Gated Timer mode
    T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR3 = 0x00; // Clear 32-bit Timer (msw)
    TMR2 = 0x00; // Clear 32-bit Timer (lsw)
    PR3 = 0xFFFF; // Load 32-bit period value (msw)
    PR2 = 0xFFFF; // Load 32-bit period value (lsw)
    IPC2bits.T3IP = 0x01; // Set Timer3 Interrupt Priority Level
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    IEC0bits.T3IE = 0; // Enable Timer3 interrupt
    T2CONbits.TON = 1; // Start 32-bit Timer
}
