/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"          /* User funct/params, such as InitApp              */

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

                     

/******************************************************************************/
/* Konstanten                                                                 */
/******************************************************************************/

/*volatile darf nicht vom Compiler wegoptimiert werden und kann extern 
 *ver�ndert werden z.b. von Interruptroutine oder Timer*/
#define __delay_cycles(cycles)asm volatile ("repeat #%0 \n nop" : : "i" (cycles-2))

#define LCD_DATA(d) LATE = (LATE & 0xFF00) | d
#define LCD_RS _RB15        /*LCD Registerauswahlsignal*/
#define LCD_R_W _RD5        /*LCD Daten Lesen oder Schreiben*/
#define LCD_ENABLE _RD4     /*LCD Aktivierungssignal*/

#define LCD_CMD_INIT        0b0000000000110000 /*LCD wird Initialliert*/

/*Einstellen der Bewegungsrichtung von Cursor und Display.(8-Bit-Busbetrieb, 
 *2-zeilige Anzeigemodus eingestellt) Ben�tigt 38us*/
#define LCD_FUNCTION_SET    0b0000000000111000  

/*Schaltet das Display aus. Ben�tigt 38us*/
#define LCD_DISPLAY_OFF     0b0000000000001000 

/*L�schen der Anzeigedaten. Ben�tigt 1.52ms*/
#define LCD_DISPLAY_CLEAR   0b0000000000000001

/*Cursor und Anzeige werden an ihren urspr�nglichen Zustand versetzt.
 *Ben�tigt 1.52ms*/
#define LCD_DISPLAY_HOME    0b0000000000000010

/*Eingabemodus eingestellt. Cursor bewegt sich nach Rechts. Ben�tigt 38us*/
#define LCD_ENTRY_MODE      0b0000000000000110  

/*Schaltet Display und Cursor ein und l�sst den Cursor blinken. Ben�tigt 38us*/
//#define LCD_DISPLAY_ON      0b0000000000001111
/*Schaltet Display und Cursor ein und l�sst den Cursor nicht mehr blinken. 
 *Ben�tigt 38us*/
#define LCD_DISPLAY_ON      0b0000000000001110

/*Definiert den IC und gibt an das dieser in Betrieb ist. Interne Operation ist
 *im Gange es soll gewartet werden*/
#define READ_BUSY_FLAG      0b0000000010000000

/*Cursor nach rechts schieben, AC wird um 1 erh�ht und gesamte Anzeige nach
 * rechts verschieben (der Cursor bewegt sich entsprechend der Anzeige).
 * In der Anzeige wird nicht gelesen oder geschrieben. Diese Anweisung wird
 * verwendet, um Anzeigedaten zu korrigieren oder zu suchen. */
#define CURSOR_OR_DISPLAY   0b0000000000010100

/*Zeichen definieren*/
#define LCD_ZEICHEN
//#define LCD_ZEICHEN(zeichen)zeichen

void initPMP(void);



