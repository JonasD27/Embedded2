#include "xc.h"
#include "system.h"
#include "lcd_gpio.h"

void lcd_init()
{
   init_timer2(); //Timer2 setzt alle 20ns Zählregister hoch
   //Alle Signale als Ausgänge
   
   //RS als Ausgang
   _TRISB15=0;
   //R/W als Ausgang
   _TRISD5=0;
   //Enable als Ausgang
   _TRISD4=0;
   
   //Datenbus als Ausgang
   TRISE = TRISE & (0b1111111100000000);
#if 0
   //Datenbus als Eingang
   TRISE = TRISE | (0b0000000011111111);
#endif
   
   //Alle Datenbus-Signale als Digital
   ANSELE = ANSELE & (0b1111111100000000);
   
   //RS auf digital
   _ANSB15=0;
   
   

   delay_ms(50);
   //Function set 1
   LCD_RS=0;
   LCD_R_W=0;
   
   TMR2=0; while( TMR2<4); //80ns warten
   //Schreiben
   LATE = (LATE & 0b1111111100000000) | (0b00111000);
   
#if 0
   //Lesen
   uint8_t data = PORTE & 0b0000000011111111;
#endif
   
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   delay_ms(5);
   
   //Function set 2
   TMR2=0; while( TMR2<4); //80ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   delay_ms(1);
   
   //Function set 3
   TMR2=0; while( TMR2<4); //80ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   
   delay_ms(50);
   
   //Function set
   LATE = (LATE & 0b1111111100000000) | (0b00111000);
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   TMR2=0; while( TMR2<50); //1000ns warten
   
   //Display off
   LATE = (LATE & 0b1111111100000000) | (0b00001000);
   TMR2=0; while( TMR2<1); //20ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   TMR2=0; while( TMR2<50); //1000ns warten
   
   //Display clear
   LATE = (LATE & 0b1111111100000000) | (0b00000001);
   TMR2=0; while( TMR2<1); //20ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   TMR2=0; while( TMR2<50); //1000ns warten
   
   //Entry mode
   LATE = (LATE & 0b1111111100000000) | (0b00000100);
   TMR2=0; while( TMR2<1); //20ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   TMR2=0; while( TMR2<50); //1000ns warten
   delay_ms(5);
   
   
   /********************************************************************/
#if 1
   //Display on
   LATE = (LATE & 0b1111111100000000) | (0b00001111);
   TMR2=0; while( TMR2<1); //20ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   TMR2=0; while( TMR2<50); //1000ns warten
#endif 
   //Display clear
   LATE = (LATE & 0b1111111100000000) | (0b00000001);
   TMR2=0; while( TMR2<1); //20ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
   
   TMR2=0; while( TMR2<50); //1000ns warten
   
   //Schreiben
   LCD_RS=1;
   LCD_R_W=0;
   TMR2=0; while( TMR2<4); //80ns warten
   LATE = (LATE & 0b1111111100000000) | (0b00110000); //"0"
   TMR2=0; while( TMR2<1); //20ns warten
   LCD_ENABLE=1;
   TMR2=0; while( TMR2<25); //500ns warten
   LCD_ENABLE=0;
  
   TMR2=0; while( TMR2<50); //1000ns warten
}
