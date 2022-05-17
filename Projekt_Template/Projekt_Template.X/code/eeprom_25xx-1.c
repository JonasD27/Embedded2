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

    //SPI MISO als Input
    _TRISA14=1;
    //SPI Signale als Output
    _TRISF5=0; //MOSI
    _TRISF3=0; //CS3
    _TRISF2=0; //CS2
    _TRISF8=0; //CS1
    _TRISF4=0; //SCK
  
    __builtin_write_OSCCONL(OSCCON & 0xDF);        // to clear IOLOCK
    
    _RP100R = _RPOUT_SCK1;
    _RP101R = _RPOUT_SDO1;
    
    //RPINR29bits.SCK3R=100; //SCK3 als Input
    //RPINR29bits.SDI3R=30;
    RPINR20bits.SDI1R = 30;
    
    __builtin_write_OSCCONL(OSCCON | 0x40);        // to set IOLOCK
    //STAT
    
    
    /* Der Betrieb des SPIx-Moduls wird im Idle-Modus fortgesetzt.*/
    SPI1STATbits.SPISIDL = 0;
    

    _SPI1IF = 0; // Clear the Interrupt flag
    _SPI1IE = 0; // Disable the interrupt
    
    SPI1STATbits.SPITBF=0;
    SPI1STATbits.SPIRBF=0;
    
    
    //CON1
    
     /* Master-Modus.*/
    SPI1CON1bits.MSTEN = 1;
    
    SPI1STATbits.SPIROV=0;
    
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
    
    SPI1CON1bits.SSEN=0;
    
    /* Der Ruhezustand für die clock ist ein low Pegel; der aktive Zustand ist 
     * ein high Pegel.*/
    SPI1CON1bits.CKP = 0;
    
    
    
    //12.5 MHz
    /* Sekundäre Vorskalierung 8:1*/
    SPI1CON1bits.SPRE = 0b000;
    
    /* Primäre Vorskalierung 1:1*/
    SPI1CON1bits.PPRE = 0b11;   
    
    
    //CON2
    /* Framed SPIx-Unterstützung ist deaktiviert.*/
    SPI1CON2bits.FRMEN = 0;
    
    /* Erweiterter Puffer ist deaktiviert (Legacy-Modus).*/
    SPI1CON2bits.SPIBEN = 0;  
    
    
    /* SPIx-Modul ist aktiviert und konfiguriert die Stifte SCKx, SDOx, SDIx und 
     * SSx als serielle Anschlussstifte.*/
    SPI1STATbits.SPIEN = 1;
    
}/*initSPI()*/


void writeDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    
    EEPROM_NCS = 0;             //Ausgangspin für SPI Chip Select auf 0
    
#if 0
    char gesendetStr[100];
    //Zum testen Werte in stinrgs gespeichert und an uart gesendet
    sprintf(gesendetStr,"EEPROM_CMD_RDIP: %d",EEPROM_CMD_RDIP);
    putsUART(gesendetStr);
    while(!SPI1STATbits.SPIRBF);
    sprintf(gesendetStr,"Nach wh: %d",EEPROM_CMD_RDIP);
    putsUART(gesendetStr);
    sprintf(gesendetStr,"Prescaler1: %d, 2:%d",SPI1CON1bits.PPRE,SPI1CON1bits.SPRE);
    putsUART(gesendetStr);
    
#endif
    
    SPI1BUF=EEPROM_CMD_WRITE;
    SPI1BUF=addr;
    SPI1BUF=*data;
    
    EEPROM_NCS = 1;             //Ausgangspin für SPI Chip Select auf 1
    
}/*writeDataEEPROM()*/



void readDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    uint8_t readDataBuffer = SPI1BUF;
    
    return readDataBuffer;
    
}/*readDataEEPROM()*/


uint8_t  readStatusEEPROM(void)
{
    uint8_t status;
    
    EEPROM_NCS = 0;
    __delay_cycles(2); //2*40ns=80ns
    
    SPI1BUF=EEPROM_CMD_PDSR;
    status = SPI1BUF;
    EEPROM_NCS = 1;
    __delay_cycles(2); //2*40ns=80ns
    
    char debug[100];
    sprintf(debug,"Stauts: %02X",status);
    putsUART(debug);
    
    return status;

}

uint8_t  readSignatureEEPROM(void)
{
    uint8_t signature;
    volatile uint8_t dummy;
    
    EEPROM_NCS = 0;
    __delay_us(1);
    
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    dummy = SPI1BUF;
    dummy = SPI1BUF;
    SPI1BUF=EEPROM_CMD_RDIP;
    
    //Dummy Adresse

    
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    dummy = SPI1BUF;
    dummy = SPI1BUF;
    SPI1BUF=0xF;
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    dummy = SPI1BUF;
    dummy = SPI1BUF;
    SPI1BUF=0xF;
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    dummy = SPI1BUF;
    dummy = SPI1BUF;
    SPI1BUF=0xF;
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    dummy = SPI1BUF;
    dummy = SPI1BUF;
    
    delay_ms(1);
    dummy = SPI1BUF;
    dummy = SPI1BUF;
    
    
    
    
    SPI1BUF=0x0;
    //while(!SPI1STATbits.SPIRBF);
    signature=SPI1BUF;
    __delay_us(6);
    EEPROM_NCS = 1;

     
    
    
    char debug[100];
    sprintf(debug,"Signatur: 0x%02X",signature);
    putsUART(debug);
    char lf[2];
    sprintf(lf, "\n"); 
    putsUART(lf);
    
    
    return signature;

}
