/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "xc.h"

#include "system.h"/* System funct/params, like osc/peripheral config         */

#include "lcd_gpio.h"

/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

void lcd_init()
{
    //Alle Signale als Ausgänge
    _TRISB15 = 0;       //RS
    _TRISD5 = 0;        //R_W
    _TRISD4 = 0;        //Enable
    TRISE &= 0xFF00;    //Datenbus
    
    
    //Alle Ausgänge als Digital
    _ANSB15 = 0;
    ANSELE &= 0xFF00;
   
    LCD_ENABLE = 0;     //LCD Aktivierungssignal
    LCD_RS = 0;         //LCD Registerauswahlsignal
    LCD_R_W = 0;        //LCD Daten Lesen oder Schreiben
    
    
    //Function Set 1
    delay_ms(40);       //40 ms warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    
    
    //Function Set 2
    __delay_us(4100);   //4.1 ms warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    
    
    //Function Set 3
     __delay_us(100);   //100 us warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_CMD_INIT benötigt 38 us zum Ausführen 
    
    
    /*************************Alle anderen Anweisungen*************************/
    
    //Function Set
    LCD_DATA(LCD_FUNCTION_SET);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_FUNCTION_SET benötigt 38 us zum Ausführen
    
    
    //Display off
    LCD_DATA(LCD_DISPLAY_OFF); 
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_DISPLAY_OFF benötigt 38 us zum Ausführen

    
    //Display zurücksetzen
    LCD_DATA(LCD_DISPLAY_CLEAR);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(1520);   //LCD_DISPLAY_CLEAR benötigt 1.52 ms zum Ausführen
    

    //Eingabemodus
    LCD_DATA(LCD_ENTRY_MODE); 
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_ENTRY_MODE benötigt 38 us zum Ausführen
    

    //Display on
    LCD_DATA(LCD_DISPLAY_ON);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_DISPLAY_ON benötigt 38 us zum Ausführen

}/*lcd_init()*/


void lcd_write_data(uint8_t data)
{
    waitForBusyLCD();
    
    LCD_RS = 1;
    LCD_R_W = 0;
    __delay_cycles(4);  //80ns 
    LCD_DATA(data);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_WRITE benötigt 38 us zum Ausführen 
    
}/*lcd_write_data()*/


/**Sendet den Befehl, das LCD zurückzusetzten und den Inhalt zu löschen.*/
void lcd_clear(void)
{
    waitForBusyLCD();
    
    LCD_RS = 0;
    LCD_R_W = 0;
    __delay_cycles(4);
    LCD_DATA(LCD_DISPLAY_CLEAR);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(1520);   //LCD_DISPLAY_CLEAR benötigt 1.52 ms zum Ausführen
    
}/*lcd_clear()*/


/*Gibt einen String auf dem Display aus*/
void writeStrLCD(const char* str)
{
    uint8_t i = 0;
    
    while (str[i]!=0)
    {
        /* Beinhaltet die ASCII-Tabelle von Leerzeichen bis Tilde mit 
         * Hex = 0x20 - 0x7F oder Dezimal von d032 - d126.
         * Die Zeichen mit den ASCII-Codes 32 bis 126 sind sog. druckbare 
         * Zeichen, die für die Anzeige bzw. Ausgabe bestimmt sind.*/
        if (str[i]>=' ' && str[i]<='~')
        {
			lcd_write_data((str[i]));
        }
     
		else
        {
			break;
        }
        
		i++;       
	}
    
}/*writeStrLCD()*/

/**
 * Setzt die Position des Cursors
 * @param line Linie, entweder 1 oder 2
 * @param pos Position
 */
void lcd_set_pos(int line, int pos)
{
    waitForBusyLCD();
    
    //Position auf 0,0 setzten
    LCD_RS = 0;
    LCD_R_W = 0;
    __delay_cycles(4);
    LCD_DATA(LCD_DISPLAY_HOME);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(1520);       //LCD_DISPLAY_HOME benötigt 1.52 ms zum Ausführen 
     
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
        LCD_RS = 0;
        LCD_R_W = 0;
        __delay_cycles(4);
        LCD_DATA(0b00010100);
        LCD_ENABLE = 1;
        __delay_cycles(33);
        LCD_ENABLE = 0;
        __delay_us(38);     //??? benötigt 38 us zum Ausführen 
    }
    
}/*lcd_set_pos()*/


/**
 * Liest den akutellen Status des LCDs aus.
 * @return 8bit breiter Status, Bit 7 ist Busy Flag
 */
uint8_t lcd_get_status(void)
{
    uint8_t received_data;
    
    //Datenbus als Eingang
    TRISE = TRISE | 0x00FF;
    
    LCD_RS = 0;
    LCD_R_W = 1;
    __delay_cycles(4);      //80 ns warten??
    LCD_ENABLE = 1;
    __delay_cycles(20);     //min 360 ns warten -> 400ns
    received_data = PORTE;
    LCD_ENABLE = 0;
    __delay_us(35);         //zur sicherheit 1us warten
    
    TRISE &= 0xFF00;        //Datenbus als Ausgang
   
    return received_data;
    
}/*lcd_get_status()*/


/**
 * Ruft lcd_get_status() zyklisch auf, bis Busy Flag nicht mehr gesetzt ist.
 */
void waitForBusyLCD(void)
{
    while((lcd_get_status() & 0b10000000))
    {
        
    }
    return;
    
}/*waitForBusyLCD()*/


