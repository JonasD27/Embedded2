/*TODO
    Doku mit DoxyGen
 *  
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#define MAIN
#include <xc.h>

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "UART.h"
#include "I2C.h"
#include "lcd_gpio.h"

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */


/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

#define HEARTBEAT_MS 1

i2c_status_t status_temperatur = Error;
i2c_status_t status_licht = Error;

uint8_t write_data_buffer_temp=0b00000000;
uint8_t write_data_buffer_light=0b00010000;
uint8_t read_data_buffer_temp[2];
uint8_t read_data_buffer_light[2];

int test=0;

/*Prototypen*/

void print_sensor_values(void);

double get_Temperatur(void); 
double get_Light(void); 

/*Funktionen*/

/**
 * Ausgabe der ausgelesenen Sensor-Werte per UART
 */
void print_sensor_values()
{
    static int count=0;

    //Temperatur
    if (status_temperatur==Finished)
    {
       char str[32]; 
       sprintf(str,"Temperatur: %.1f Grad",get_Temperatur());
       putsUART(str);
       char lf[2];
       sprintf(lf, "\n"); 
       putsUART(lf);
       status_temperatur=Pending;
    }
    
    if (status_licht==Finished)
    {
       char str[16]; 
       sprintf(str,"Licht: %.1f lux",get_Light());
       putsUART(str);
       char lf[2];
       sprintf(lf, "\n"); 
       putsUART(lf);
       status_licht=Pending;
    }
    if (status_licht==Error || status_temperatur==Error)
    {
        if (count>=1000)
        {
           count=0;
           char str[32]; 
           sprintf(str, "Fehler beim Auslesen!");
           putsUART(str); 
           char lf[2];
           sprintf(lf, "\n"); 
           putsUART(lf);
        } 
        else
        {
           count++;
        }
    }

       
   
 
} /* print_sensor_values() */

double get_Temperatur(void)
{
    double temp = read_data_buffer_temp[0]<<8|read_data_buffer_temp[1];
    temp = temp/256;
    return temp;
}

double get_Light(void)
{
    double light = read_data_buffer_light[0]<<8 | read_data_buffer_light[1];
    light = light/1.2;
    return light;
}

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
    uint16_t Count = 0;

    ConfigureOscillator();
    initUART();
    init_timer1();
    init_ms_t4();
    initI2C();
    lcd_init();
    lcd_write_data('H');
    lcd_write_data('a');
    lcd_write_data('l');
    lcd_write_data('l');
    lcd_write_data('i');

    _RP66R = _RPOUT_U1TX; //UART Pin Mapping
    RPINR18bits.U1RXR = 0b1011000;

    while(1)
    {
        if(_T4IF)
        {
            _T4IF=0;
            Count++;
            if (Count >= HEARTBEAT_MS)
            {
                Count = 0;
                doI2C();
                print_sensor_values();
            }         
        }
    }
}
