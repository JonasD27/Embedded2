/**********************************************************************
* ?2007 Microchip Technology Inc.
*
* FileName:        LCD.h
* Dependencies:    Header (.h) files if applicable, see below 
* Processor:       PIC24FJ128GA010
* Compiler:        MPLAB?C30 v3.11 or higher
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
* Mark Pallones		02/25/11		First Release
* JHD                   18.07.2012              kleinere Anpassungen für
*                                               universellen Gebrauch
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

#ifndef __LCD_H
#define __LCD_H
//#include <p24fj128ga010.h>

#define LCDDATA 1
#define LCDCMD  0
#define PMDATA  PMDIN1

void initLCD( void);
char readLCD( int addr);

#define busyLCD() readLCD( LCDCMD) & 0x80
#define addrLCD() readLCD( LCDCMD) & 0x7F
#define getLCD()  readLCD( LCDDATA)


void writeLCD( int addr, char c);
  
    

#define putcharLCD( d)  writeLCD( LCDDATA, (d))
#define cmdLCD( c)  writeLCD( LCDCMD, (c))

#define homeLCD()   writeLCD( LCDCMD, 2)    
#define clrLCD()    writeLCD( LCDCMD, 1)
#define setLCDG( a) writeLCD( LCDCMD, ((a) & 0x3F) | 0x40)
#define setLCDC( a) writeLCD( LCDCMD, ((a) & 0x7F) | 0x80)
#define dispsecLine(a)  writeLCD (LCDCMD,((a) & 0x0f)| 0xc0)
#define dispfirstLine(a)  writeLCD (LCDCMD,((a) & 0x0f)| 0x80)


void putstringLCD( const char *s);


#define TFLY 9000       // 9000 x 16us = 144ms
#define DELAY() TMR1=0; while( TMR1<TFLY)


#endif

