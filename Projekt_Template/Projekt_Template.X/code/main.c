/*TODO
    Bibliothekenkonzept
    Doku mit DoxyGen
    Lichtsensor testen
    FSM in Interrupt
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

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */


/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
#define HEARTBEAT_MS 1
#define SENSOR_TIME 5

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    _T1IF = 0; //Clear Timer1 interrupt flag
    static int count=0;
    
    if (count>=SENSOR_TIME-1)
    {
        count=0;
        i2c_status_t status;   
        //putsUART("Hello World\n"); 
        //Anfrage Temperatur-Sensor
        exchangeI2C(0b1001000, 1, &write_data_buffer_temp, 2, read_data_buffer_temp, &status);
        //Anfrage Licht-Sensor
        exchangeI2C(0b0100011, 1, &write_data_buffer_light, 2, read_data_buffer_light, &status);
        print_sensor_values();
    }
    else
    {
        count++;
    }
    
    
}

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
    uint16_t Count = 0;
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
    //InitApp();
    initUART();
    init_timer1();
    init_ms_t4();
    initI2C();

    _RP66R = _RPOUT_U1TX; //UART Pin Mapping
    RPINR18bits.U1RXR = 0b1011000;
    
    
    write_data_buffer_temp=0b00000000;
    write_data_buffer_light=0b00010000;
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
            }         
        }
    }
}
