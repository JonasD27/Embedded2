/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "xc.h"
#include "system.h"
#include "lcd_gpio.h"

/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

void lcd_init()
{
    //Alle Signale als Ausgänge
    _TRISB15 = 0;
    _TRISD5 = 0;
    _TRISD4 = 0;
    TRISE &= 0xFF00;//Datenbusleitung
    
    
   //Alle Ausgänge als Digital
    _ANSB15 = 0;
    ANSELE &= 0xFF00;
   
    LCD_ENABLE = 0; //LCD Aktivierungssignal
    LCD_RS = 0;     //LCD Registerauswahlsignal
    LCD_R_W = 0;    //LCD Daten Lesen oder Schreiben
    
    
    //Function Set 1
    delay_ms(40); //40 ms warten
    LCD_ENABLE = 1;
    LCD_DATA(LCD_CMD_INIT);
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    
    
    //Function Set 2
    __delay_us(4100); //4.1 ms warten
    LCD_ENABLE = 1;
    LCD_DATA(LCD_CMD_INIT);
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    
    
    //Function Set 3
     __delay_us(100); //100 us warten
    LCD_ENABLE = 1;
    LCD_DATA(LCD_CMD_INIT);
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_CMD_INIT benötigt 38 us zum Ausführen 
    
    
    /*************************Alle anderen Anweisungen*************************/
    
    //Function Set
    LCD_ENABLE = 1;
    LCD_DATA(LCD_FUNCTION_SET);
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_FUNCTION_SET benötigt 38 us zum Ausführen
    
    
    //Display off
    LCD_ENABLE = 1;
    LCD_DATA(LCD_DISPLAY_OFF); 
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_DISPLAY_OFF benötigt 38 us zum Ausführen

    
    //Display zurücksetzen
    LCD_ENABLE = 1;
    LCD_DATA(LCD_DISPLAY_CLEAR); 
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(1520);   //LCD_DISPLAY_CLEAR benötigt 1.52 ms zum Ausführen
    

    //Eingabemodus
    LCD_ENABLE = 1;
    LCD_DATA(LCD_ENTRY_MODE); 
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_ENTRY_MODE benötigt 38 us zum Ausführen
    

    //Display on
    LCD_ENABLE = 1;
    LCD_DATA(LCD_DISPLAY_ON);
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_DISPLAY_ON benötigt 38 us zum Ausführen
    
}/*lcd_init()*/


void lcd_write_data(uint8_t data)
{
    LCD_RS = 1;
    LCD_R_W = 0;
    LCD_ENABLE = 1;
    LCD_DATA(data);
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_WRITE benötigt 38 us zum Ausführen 
    
}/*lcd_write_data()*/


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
			lcd_write_data(LCD_ZEICHEN(str[i]));
        }
     
		else
        {
			break;
        }
        
		i++;
        
	}
    
}/*writeStrLCD()*/




