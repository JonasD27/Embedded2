#include "user.h"          /* Benutzer - Funktion/Parameter                   */
#include <stdint.h>        /* Enthält uint16_t-Definition                     */
#include <stdbool.h>       /* Enthält eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enthält Zeichenketten                           */
#include <stdio.h>         /* Enthält Ein - und Ausgabefunktionen             */
#include <stdlib.h>        /* Enthält Hilfsfunktionen                         */

#include <xc.h>            /* Jede Prozessordatei ist geschützt.       */

/*Konstanten Defines*/

#define PAJ76_ID 0x73
#define INIT_REG_ARRAY_SIZE (sizeof(initRegisterArray)/sizeof(initRegisterArray[0]))



/*Prototypen*/

void gesture_write(uint8_t addr, uint8_t data);
void gesture_sel_bank(int i);
void gesture_read(uint8_t addr, uint8_t num, uint8_t data[]);
void gesture_init();


