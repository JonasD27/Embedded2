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
#include "UART.h"          /* Behinhaltet Konstanten und Prototypen           */

/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

void initSPI()
{
    //STAT
    /* SPIx-Modul ist aktiviert und konfiguriert die Stifte SCKx, SDOx, SDIx und 
     * SSx als serielle Anschlussstifte.*/
    SPI1STATbits.SPIEN = 1;
    
    /* Der Betrieb des SPIx-Moduls wird im Idle-Modus fortgesetzt.*/
    SPI1STATbits.SPISIDL = 0;
    
    
    //CON1
    /* SPIx-Takt am SCKx-Pin ist aktiviert.*/
    SPI1CON1bits.DISSCK = 0;
    
    /* SDOx-Pin wird vom Modul gesteuert.*/
    SPI1CON1bits.DISSDO = 0;
    
    /* Die Kommunikation erfolgt byteweise (8 Bit).*/
    SPI1CON1bits.MODE16 = 0;
    
    /* Eingangsdaten werden am Ende der Datenausgabezeit abgetastet.*/
    SPI1CON1bits.SMP = 1;
    
    /* Änderung der seriellen Ausgangsdaten beim Übergang vom aktiven 
     * Taktzustand zum Idle-Taktzustand.*/
    SPI1CON1bits.CKE = 1;
    
    /* Der Ruhezustand für die clock ist ein low Pegel; der aktive Zustand ist 
     * ein high Pegel.*/
    SPI1CON1bits.CKP = 0;
    
    /* Master-Modus.*/
    SPI1CON1bits.MSTEN = 1;
    
    
    //12.5 MHz
    /* Sekundäre Vorskalierung 2:1*/
    SPI1CON1bits.SPRE = 0b110;
    
    /* Primäre Vorskalierung 1:1*/
    SPI1CON1bits.PPRE = 0b11;   
    
    
    //CON2
    /* Framed SPIx-Unterstützung ist deaktiviert.*/
    SPI1CON2bits.FRMEN = 0;
    
    /* Erweiterter Puffer ist deaktiviert (Legacy-Modus).*/
    SPI1CON2bits.SPIBEN = 0;  
    
}/*initSPI()*/


void writeDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    char gesendetStr[100];
    while(SPI1STATbits.SPITBF);
    EEPROM_NCS = 0;             //Ausgangspin für SPI Chip Select
    
    //Zum testen Werte in stinrgs gespeichert und an uart gesendet
    sprintf(gesendetStr,"EEPROM_CMD_RDIP: %d",EEPROM_CMD_RDIP);
    putsUART(gesendetStr);
    while(!SPI1STATbits.SPIRBF);
    sprintf(gesendetStr,"Nach wh: %d",EEPROM_CMD_RDIP);
    putsUART(gesendetStr);
    sprintf(gesendetStr,"Prescaler1: %d, 2:%d",SPI1CON1bits.PPRE,SPI1CON1bits.SPRE);
    putsUART(gesendetStr);
    
    EEPROM_NCS = 1;             //Ausgangspin für SPI Chip Select
    
}/*writeDataEEPROM()*/



void readDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    uint8_t readDataBuffer = SPI1BUF;
    
    return readDataBuffer;
    
}/*readDataEEPROM()*/

