/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition   */
#include <stdbool.h>       /* Includes true/false definition */
#include "user.h"
#include "I2C.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    _T1IF = 0; //Clear Timer1 interrupt flag
    static int count=0;
    
    if (count>=SENSOR_TIME-1)
    {
        count=0;     
        //Anfrage Temperatur-Sensor
        exchangeI2C(0b1001000, 1, &write_data_buffer_temp, 2, read_data_buffer_temp, &status_temperatur);
        //Anfrage Licht-Sensor
        exchangeI2C(0b0100011, 1, &write_data_buffer_light, 2, read_data_buffer_light, &status_licht);   
    }
    else
    {
        count++;
    }

}


/* TODO Add interrupt routine code here. */
