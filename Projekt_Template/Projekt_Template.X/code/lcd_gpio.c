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
    //Alle Signale als Ausg�nge
    _TRISB15 = 0; //RS
    _TRISD5 = 0; //R_W
    _TRISD4 = 0; //Enable
    TRISE &= 0xFF00; //Datenbus
    
    
   //Alle Ausg�nge als Digital
    _ANSB15 = 0;
    ANSELE &= 0xFF00;
   
    LCD_ENABLE = 0; //LCD Aktivierungssignal
    LCD_RS = 0;     //LCD Registerauswahlsignal
    LCD_R_W = 0;    //LCD Daten Lesen oder Schreiben
    
    
    //Function Set 1
    delay_ms(40); //40 ms warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    
    
    //Function Set 2
    __delay_us(4100); //4.1 ms warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    
    
    //Function Set 3
     __delay_us(100); //100 us warten
    LCD_DATA(LCD_CMD_INIT);
    LCD_ENABLE = 1;
    __delay_cycles(33); 
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_CMD_INIT ben�tigt 38 us zum Ausf�hren 
    
    
    /*************************Alle anderen Anweisungen*************************/
    
    //Function Set
    LCD_DATA(LCD_FUNCTION_SET);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_FUNCTION_SET ben�tigt 38 us zum Ausf�hren
    
    
    //Display off
    LCD_DATA(LCD_DISPLAY_OFF); 
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_DISPLAY_OFF ben�tigt 38 us zum Ausf�hren

    
    //Display zur�cksetzen
    LCD_DATA(LCD_DISPLAY_CLEAR);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(1520);   //LCD_DISPLAY_CLEAR ben�tigt 1.52 ms zum Ausf�hren
    

    //Eingabemodus
    LCD_DATA(LCD_ENTRY_MODE); 
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_ENTRY_MODE ben�tigt 38 us zum Ausf�hren
    

    //Display on
    LCD_DATA(LCD_DISPLAY_ON);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_DISPLAY_ON ben�tigt 38 us zum Ausf�hren

}

void lcd_write_data(uint8_t data)
{
    waitForBusyLCD();
    
    LCD_RS = 1;
    LCD_R_W = 0;
    __delay_cycles(4); //80ns 
    LCD_DATA(data);
    LCD_ENABLE = 1;
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_WRITE ben�tigt 38 us zum Ausf�hren 
    
}/*lcd_write_data()*/

/**Sendet den Befehl, das LCD zur�ckzusetzten und den Inhalt zu l�schen.*/
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
    delay_ms(2); // Clear Display braucht 1.5ms  
}


/*Gibt einen String auf dem Display aus*/
void writeStrLCD(const char* str)
{
    uint8_t i = 0;
    
     while (str[i]!=0)
    {
        /* Beinhaltet die ASCII-Tabelle von Leerzeichen bis Tilde mit 
         * Hex = 0x20 - 0x7F oder Dezimal von d032 - d126.
         * Die Zeichen mit den ASCII-Codes 32 bis 126 sind sog. druckbare 
         * Zeichen, die f�r die Anzeige bzw. Ausgabe bestimmt sind.*/
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
    delay_ms(2); // Home Display braucht 1.5ms 
    
    
    
    int i=0;
    int to_shift=0;
    
    if (line==1) //Erste Linie
    {
        to_shift=pos;
    }
    
    else //Zweite Linie
    {
        to_shift=pos+40;
    }
    
    for(i=0; i<to_shift-1; i++)
    {
        LCD_RS = 0;
        LCD_R_W = 0;
        __delay_cycles(4);
        LCD_DATA(0b00010100);
        LCD_ENABLE = 1;
        __delay_cycles(33);
        LCD_ENABLE = 0;
        __delay_us(38);     //LCD_WRITE ben�tigt 38 us zum Ausf�hren 
    }
}

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
    __delay_cycles(4); //80 ns warten??
    LCD_ENABLE = 1;
    __delay_cycles(20); //min 360 ns warten -> 400ns
    received_data = PORTE;
    LCD_ENABLE = 0;
    __delay_us(35); //zur sicherheit 1us warten
    
    TRISE &= 0xFF00; //Datenbus als Ausgang
   
    return received_data;
}

/**
 * Ruft lcd_get_status() zyklisch auf, bis Busy Flag nicht mehr gesetzt ist.
 */
void waitForBusyLCD(void)
{
    while((lcd_get_status() & 0b10000000))
    {
    }
    return;
}


