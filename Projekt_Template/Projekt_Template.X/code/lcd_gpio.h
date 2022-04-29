#include "user.h"
//#include "UART.h"
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include "libpic30.h"

#define LCD_DATA(d) LATE = (LATE & 0xFF00) | d

#define __delay_cycles(cycles)          asm volatile ("repeat #%0 \n nop" : : "i" (cycles-2) )

#define LCD_RS _RB15
#define LCD_R_W _RD5
#define LCD_ENABLE _RD4

#define LCD_CMD_INIT        0b0000000000110000  //LCD wird Initialliert
#define LCD_FUNCTION_SET    0b0000000000111000  //Einstellen der Bewegungsrichtung von Cursor und Display.(8-Bit-Busbetrieb, 2-zeilige Anzeigemodus eingestellt) Benötigt 38us
#define LCD_DISPLAY_OFF     0b0000000000001000  //Schaltet das Display aus. Benötigt 38us
#define LCD_DISPLAY_CLEAR   0b0000000000000001  //Löschen der Anzeigedaten. Benötigt 1.52ms
#define LCD_ENTRY_MODE      0b0000000000000110  //Eingabemodus eingestellt. Cursor bewegt sich nach Rechts. Benötigt 38us

#define LCD_DISPLAY_ON      0b0000000000001111  //Schaltet das Display an

#define LCD_WRITE           0b0000000000110000  //"0"
#define LCD_WRITE1          0b0000000000110001  //"0"

void lcd_init(void);

