#include "user.h"
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

//Konstanten
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1

//Typedefs
typedef struct 
{
  uint8_t data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
}Buffer;

#ifdef MAIN
//Globale Variablen
Buffer FIFO = {{}, 0, 0}; //FIFO zum Versenden über UART
#else
extern Buffer FIFO; 
#endif

//Prototypen
void initUART(void);

int16_t putsUART(const char *str);
int16_t getcFIFO_TX(volatile uint16_t *c);
//int16_t getcFIFO_RX(char *c);

int16_t putcFIFO_TX(char c);
//int16_t putcFIFO_RX(char c);