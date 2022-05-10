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
/**
 * Interrupt des Timer1, welcher jede Sekunde ausgef�hrt wird. Damit wird jede Sekunde eine I2C-Anfrage f�r den Licht- und den Temperatursensor gestellt.
 * Als Erweiterung wird nun ein Zeiger auf die auszuf�hrende Callback-Funktion sowie eine ID bei den Anfragen �bergeben.
 */
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    _T1IF = 0; //Clear Timer1 interrupt flag
    static int count=0;
    
    if (count>=SENSOR_TIME-1)
    {
        count=0;     
        //Anfrage Temperatur-Sensor
        exchangeI2C(0b1001000, 1, &write_data_buffer_temp, 2, read_data_buffer_temp, &status_temperatur, &I2C_TempSens_Callback ,0);
        //Anfrage Licht-Sensor
        exchangeI2C(0b0100011, 1, &write_data_buffer_light, 2, read_data_buffer_light, &status_licht, &I2C_LightSens_Callback, 1);   
    }
    else
    {
        count++;
    }

}
