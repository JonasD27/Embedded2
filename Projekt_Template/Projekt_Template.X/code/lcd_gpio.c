/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

//TODO: Logic Analyzer Doku mit Stuersignalen, Bit7 +...

#include <xc.h>            /* Jede Prozessordatei ist geschützt.              */

#include "system.h"        /* System - Funktion/Parameter                     */

#include "lcd_gpio.h"      /* Behinhaltet Konstanten und Prototypen           */

/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

/**
 * Initilisierung des LCDs. Drei Function Sets und anschließend die 
 * Konfiguartion des LCDs. Diese beinhaltet das Funktion Set, Display off,
 * Display clear, Entry mode und Display on. 
 */
void lcd_init()
{
    //Alle Signale als Ausgänge
    _TRISB15 = 0;           //RS
    _TRISD5 = 0;            //R_W
    _TRISD4 = 0;            //Enable
    TRISE &= 0xFF00;        //Datenbus als Ausgang
    
    //Alle Ausgänge als Digital
    _ANSB15 = 0;
    ANSELE &= 0xFF00;
   
    LCD_ENABLE = 0;         //LCD Aktivierungssignal
    LCD_RS = 0;             //LCD Registerauswahlsignal
    LCD_R_W = 0;        //LCD Daten Lesen oder Schreiben
    
    
    //Function Set 1
    delay_ms(40);           //40 ms warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    
    
    //Function Set 2
    __delay_us(4100);       //4.1 ms warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    
    
    //Function Set 3
     __delay_us(100);       //100 us warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten 
    LCD_ENABLE = 0;
    __delay_us(38);         //LCD_CMD_INIT benötigt 38 us zum Ausführen 
    
    
    /*************************Alle anderen Anweisungen*************************/
    
    
    /*LCD_FUNCTION_SET stellt die Bewegungsrichtig von Cursor und Display ein.*/
    LCD_DATA(LCD_FUNCTION_SET);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    __delay_us(38);         //LCD_FUNCTION_SET benötigt 38 us zum Ausführen
    
    
    /*LCD_DISPLAY_OFF schaltet das Display aus.*/
    LCD_DATA(LCD_DISPLAY_OFF); 
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    __delay_us(38);         //LCD_DISPLAY_OFF benötigt 38 us zum Ausführen

    
    /*LCD_DISPLAY_CLEAR löscht die Anzeigedaten.*/
    LCD_DATA(LCD_DISPLAY_CLEAR);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    __delay_us(1520);       //LCD_DISPLAY_CLEAR benötigt 1.52 ms zum Ausführen
    

    /*LCD_ENTRY_MODE stellt den Eingabemodus ein. Der Cursor bewegt sich dabei
     *nach rechts.*/
    LCD_DATA(LCD_ENTRY_MODE); 
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    __delay_us(38);         //LCD_ENTRY_MODE benötigt 38 us zum Ausführen
    

    /*LCD_DISPLAY_ON schaltet das Display ein.*/
    LCD_DATA(LCD_DISPLAY_ON);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    __delay_us(38);         //LCD_DISPLAY_ON benötigt 38 us zum Ausführen

}/*lcd_init()*/


/**
 * Schreiben von Daten auf den Speicher des LCDs. Vor dem Zugriff wird Funktion 
 * waitForBusyLCD() solange blockieren, bis Busy Flag nicht gesetzt.
 */
void lcd_write_data(uint8_t data)
{
    waitForBusyLCD();
    TRISE &= 0xFF00;        //Datenbus als Ausgang
    
    LCD_RS = 1;
    LCD_R_W = 0;
    __delay_cycles(4);      //80 ns warten
    LCD_DATA(data);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0; 
    
}/*lcd_write_data()*/


/**
 * Sendet den Befehl, das LCD zurückzusetzten und den Inhalt zu löschen. Vor 
 * dem Zugriff wird Funktion waitForBusyLCD() solange blockieren, bis Busy Flag 
 * nicht gesetzt.
 */
void lcd_clear(void)
{
    waitForBusyLCD();
    TRISE &= 0xFF00;        //Datenbus als Ausgang
    
    LCD_RS = 0;
    LCD_R_W = 0;
    __delay_cycles(4);      //80 ns warten
    
    /*LCD_DISPLAY_CLEAR löscht die Anzeigedaten.*/
    LCD_DATA(LCD_DISPLAY_CLEAR);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    
}/*lcd_clear()*/


/**
 * Gibt einen String an der aktuellen Position im Display aus.
 * @param str Zeichenkette
 */
void writeStrLCD(const char* str)
{
    uint8_t i = 0;
    
    while (str[i]!=0)       //Solange es etwas zu schreiben gibt.
    {
        lcd_write_data((str[i]));
        i++;      
	}
    
}/*writeStrLCD()*/

/**
 * Setzt die Position des Cursors. Vor dem Zugriff wird Funktion 
 * waitForBusyLCD() solange blockieren, bis Busy Flag nicht gesetzt.
 * @param line Linie, entweder 1 oder 2
 * @param pos Position
 */
void lcd_set_pos(int line, int pos)
{
    waitForBusyLCD();
    TRISE &= 0xFF00;        //Datenbus als Ausgang
    
    //Position auf 0,0 setzten
    LCD_RS = 0;
    LCD_R_W = 0;
    __delay_cycles(4);      //80 ns warten
    
    /* LCD_DISPLAY_HOME setzt Cursor und Anzeige an ihren ursprünglichen Zustand 
     * zurück.*/
    LCD_DATA(LCD_DISPLAY_HOME);
    LCD_ENABLE = 1;
    __delay_cycles(33);     //660 ns warten
    LCD_ENABLE = 0;
    
     
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
        TRISE &= 0xFF00;    //Datenbus als Ausgang
        
        LCD_RS = 0;
        LCD_R_W = 0;
        __delay_cycles(4);  //80 ns warten
        
        /*CURSOR_OR_DISPLAY schiebt Cursor und Anzeige nach rechts (der Cursor 
         *bewegt sich entsprechend der Anzeige).*/
        LCD_DATA(CURSOR_OR_DISPLAY);
        LCD_ENABLE = 1;
        __delay_cycles(33); //660 ns warten
        LCD_ENABLE = 0; 
    }
    
}/*lcd_set_pos()*/


/**
 * Liest den akutellen Status des LCDs aus.
 * @return 8bit breiter Status, Bit 7 ist Busy Flag
 */
uint8_t lcd_get_status(void)
{
    uint8_t received_data;
    TRISE = TRISE | 0x00FF; //Datenbus als Eingang
    
    LCD_RS = 0;
    LCD_R_W = 1;
    __delay_cycles(4);      //80 ns warten
    LCD_ENABLE = 1;
    __delay_cycles(20);     //min 360 ns warten -> 400ns
    received_data = PORTE;
    LCD_ENABLE = 0;
    
    return received_data;
    
}/*lcd_get_status()*/


/**
 * Ruft lcd_get_status() zyklisch auf, bis Busy Flag nicht mehr gesetzt ist.
 */
void waitForBusyLCD(void)
{
    /* READ_BUSY_FLAG definiert den IC und gibt an das dieser in Betrieb ist. 
     * Interne Operation ist im Gange es soll gewartet werden.*/
    while((lcd_get_status() & READ_BUSY_FLAG))
    {
        
    }
    return;
    
}/*waitForBusyLCD()*/