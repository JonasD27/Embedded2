/*******************************************************************************
 * @file eeprom_25xx.h  
 * @brief  Zugriffsfunktionen auf ein SPI EEPROM  
 * 
 * Implementierung für das 25LC1024, jedoch werden auch größere und kleinere 
 * EEPROMs mit dem gleichen Zugriffsprotokol unterstützt 
 * 
 * 
 * @author Josef Hahn-Dambacher <josef.hahn-dambacher@htw-aalen.de> JHD
 * @date   2021-03-15
 * 
 * @version 0.1
 * (JHD  13.05.2022)
 * Header Datei mit Prototypen und Makrodefines angelegt
 * 
 * @noop bug
 * 
 * @noop attention
 * 
 * @noop warning
 * 
 * @noop todo
 * 
 ******************************************************************************/


#ifndef EEPROM_25XX_H
#define	EEPROM_25XX_H


/// Output Pin for SPI Chip Select 
#define EEPROM_NCS _LATF8

    
/** @name EEPROM Commands
 *  @anchor makrodefines_eeprom_commands
 * 
 * Kommandos für den SPI Zugriff auf das EEPROM
 * 
 @{ */   
/// Read data from memory array beginning at selected address
#define EEPROM_CMD_READ  0b00000011u
/// Write data to memory array beginning at selected address
#define EEPROM_CMD_WRITE 0b00000010u
/// Set the write enable latch (enable write operations)
#define EEPROM_CMD_WREN  0b00000110u
/// Reset the write enable latch (disable write operations)
#define EEPROM_CMD_WRDI  0b00000100u
/// Read STATUS register
#define EEPROM_CMD_PDSR  0b00000101u
/// Write STATUS register
#define EEPROM_CMD_WRSR  0b00000001u
/// Page Erase, erase one page in memory array
#define EEPROM_CMD_PE    0b01000010u
/// Sector Erase, erase one sector in memory array
#define EEPROM_CMD_SE    0b11011000u
/// Chip Erase, erase all sectors in memory array
#define EEPROM_CMD_CE    0b11000111u
/// Release from Deep power-down and read electronic signature
#define EEPROM_CMD_RDIP  0b10101011u
/// Deep Power-Down mode
#define EEPROM_CMD_DPD   0b10111001u
/** @} */    

    
/** @name EEPROM Status
 *  @anchor makrodefines_eeprom_status
 *
 * Makrodefines for read/write of the Status Register
 * @{ */   
/// Status - Write In Progress
#define EEPROM_STATUS_WIP      0b00000001u
/// Status - Write Enable Latch
#define EEPROM_STATUS_WEL      0b00000010u
/// Status - Block Protection MASK - Sector 0, 1, 2 und 3 
#define EEPROM_STATUS_BP_MASK  0b00001100u
/// Status - Block Protection - None 
#define EEPROM_STATUS_BP_NONE  0b00000000u
/// Status - Block Protection - Section 3 
#define EEPROM_STATUS_BP_S3    0b00000100u
/// Status - Block Protection - Section 2 and 3 
#define EEPROM_STATUS_BP_S23   0b00001000u
/// Status - Block Protection - Full - Section 0, 1, 2 and 3 
#define EEPROM_STATUS_BP_S0123 0b00001100u
/// Status - WP Enable Bit 
#define EEPROM_STATUS_WPEN     0b10000000u
/** @} */    
    
/// Page Größe des verwendeten EEPROMs    
#define EEPROM_PAGE_SIZE       256u
/// Page Größe des verwendeten EEPROMs als UND-Maske definiert
#define EEPROM_PAGE_MASK       0xFFFFFF00u    
    


//OPTIONAL////////////////////////////////////////////////////////////////////////
// ev. in spi.h / spi.c gekapselt da universell	verwendbar
#define SPI_MODE 0
#define SPI_FREQUENCY 0

//* Initialisierung der SPI Schnittstelle */	
void     initSPI();
//* Führt einen Datentransfer von einem Byte auf der SPI Schnittstelle durch */
//void     uint8_t exchangeDataSPI(uint8_t data_out);	
//* Führt einen Datentransfer für N Daten auf der SPI Schnittstelle durch */
//void     void exchangeDataBufSPI(uint16_t N, uint8_t *data_out, uint8_t *data_in);	
////////////////////////////////////////////////////////////////////////////



// Mögliche EEPROM Zugriffsfunktionen	
void     readDataEEPROM(uint32_t addr, uint8_t *data, int count);
void     writeDataEEPROM(uint32_t addr, uint8_t *data, int count);
uint16_t writeBigDataEEPROM(uint32_t *addr, uint8_t *data, uint16_t *count);
void     setWriteEnableEEPROM(void);
void     setWriteDisableEEPROM(void);
uint8_t  readStatusEEPROM(void);
void     writeStatusEEPPROM(uint8_t status);
void     erasePageEEPROM(uint32_t addr);
void     eraseSectorEEPROM(uint32_t addr);
void     eraseChipEEPROM(void);
void     gotoDeepPowerDownEEPROM(void);
void     relaseDeepPowerDownEEPROM(void);
uint8_t  readSignatureEEPROM(void);
uint8_t  busyEEPROM(void);


#endif	/* EEPROM_25XX_H */

// end of file 