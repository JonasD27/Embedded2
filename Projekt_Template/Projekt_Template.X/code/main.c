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

double latest_temperatur;
float latest_cpu_load;

char received_UART[20];
int UART_RX_count;


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
       //Asugabe per UART
       char str[32]; 
       sprintf(str,"Temperatur: %.1f Grad",get_Temperatur());
       putsUART(str);
       char lf[2];
       sprintf(lf, "\n"); 
       putsUART(lf);
       
       latest_temperatur=get_Temperatur();
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

void measureProcesstime(){
    static uint16_t time = 0;
    time++;
    if (time >= 10000) //Alle 10s Auslastung berechnen
    {
        float Auslastung;
        uint16_t lsw = TMR2;
        uint16_t msw = TMR3HLD;

        float Auslastung1 = (float)lsw * 10;
        float Auslastung2 = (float)msw *65535*10;
        Auslastung = (Auslastung1 + Auslastung2) / (FCY);
        
        latest_cpu_load=Auslastung;
        
        time = 0;
        TMR3HLD=0;
        TMR3 = 0; // Clear 32-bit Timer (msw)
        TMR2 = 0;
    }
 }


void display_UART_RX()
{
    if (UART_RX_count>0)
    {
       lcd_set_pos(2,1);
       writeStrLCD("                ");
       lcd_set_pos(2,1);
       writeStrLCD(received_UART);
       memset(received_UART,0,sizeof(received_UART));
       UART_RX_count=0;
    }
    
}

void display_temp_load()
{
   static uint16_t time = 0;
   static bool switch_value=1;
   time++;
   
   if (time >= 3000) //Alle 3s Auslastung berechnen
   {
       time=0;
       if (switch_value) //Temperatur
       {
            char str[32]; 
            sprintf(str,"Temperatur: %.1f",latest_temperatur);
            lcd_set_pos(1,1);
            writeStrLCD(str);
            switch_value=0;
       }
       
       else //Auslastung
       {
           char str[32]; 
           sprintf(str,"Auslastung: %.1f",(double)latest_cpu_load);
           lcd_set_pos(1,1);
           writeStrLCD(str);
           switch_value=1;
       }
       
       display_UART_RX();
   }
   
   

}



/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    
    uint16_t Count = 0;

    ConfigureOscillator();
    initUART();
    init_timer1(); //Jede Sekunde I2C Anfrage
    init_ms_t4(); //Heartbeat in Superloop
    init_t2_t3();
    initI2C();
    lcd_init();

    writeStrLCD("Hello World");
    delay_ms(2000);
    lcd_clear();

    _RP66R = _RPOUT_U1TX; //UART Pin Mapping
    RPINR18bits.U1RXR = 0b1011000;

    
    _TRISF0 = 0; //F�r Asuslastungsberechnung
    _T2CKR = 96;//LATF0 auf Gated Timer Input mappen

    _TRISE8 = 1; //Uart RX als Input
    _ANSE8 = 0;

    
    while(1)
    {
        _LATF0 = 0; //Gated Timer stoppen
        if(_T4IF)
        {
            _T4IF=0;
            Count++;
            if (Count >= HEARTBEAT_MS)
            {
                _LATF0 = 1; //Gated Timer starten
                Count = 0;
                doI2C();
                print_sensor_values();
                measureProcesstime();
                display_temp_load();
                
            }         
        }
    }
}
