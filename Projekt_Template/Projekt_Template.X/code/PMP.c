/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Device header file                              */

#include <stdio.h>        
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdlib.h>
#include <string.h>

#include "PMP.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

#include "libpic30.h"      /*Beinhaltet Delay-Funktionen                      */


/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

void initPMP(void)
{
    //Alle Ausg‰nge als Digital
    _ANSB15 = 0;
    ANSELE &= 0xFF00;
    
    //CON
    PMCONbits.PMPEN = 1;
    PMCONbits.PSIDL = 0;
    PMCONbits.ADRMUX = 0b00;
    PMCONbits.PTBEEN = 0;
    PMCONbits.PTWREN = 1;
    PMCONbits.PTRDEN = 1;
    PMCONbits.CSF = 0b00; 
    PMCONbits.ALP = 1;
    PMCONbits.CS2P = 1;
    PMCONbits.CS1P = 1;
    PMCONbits.BEP = 1;
    PMCONbits.WRSP = 1;
    PMCONbits.RDSP = 1;

    //MODE
    PMMODEbits.IRQM   = 0b00;
    PMMODEbits.INCM   = 0b00;
    PMMODEbits.MODE16 = 0;
    PMMODEbits.MODE   = 0b11;   //Master Mode 1
    PMMODEbits.WAITB  = 0b11;
    PMMODEbits.WAITM  = 0b1111;
    PMMODEbits.WAITE  = 0b11;

    //ADRESS ENABLE
    PMAEN = 0x0001;             // PMA0 enabled
    PADCFG1bits.PMPTTL=0;
    delay_ms(40);               //40 ms warten
    
    /**************************************************************************/
    
    PMADDR = 0; //RS auf 0
    PMDIN1 = LCD_CMD_INIT; //Function Set 1
    delay_ms(5);
    PMDIN1 = LCD_CMD_INIT; //Function Set 2
    __delay_cycles(6000); //
    PMDIN1 = LCD_CMD_INIT; //Function Set 3
    __delay_us(38);
    
    PMDIN1 = LCD_FUNCTION_SET; 
    __delay_us(38);
    PMDIN1 = LCD_DISPLAY_OFF; 
    __delay_us(38);
    PMDIN1 = LCD_DISPLAY_CLEAR; 
    __delay_us(1520);
    PMDIN1 = LCD_ENTRY_MODE;
    __delay_us(38);
    PMDIN1 =LCD_DISPLAY_ON;
    __delay_us(38);
    
}/*initPMP()*/


uint8_t lcd_get_status(void)
{
    uint8_t dummy;
    while( PMMODEbits.BUSY); //Warten bis PMP bereit
    PMADDR = 0;
    
    dummy = PMDIN1; //lesen anstoﬂen durch dummy read
    
    while( PMMODEbits.BUSY);
    return (PMDIN1);
    
}/*lcd_get_status()*/


void waitForBusyLCD(void)
{
    while((lcd_get_status() & READ_BUSY_FLAG))
    {
        
    }
    return;

}/*waitForBusyLCD()*/


void lcd_write_data(uint8_t data)
{
    waitForBusyLCD();
    while( PMMODEbits.BUSY ); 
    PMADDR = 1;
    PMDIN1 = data; 
    
}/*lcd_write_data()*/

void writeStrLCD(const char* str)
{
    uint8_t i = 0;
    
    while (str[i]!=0)
    {
        lcd_write_data((str[i]));
        i++;		       
	}
    
}/*writeStrLCD()*/


void lcd_clear(void)
{
    waitForBusyLCD();
    while( PMMODEbits.BUSY ); 
    PMADDR = 0;
    PMDIN1 = LCD_DISPLAY_CLEAR;

}/*lcd_clear()*/


void lcd_set_pos(int line, int pos)
{
    /*Display auf Home Position*/
    waitForBusyLCD();
    while( PMMODEbits.BUSY ); 
    PMADDR = 0;
    PMDIN1 = LCD_DISPLAY_HOME;
    
    /*Berechnung wieivel geshiftet werden muss*/
    
    int i = 0;
    int to_shift = 0;
    
    if (line == 1)          //Erste Linie
    {
        to_shift = pos;
    }
    
    else                    //Zweite Linie
    {
        to_shift = pos + 40;
    }
    
    for(i = 0; i < to_shift - 1; i++)
    {
        waitForBusyLCD();
        while( PMMODEbits.BUSY ); 
        PMADDR = 0;
        PMDIN1 = CURSOR_OR_DISPLAY;
    }
   
}/*lcd_set_pos()*/
