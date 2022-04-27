#include "xc.h"
#include "system.h"
#include "lcd_gpio.h"

void lcd_init()
{
   init_timer2(); //Timer2 setzt alle 20ns Zählregister hoch
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

   delay_ms(50);
   //Function set 1
   LCD_RS=0;
   LCD_R_W=0;
   
   TMR2=0; while( TMR2<4); //80ns warten
   LATE = (LATE & 0b1111111100000000) | (0b00111000);
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
