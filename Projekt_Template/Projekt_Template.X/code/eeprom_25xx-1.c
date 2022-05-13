/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Jede Prozessordatei ist geschützt.              */

#include <stdint.h>        /* Enthält uint16_t-Definition                     */
#include <stdbool.h>       /* Enthält eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enthält Zeichenketten                           */
#include <stdio.h>         /* Enthält Ein - und Ausgabefunktionen             */
#include <stdlib.h>        /* Enthält Hilfsfunktionen                         */

#include "system.h"        /* System - Funktion/Parameter                     */
#include "user.h"          /* Benutzer - Funktion/Parameter                   */

#include "libpic30.h"      /* Beinhaltet Delay-Funktionen                     */
#include "eeprom_25xx-1.h" /* Behinhaltet Konstanten und Prototypen           */


/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/


void initSPI()
{
    //STAT
    SPI1STATbits.SPIEN = 1;
    SPI1STATbits.SPISIDL = 0;
    
    
    //CON1
    SPI1CON1bits.DISSCK = 0;
    SPI1CON1bits.DISSDO = 0;
    SPI1CON1bits.MODE16 = 0;
    SPI1CON1bits.SMP = 1;
    SPI1CON1bits.CKE = 1;
    SPI1CON1bits.CKP = 0;
    SPI1CON1bits.MSTEN = 1;
    
    //12.5 MHz
    SPI1CON1bits.SPRE = 110;        //2:1 eingestellt
    SPI1CON1bits.PPRE = 11;         //1:1 eingestellt
    
    //CON2
    SPI1CON2bits.FRMEN = 0;
    SPI1CON2bits.SPIBEN = 0;   
}


void readDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    
}


void writeDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    
}
