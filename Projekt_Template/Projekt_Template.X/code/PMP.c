/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Jede Prozessordatei ist geschützt.              */

#include <stdint.h>        /* Enthält uint16_t-Definition                     */
#include <stdbool.h>       /* Enthält eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enthält Zeichenketten                           */
#include <stdio.h>         /* Enhält Ein - und Ausgabefunktionen              */
#include <stdlib.h>        /* Enthält Hilfsfunktionen                         */

#include "system.h"        /* System - Funktion/Parameter                     */
#include "user.h"          /* Benutzer - Funktion/Parameter                   */

#include "libpic30.h"      /* Beinhaltet Delay-Funktionen                     */
#include "PMP.h"           /* Behinhaltet Konstanten und Prototypen           */


/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

/**
 * Initilisierung des PMPs. Drei Function Sets und anschließend die 
 * Konfiguartion des LCDs. Diese beinhaltet das Funktion Set, Display off,
 * Display clear, Entry mode und Display on. 
 */
void initPMP(void)
{
    //Alle Ausgänge als Digital
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
    
    PMADDR = 0;                 //RS auf 0
    
    delay_ms(40);               //40 ms warten
    
    PMDIN1 = LCD_CMD_INIT;      //Function Set 1
    __delay_us(4100);           //4.1 ms warten
    
    PMDIN1 = LCD_CMD_INIT;      //Function Set 2
    __delay_us(100);            //100 us warten
    
    PMDIN1 = LCD_CMD_INIT;      //Function Set 3
    __delay_us(38);             //LCD_CMD_INIT benötigt 38 us zum Ausführen
    
    /*************************Alle anderen Anweisungen*************************/
    
    PMDIN1 = LCD_FUNCTION_SET;
    __delay_us(38);             //LCD_FUNCTION_SET benötigt 38 us zum Ausführen
    
    PMDIN1 = LCD_DISPLAY_OFF; 
    __delay_us(38);             //LCD_DISPLAY_OFF benötigt 38 us zum Ausführen
    
    //LCD_DISPLAY_CLEAR benötigt 1520 us zum Ausführen
    PMDIN1 = LCD_DISPLAY_CLEAR;  
    __delay_us(1520);
    
    PMDIN1 = LCD_ENTRY_MODE;
    __delay_us(38);             //LCD_ENTRY_MODE benötigt 38 us zum Ausführen
    
    PMDIN1 = LCD_DISPLAY_ON;
    __delay_us(38);             //LCD_DISPLAY_ON benötigt 38 us zum Ausführen
    
}/*initPMP()*/

/**
 * Liest den akutellen Status des LCDs aus.
 * @return PMDIN1 wird zum Puffer eingehender Daten verwendet
 */
uint8_t lcd_get_status(void)
{
    uint8_t dummy;
    while( PMMODEbits.BUSY);    //Warten bis PMP bereit
    PMADDR = 0;
    
    dummy = PMDIN1;             //lesen anstoßen durch dummy read
    
    while( PMMODEbits.BUSY);
    return (PMDIN1);
    
}/*lcd_get_status()*/


/**
 * Ruft lcd_get_status() zyklisch auf, bis Busy Flag nicht mehr gesetzt ist.
 */
void waitForBusyLCD(void)
{
    while((lcd_get_status() & READ_BUSY_FLAG))
    {
        
    }
    return;

}/*waitForBusyLCD()*/


/**
 * Schreiben von Daten auf den Speicher des LCDs. Vor dem Zugriff wird Funktion 
 * waitForBusyLCD() solange blockieren, bis Busy Flag nicht gesetzt.
 */
void lcd_write_data(uint8_t data)
{
    waitForBusyLCD();
    while( PMMODEbits.BUSY ); 
    PMADDR = 1;
    PMDIN1 = data; 
    
}/*lcd_write_data()*/


/**
 * Gibt einen String an der aktuellen Position im Display aus.
 * @param str Zeichen
 */
void writeStrLCD(const char* str)
{
    uint8_t i = 0;
    
    while (str[i]!=0)
    {
        lcd_write_data((str[i]));
        i++;		       
	}
    
}/*writeStrLCD()*/


/**
 * Sendet den Befehl, das LCD zurückzusetzten und den Inhalt zu löschen. Vor 
 * dem Zugriff wird Funktion waitForBusyLCD() solange blockieren, bis Busy Flag 
 * nicht gesetzt.
 */
void lcd_clear(void)
{
    waitForBusyLCD();
    while( PMMODEbits.BUSY ); 
    PMADDR = 0;
    PMDIN1 = LCD_DISPLAY_CLEAR;

}/*lcd_clear()*/


/**
 * Setzt die Position des Cursors. Vor dem Zugriff wird Funktion 
 * waitForBusyLCD() solange blockieren, bis Busy Flag nicht gesetzt.
 * @param line Linie, entweder 1 oder 2
 * @param pos Position
 */
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
