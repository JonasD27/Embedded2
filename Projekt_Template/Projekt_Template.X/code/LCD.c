/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        lcdfunc.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC24FJ128GA010
* Compiler:        MPLAB C30 v3.11 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all 
* ownership and intellectual property rights in the code accompanying
* this message and in all derivatives hereto.  You may use this code,
* and any derivatives created by any person or entity by or on your 
* behalf, exclusively with Microchip's proprietary products.  Your 
* acceptance and/or use of this code constitutes agreement to the 
* terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS". NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT 
* NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, 
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS 
* CODE, ITS INTERACTION WITH MICROCHIP'S PRODUCTS, COMBINATION WITH 
* ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE 
* LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
* BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, 
* CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
* EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR 
* EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER 
* CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE
* DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, 
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP 
* SPECIFICALLY TO HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify,
* test, certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      		Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Mark Pallones		02/23/11		
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* Code Tested on:
* Explorer 16 Demo board with  PIC24FJ128GA010 controller
*
* DESCRIPTION:
* This code example describes how to use PMP on PIC24F. Here I display
* a message on LCD via PMP Module. 
*********************************************************************/

/* Device header file */
#if defined(__PIC24E__)
	#include <p24Exxxx.h>
#elif defined(__PIC24F__)
	#include <p24Fxxxx.h>
#elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
#elif defined(__dsPIC33E__)
	#include <p33Exxxx.h>
#elif defined(__dsPIC33F__)
	#include <p33Fxxxx.h>
#endif

#include "LCD.h"


void initLCD( void)
{
    // PMP initialization 
    //PMCON = 0x83BF;             // Enable the PMP, long waits
            //0b1000 0011 1011 1111
    PMCONbits.PMPEN = 1;
    PMCONbits.PSIDL = 0;
    PMCONbits.ADRMUX = 0b00;
    PMCONbits.PTBEEN = 0;
    PMCONbits.PTWREN = 1;
    PMCONbits.PTRDEN = 1;
    PMCONbits.CSF = 0b00; 
    PMCONbits.ALP = 1;
    PMCONbits.CS2P = 0;
    PMCONbits.CS1P = 0;
    PMCONbits.BEP = 1;
    PMCONbits.WRSP = 1;
    PMCONbits.RDSP = 1;
    
    //PMMODE = 0x3FF;             // Master Mode 1
             //0b0000 0011 1111 1111
    PMMODEbits.IRQM   = 0b00;
    PMMODEbits.INCM   = 0b00;
    PMMODEbits.MODE16 = 0;
    PMMODEbits.MODE   = 0b11; //Master Mode 1
    PMMODEbits.WAITB  = 0b11;
    PMMODEbits.WAITM  = 0b1111;
    PMMODEbits.WAITE  = 0b11;

    PMAEN = 0x0001;             // PMA0 enabled



    // init TMR1
    T1CON = 0x8030;             // Fosc/2, prescaled 1:256, 16us/tick

#define Tmul 4
    #define T32ms 2000*Tmul
    #define T48us 3*Tmul
    #define T1_6ms 100*Tmul

    // wait for >30ms
    TMR1 = 0; while( TMR1<T32ms);// 2000 x 16us = 32ms
    
    //initiate the HD44780 display 8-bit init sequence
    PMADDR = LCDCMD;            // command register
    PMDATA = 0b00111000;        // 8-bit interface, 2 lines, 5x7
    TMR1 = 0; while( TMR1<T48us);   // 3 x 16us = 48us
    
    PMDATA = 0b00001100;        // display ON, cursor off, blink off
    TMR1 = 0; while( TMR1<T48us);   // 3 x 16us = 48us
    
    PMDATA = 0b00000001;        // clear display
    TMR1 = 0; while( TMR1<T1_6ms); // 100 x 16us = 1.6ms
    
    PMDATA = 0b00000110;        // increment cursor, no shift
    TMR1 = 0; while( TMR1<T1_6ms); // 100 x 16us = 1.6ms
} // initLCD


char readLCD( int addr)   // naddr = 0/1
{
    int dummy;
    while( PMMODEbits.BUSY);    // wait for PMP to be available
    PMADDR = addr;              // select the command address
    dummy = PMDATA;            // initiate a read cycle, dummy read
     while( PMMODEbits.BUSY);    // wait for PMP to be available
    return( PMDATA);            // read the status register
    
} // readLCD



void writeLCD( int addr, char c)    
{
    while( busyLCD() );
    while( PMMODEbits.BUSY );    // wait for PMP to be available
    PMADDR = addr;
    PMDATA = c;
} // writeLCD
    


void putstringLCD( const char *s)
{
    while( *s) putcharLCD( *s++);
} //putstringLCD

