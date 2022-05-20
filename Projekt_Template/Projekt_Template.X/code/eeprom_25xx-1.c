/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Jede Prozessordatei ist gesch�tzt.              */

#include <stdint.h>        /* Enth�lt uint16_t-Definition                     */
#include <stdbool.h>       /* Enth�lt eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enth�lt Zeichenketten                           */
#include <stdio.h>         /* Enth�lt Ein - und Ausgabefunktionen             */
#include <stdlib.h>        /* Enth�lt Hilfsfunktionen                         */

#include "system.h"        /* System - Funktion/Parameter                     */
#include "user.h"          /* Benutzer - Funktion/Parameter                   */

#include "libpic30.h"      /* Beinhaltet Delay-Funktionen                     */
#include "eeprom_25xx-1.h" /* Beinhaltet Konstanten und Prototypen            */
#include "UART.h"          /* Beinhaltet Konstanten und Prototypen            */

/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

void initSPI()
{
    /***********************SPI MISO als Input*********************************/
    _TRISA14=1;
    
    /*********************SPI Signale als Output*******************************/
    _TRISF5=0; //MOSI
    _TRISF3=0; //CS3
    _TRISF2=0; //CS2
    _TRISF8=0; //CS1
    _TRISF4=0; //SCK
  
    /**************************************************************************/
    
    __builtin_write_OSCCONL(OSCCON & 0xDF);         // l�sche IOLOCK
    
    /*****************************Input Pins***********************************/
    RPINR20bits.SCK1R = 100;                        // Takteingang an RP100
    RPINR20bits.SDI1R = 30;                         // SDI an RP30
    
    /****************************Output Pins***********************************/
    RPOR9bits.RP101R = _RPOUT_SDO1;                 // SPO
    RPOR9bits.RP100R = _RPOUT_SCK1;                 // SPCLK

    __builtin_write_OSCCONL(OSCCON | 0x40);         // setze IOLOCK
    
    /*******************Interrupt Controller Settings**************************/
    
    /* L�schen des Interrupt-Flags */
    IFS0bits.SPI1EIF = 0;

    /* Interrupt wird deaktiviert */
    IEC0bits.SPI1IE = 0;

    /*************SPIxSTAT: SPIx Status and Control Register*******************/
    
    /* Der Betrieb des SPIx-Moduls wird im Idle-Modus fortgesetzt.*/
    SPI1STATbits.SPISIDL = 0;
    
    /* Es ist kein �berlauf aufgetreten */
    SPI1STATbits.SPIROV=0;
       
    /* �bertragung hat begonnen, SPIxTXB-Puffer ist leer */
    SPI1STATbits.SPITBF=0;
    
    /* Empfang ist unvollst�ndig, SPIxRXB ist leer */
    SPI1STATbits.SPIRBF=0;
    
    /*****************SPI XCON1: SPIx Control Register 1***********************/
    
    /* SPIx-Takt am SCKx-Pin ist aktiviert.*/
    SPI1CON1bits.DISSCK = 0;
    
    /* SDOx-Pin wird vom Modul gesteuert.*/
    SPI1CON1bits.DISSDO = 0;
    
    /* Die Kommunikation erfolgt byteweise (8 Bit).*/
    SPI1CON1bits.MODE16 = 0;
    
    /* Eingangsdaten werden am Ende der Datenausgabezeit abgetastet.*/
    SPI1CON1bits.SMP = 1;
    
    /* �nderung der seriellen Ausgangsdaten beim �bergang vom aktiven 
     * Taktzustand zum Idle-Taktzustand.*/
    SPI1CON1bits.CKE = 1;
    
    SPI1CON1bits.SSEN=0;
    
    /* Der Ruhezustand f�r die clock ist ein low Pegel; der aktive Zustand ist 
     * ein high Pegel.*/
    SPI1CON1bits.CKP = 0;
    
    /* Master-Modus.*/
    SPI1CON1bits.MSTEN = 1;
    
    /***********************12,5 MHz eingestellt*******************************/
    
    /* Taktfrequenz = (Systemfrequenz / (Prim�re * Sekund�re Vorskalierung)) */
    
    /* Prim�re Vorskalierung 16:1*/     // 50 Mhz / 16 = 3,125 MHz
    SPI1CON1bits.PPRE = 0b01;   
    
    /* Sekund�re Vorskalierung 8:1*/    // 3,125 MHz / 1 = 3,125 MHz
    SPI1CON1bits.SPRE = 0b111;
     
    /*****************SPI XCON1: SPIx Control Register 2***********************/
    
    /* Framed SPIx-Unterst�tzung ist deaktiviert.*/
    SPI1CON2bits.FRMEN = 0;
    
    /* Erweiterter Puffer ist deaktiviert (Legacy-Modus).*/
    SPI1CON2bits.SPIBEN = 0;  
    
    /* SPIx-Modul ist aktiviert und konfiguriert die Stifte SCKx, SDOx, SDIx und 
     * SSx als serielle Anschlussstifte.*/
    SPI1STATbits.SPIEN = 1;
    
}/*initSPI()*/


void writeDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    setWriteEnableEEPROM();
    
    while(busyEEPROM());
    
    volatile uint8_t dummy;
    
    SPI1STATbits.SPIROV=0; //Overflow Flag clearen
    dummy = SPI1BUF;
    
    EEPROM_NCS = 0;
    //Write instruction
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=EEPROM_CMD_WRITE;
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    
    //Adressse senden
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    // 2. Wort
    SPI1BUF=(addr >> 16 );
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    
    // 3. Wort
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=(addr >> 8 );
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    
    // 4. Wort
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=(addr >> 0 );
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    
    //Daten schreiben
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=*data;
    //Warten bis Recieve Buffer voll ist
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    
    EEPROM_NCS = 1;
    
}/*writeDataEEPROM()*/



void readDataEEPROM(uint32_t addr, uint8_t *data, int count)
{
    uint8_t readDataBuffer = SPI1BUF;
    
    return readDataBuffer;
    
}/*readDataEEPROM()*/


uint8_t  readStatusEEPROM(void)
{
    uint8_t status;
    volatile uint8_t dummy;
    
    SPI1STATbits.SPIROV=0; //Overflow Flag clearen
    dummy = SPI1BUF;
    
    EEPROM_NCS = 0;
    
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=EEPROM_CMD_PDSR;
    while(!SPI1STATbits.SPIRBF); //Solange warten, bis Eingelese ist
    dummy = SPI1BUF; //leeren
    
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=0x0;
    //Warten bis Recieve Buffer voll ist
    while(!SPI1STATbits.SPIRBF);
    status = SPI1BUF;
    
    EEPROM_NCS = 1;
    
    return status;

}

uint8_t  readSignatureEEPROM(void)
{
    uint8_t signature;
    volatile uint8_t dummy;
    
    SPI1STATbits.SPIROV=0; //Overflow Flag clearen
    //Dummy read um Buffer zu leeren
    dummy = SPI1BUF;
    
    EEPROM_NCS = 0;
    
    //Read Signatur Befehl senden
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=EEPROM_CMD_RDIP;
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    
    //Dummy Adresse senden
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=0xF;
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=0xF;
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=0xF;
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist

    
    SPI1BUF=0x0;
    //Warten bis Recieve Buffer voll ist
    while(!SPI1STATbits.SPIRBF);
    signature=SPI1BUF;
    EEPROM_NCS = 1;
    
     return signature;

}

uint8_t  busyEEPROM(void)
{
    if (readStatusEEPROM() & EEPROM_STATUS_WIP) //�berpr�fen ob WIP gesetzt ist
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


void setWriteEnableEEPROM(void)
{
    while(busyEEPROM());
    
    volatile uint8_t dummy;
    
    SPI1STATbits.SPIROV=0; //Overflow Flag clearen
    dummy = SPI1BUF;
    
    EEPROM_NCS = 0;
    
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=EEPROM_CMD_WREN;
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
      
    EEPROM_NCS = 1;

}

void setWriteDisableEEPROM(void)
{
    while(busyEEPROM());
    
    volatile uint8_t dummy;
    
    SPI1STATbits.SPIROV=0; //Overflow Flag clearen
    dummy = SPI1BUF;
    
    EEPROM_NCS = 0;
    
    while(SPI1STATbits.SPITBF); //Solange gestzt, bis Transmit Buffer leer ist
    SPI1BUF=EEPROM_CMD_WRDI;
    while(!SPI1STATbits.SPIRBF);
    dummy = SPI1BUF;
      
    EEPROM_NCS = 1;

}