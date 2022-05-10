/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <stdint.h>        /* Enthält uint16_t-Definition                     */
#include <stdbool.h>       /* Enthält eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enthält Zeichenketten                           */
#include <stdio.h>         /* Enhält Ein - und Ausgabefunktionen              */
#include <stdlib.h>        /* Enthält Hilfsfunktionen                         */

#include <xc.h>            /* Jede Prozessordatei ist geschützt.              */

#include "libpic30.h"      /* Beinhaltet Delay-Funktionen                     */

/******************************************************************************/
/* Konstanten                                                                 */
/******************************************************************************/

/*volatile darf nicht vom Compiler wegoptimiert werden und kann extern 
 *verändert werden z.b. von Interruptroutine oder Timer*/
#define __delay_cycles(cycles)asm volatile ("repeat #%0 \n nop" : : "i" (cycles-2))

#define LCD_DATA(d) LATE = (LATE & 0xFF00) | d
#define LCD_RS _RB15        /*LCD Registerauswahlsignal*/
#define LCD_R_W _RD5        /*LCD Daten Lesen oder Schreiben*/
#define LCD_ENABLE _RD4     /*LCD Aktivierungssignal*/

#define LCD_CMD_INIT        0b0000000000110000 /*LCD wird Initialliert*/

/*Einstellen der Bewegungsrichtung von Cursor und Display.(8-Bit-Busbetrieb, 
 *2-zeilige Anzeigemodus eingestellt) Benötigt 38us*/
#define LCD_FUNCTION_SET    0b0000000000111000  

/*Schaltet das Display aus. Benötigt 38us*/
#define LCD_DISPLAY_OFF     0b0000000000001000 

/*Löschen der Anzeigedaten. Benötigt 1.52ms*/
#define LCD_DISPLAY_CLEAR   0b0000000000000001

/*Cursor und Anzeige werden an ihren ursprünglichen Zustand versetzt.
 *Benötigt 1.52ms*/
#define LCD_DISPLAY_HOME    0b0000000000000010

/*Eingabemodus eingestellt. Cursor bewegt sich nach Rechts. Benötigt 38us*/
#define LCD_ENTRY_MODE      0b0000000000000110  

/*Schaltet Display und Cursor ein und lässt den Cursor blinken. Benötigt 38us*/
/*#define LCD_DISPLAY_ON      0b0000000000001111*/
/*Schaltet Display und Cursor ein und lässt den Cursor nicht mehr blinken. 
 *Benötigt 38us*/
#define LCD_DISPLAY_ON      0b0000000000001110

/*Definiert den IC und gibt an das dieser in Betrieb ist. Interne Operation ist
 *im Gange es soll gewartet werden. Benötigt 0us.*/
#define READ_BUSY_FLAG      0b0000000010000000

/* Cursor nach rechts schieben, AC wird um 1 erhöht und gesamte Anzeige nach
 * rechts verschieben (der Cursor bewegt sich entsprechend der Anzeige).
 * In der Anzeige wird nicht gelesen oder geschrieben. Diese Anweisung wird
 * verwendet, um Anzeigedaten zu korrigieren oder zu suchen. Benötigt 38us. */
#define CURSOR_OR_DISPLAY   0b0000000000010100

/******************************************************************************/
/* Prototypen                                                                 */
/******************************************************************************/

void lcd_init(void);
void lcd_write_data(uint8_t data);
void writeStrLCD(const char* str);
void lcd_clear(void);
void lcd_set_pos(int line, int pos);
void waitForBusyLCD(void); 
uint8_t lcd_get_status(void);