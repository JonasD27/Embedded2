#include "user.h"
//#include "UART.h"
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#define LCD_RS _RB15
#define LCD_R_W _RD5
#define LCD_ENABLE _RD4

void lcd_init(void);

