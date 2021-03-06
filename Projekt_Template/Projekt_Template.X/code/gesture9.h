#include "user.h"          /* Benutzer - Funktion/Parameter                   */
#include <stdint.h>        /* Enth?lt uint16_t-Definition                     */
#include <stdbool.h>       /* Enth?lt eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enth?lt Zeichenketten                           */
#include <stdio.h>         /* Enth?lt Ein - und Ausgabefunktionen             */
#include <stdlib.h>        /* Enth?lt Hilfsfunktionen                         */

#include <xc.h>            /* Jede Prozessordatei ist gesch?tzt.       */

/*Konstanten Defines*/

#define PAJ76_ID 0x73
#define INIT_REG_ARRAY_SIZE (sizeof(initRegisterArray)/sizeof(initRegisterArray[0]))



/*Prototypen*/

void gesture_write(uint8_t addr, uint8_t data);
void gesture_sel_bank(int i);
void gesture_read(uint8_t addr, uint8_t num, uint8_t data[]);
void gesture_init();


