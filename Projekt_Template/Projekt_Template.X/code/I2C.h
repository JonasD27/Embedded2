/**
 In dieser Header-Datei werden alle für das I2C-Protokoll benötigten Konstanten,
 * Typedefs und Prototypen definiert
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"          /* Benutzer - Funktion/Parameter                   */
//#include "UART.h"
#include <stdint.h>        /* Enthält uint16_t-Definition                     */
#include <stdbool.h>       /* Enthält eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enthält Zeichenketten                           */
#include <stdio.h>         /* Enthält Ein - und Ausgabefunktionen             */
#include <stdlib.h>        /* Enthält Hilfsfunktionen                         */

#include <xc.h>            /* Jede Prozessordatei ist geschützt.              */

/******************************************************************************/
/* Konstanten                                                                 */
/******************************************************************************/

#define I2C_SCL      _RA2
#define I2C_SDA      _RA3
#define I2C_SCL_TRIS _TRISA2
#define I2C_SDA_TRIS _TRISA3 

/******************************************************************************/
/* Typedef                                                                    */
/******************************************************************************/

typedef enum {Pending, Finished, Error} i2c_status_t;
typedef void (*I2C_Callback_t)(uint8_t *readbuf, uint16_t num_read, i2c_status_t *status, int16_t ID);

/**
 * Datenstruktur für die Kapselung aller benötigten Variablen, welche für
 * ein character basiertes FIFO benötigt werden.
 * 
 * Diese Datenstruktur muss als globale Variable (z.B. mit Zusatz static)
 * angelegt werden
 */
typedef struct
{
    uint8_t address;
    uint16_t num_write;
    uint8_t *writebuf;
    uint16_t num_read;
    uint8_t *readbuf;
    i2c_status_t *status;
    I2C_Callback_t callback;
    int16_t ID;
}I2C_struct;


typedef struct 
{
  I2C_struct data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
}Buffer_I2C_FSM;

typedef void *(*StateFunc)();


/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

//In Header nur Prototypen, genauso auch mit den Variablen. Varbiale sollte in c Datei deklariert sein. In Header nur mit extern.
extern uint8_t write_data_buffer_temp;
extern uint8_t write_data_buffer_light;
extern uint8_t read_data_buffer_temp[2];
extern uint8_t read_data_buffer_light[2];

extern i2c_status_t status_temperatur;
extern i2c_status_t status_licht;

extern double latest_temperatur;

extern I2C_struct I2C_test_struct;

extern Buffer_I2C_FSM FIFO_I2C; //FIFO für die I2C FSM

/******************************************************************************/
/* Prototypen                                                                 */
/******************************************************************************/

int16_t exchangeI2C(uint8_t address, uint16_t num_write, uint8_t *writebuf, uint16_t num_read, uint8_t *readbuf, i2c_status_t *status, I2C_Callback_t callback, int16_t ID);

void I2C_TempSens_Callback(uint8_t *readbuf, uint16_t num_read, i2c_status_t *status, int16_t ID);
void I2C_LightSens_Callback(uint8_t *readbuf, uint16_t num_read, i2c_status_t *status, int16_t ID);

void doI2C(void);

void initI2C(void);

void *FSM_Idle(void);
void *FSM_Start(void);
void *FSM_Adresse_Read(void);
void *FSM_Adresse_Write(void);
void *FSM_Repeated_Start(void);
void *FSM_RECV_EN(void);
void *FSM_Stop(void);