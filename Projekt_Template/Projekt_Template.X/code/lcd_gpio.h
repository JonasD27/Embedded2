/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"
//#include "UART.h"
#include <stdint.h>        /* Includes uint16_t definition*/
#include <stdbool.h>       /* Includes true/false definition*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "libpic30.h"       /*Beinhaltet Delay-Funktionen*/


/******************************************************************************/
/* Konstanten                                                                 */
/******************************************************************************/

#define LCD_DATA(d) LATE = (LATE & 0xFF00) | d

/*volatile darf nicht vom Compiler wegoptimiert werden und kann extern 
 *verändert werden z.b. von Interruptroutine oder Timer*/
#define __delay_cycles(cycles)asm volatile ("repeat #%0 \n nop" : : "i" (cycles-2))

#define LCD_RS _RB15        /*LCD Registerauswahlsignal*/
#define LCD_R_W _RD5        /*LCD Registerauswahlsignal*/
#define LCD_ENABLE _RD4     /*LCD Aktivierungssignal*/


#define LCD_CMD_INIT        0b0000000000110000 /*LCD wird Initialliert*/
#define LCD_LEERZEICHEN     0b0000000000010000 /*"Leerzeichen"*/

/*Einstellen der Bewegungsrichtung von Cursor und Display.(8-Bit-Busbetrieb, 
 *2-zeilige Anzeigemodus eingestellt) Benötigt 38us*/
#define LCD_FUNCTION_SET    0b0000000000111000  

/*Schaltet das Display aus. Benötigt 38us*/
#define LCD_DISPLAY_OFF     0b0000000000001000 

/*Löschen der Anzeigedaten. Benötigt 1.52ms*/
#define LCD_DISPLAY_CLEAR   0b0000000000000001

/*Home Position. Benötigt 1.52ms*/
#define LCD_DISPLAY_HOME    0b0000000000000010

/*Eingabemodus eingestellt. Cursor bewegt sich nach Rechts. Benötigt 38us*/
#define LCD_ENTRY_MODE      0b0000000000000110  

/*Schaltet Display und Cursor ein und lässt den Cursor blinken. Benötigt 38us*/
#define LCD_DISPLAY_ON      0b0000000000001111 

/*ASCII-Code von Ausrufezeichen*/
#define LCD_ZEICHEN(hex)    0b0000000000100001 + (hex-0x21)

/******************************************************************************/
/* Prototypen                                                                 */
/******************************************************************************/

/*gemacht*/
void lcd_init(void);
void lcd_write_data(uint8_t data);
void writeStrLCD(const char* str);
void lcd_clear(void);
void lcd_set_pos(int line, int pos);

/*noch machen*/

uint8_t lcd_get_status(void);
uint8_t waitForBusyLCD(void); //Brauch man das?

void lcd_set_write_position(uint8_t pos);

