/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Jede Prozessordatei ist gesch?tzt.              */

#include <stdint.h>        /* Enth?lt uint16_t-Definition                     */
#include <stdbool.h>       /* Enth?lt eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enth?lt Zeichenketten                           */
#include <stdio.h>         /* Enh?lt Ein - und Ausgabefunktionen              */
#include <stdlib.h>        /* Enth?lt Hilfsfunktionen                         */

#include "user.h"          /* Benutzer - Funktion/Parameter                   */

/******************************************************************************/
/* Konstanten                                                                 */
/******************************************************************************/

#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1

/******************************************************************************/
/* Typedef                                                                    */
/******************************************************************************/

typedef struct 
{
  uint8_t data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ?ltesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
}Buffer;

/******************************************************************************/
/* Globale Variable Declaration                                               */
/******************************************************************************/

extern Buffer FIFO; 
extern Buffer FIFO_RX; 
extern char received_UART[20];
extern int UART_RX_count;

/******************************************************************************/
/* Prototypen                                                                 */
/******************************************************************************/

void initUART(void);
int16_t putsUART(const char *str);
int16_t getcFIFO_TX(volatile uint16_t *c);
int16_t getcFIFO_RX(char *c);
int16_t putcFIFO_TX(char c);
int16_t putcFIFO_RX(char c);