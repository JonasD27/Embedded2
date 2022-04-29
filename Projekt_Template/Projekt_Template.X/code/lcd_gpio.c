#include "xc.h"
#include "system.h"
#include "lcd_gpio.h"

#include "libpic30.h"


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

void lcd_init()
{
#if 1
    
    //Alle Signale als Ausgänge
   _TRISB15=0;
   _TRISD5=0;
   _TRISD4=0;
   _TRISE0=0;
   _TRISE1=0;
   _TRISE2=0;
   _TRISE3=0;
   _TRISE4=0;
   _TRISE5=0;
   _TRISE6=0;
   _TRISE7=0;
   //Alle Ausgänge als Digital
   _ANSB15=0;
   _ANSE0=0;
   _ANSE1=0;
   _ANSE2=0;
   _ANSE3=0;
   _ANSE4=0;
   _ANSE5=0;
   _ANSE6=0;
   _ANSE7=0;
   
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
    
    
    //Schreiben
    LCD_RS=1;
    LCD_R_W=0;
    LCD_ENABLE = 1;
    LCD_DATA(LCD_WRITE);
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_WRITE benötigt 38 us zum Ausführen
    
      //Schreiben
    LCD_RS=1;
    LCD_R_W=0;
    LCD_ENABLE = 1;
    LCD_DATA('Y');
    __delay_cycles(33);
    LCD_ENABLE = 0;
    __delay_us(38);     //LCD_WRITE benötigt 38 us zum Ausführen
#endif
 
}
