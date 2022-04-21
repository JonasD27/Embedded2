/**
 In dieser Header-Datei werden alle für das I2C-Protokoll benötigten Konstanten,
 * Typedefs und Prototypen definiert
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "user.h"
//#include "UART.h"
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

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
    i2c_status_t status;
}I2C_struct;


typedef struct 
{
  I2C_struct data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
}Buffer_I2C_FSM;

typedef void *(*StateFunc)();

#ifdef MAIN

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

uint8_t write_data_buffer_temp;
uint8_t write_data_buffer_light;
uint8_t read_data_buffer_temp[2];
uint8_t read_data_buffer_light[2];

bool trigger_FSM;


I2C_struct I2C_test_struct = {0,0,NULL,0,NULL,Finished};

Buffer_I2C_FSM FIFO_I2C = {{},0,0}; //FIFO für die I2C FSM
#else
extern uint8_t write_data_buffer_temp;
extern uint8_t write_data_buffer_light;
extern uint8_t read_data_buffer_temp[2];
extern uint8_t read_data_buffer_light[2];

extern bool trigger_FSM;


extern I2C_struct I2C_test_struct;

extern Buffer_I2C_FSM FIFO_I2C; //FIFO für die I2C FSM
#endif




/******************************************************************************/
/* Prototypen                                                                 */
/******************************************************************************/

int16_t exchangeI2C(uint8_t address, uint16_t num_write, uint8_t *writebuf, uint16_t num_read, uint8_t *readbuf, i2c_status_t *status);

void doI2C(void);

void initI2C(void);

void print_sensor_values(void);

void *FSM_Idle(void);
void *FSM_Start(void);
void *FSM_Adresse_Read(void);
void *FSM_Adresse_Write(void);
void *FSM_Repeated_Start(void);
void *FSM_RECV_EN(void);

void *FSM_Stop(void);


