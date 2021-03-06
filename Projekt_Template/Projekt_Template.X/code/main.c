/*TODO
    Doku mit DoxyGen
 *  
 */

/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Jede Prozessordatei ist gesch?tzt.              */

#include <stdint.h>        /* Enth?lt uint16_t-Definition                     */
#include <stdbool.h>       /* Enth?lt eine Wahr/Falsch-Definition             */
#include <string.h>        /* Enth?lt Zeichenketten                           */
#include <stdio.h>         /* Enth?lt Ein - und Ausgabefunktionen             */
#include <stdlib.h>        /* Enth?lt Hilfsfunktionen                         */

#include "system.h"        /* System - Funktion/Parameter                     */
#include "user.h"          /* Benutzer - Funktion/Parameter                   */

/* Beinhaltet Konstanten, Typdefs, globale Variablen und Prototypen f?r main  */
#include "UART.h"          
#include "I2C.h"           
//#include "lcd_gpio.h"
#include "PMP.h"
#include "eeprom_25xx-1.h" /* Behinhaltet Konstanten und Prototypen           */

/******************************************************************************/
/* Konstanten                                                                 */
/******************************************************************************/

#define MAIN
#define HEARTBEAT_MS 1

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

i2c_status_t status_temperatur = Error;
i2c_status_t status_licht = Error;

uint8_t write_data_buffer_temp = 0b00000000;
uint8_t write_data_buffer_light = 0b00010000;
uint8_t read_data_buffer_temp[2];
uint8_t read_data_buffer_light[2];

double latest_temperatur;
float latest_cpu_load;

char received_UART[20];
int UART_RX_count;

/******************************************************************************/
/* Prototypen                                                                 */
/******************************************************************************/

void print_sensor_values(void);

double get_Temperatur(void); 
double get_Light(void); 

/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

#if 0
/**
 * Ausgabe der ausgelesenen Sensor-Werte per UART
 */
void print_sensor_values()
{
    static int count = 0;

    //Temperatur
    if (status_temperatur == Finished)
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
    
    //Licht
    if (status_licht == Finished)
    {
       char str[16]; 
       sprintf(str,"Licht: %.1f lux",get_Light());
       putsUART(str);
       char lf[2];
       sprintf(lf, "\n"); 
       putsUART(lf);
       status_licht=Pending;
    }
    
    if (status_licht == Error || status_temperatur == Error)
    {
        if (count >= 1000)
        {
           count = 0;
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
    double temp = read_data_buffer_temp[0] << 8 | read_data_buffer_temp[1];
    temp = temp / 256;
    return temp;
    
}/*get_Temperatur()*/
double get_Light(void)
{
    double light = read_data_buffer_light[0] << 8 | read_data_buffer_light[1];
    light = light / 1.2;
    return light;
    
}/*get_Light()*/
#endif


void measureProcesstime()
{
    static uint16_t time = 0;
    time++;
    
    if (time >= 10000) //Alle 10s Auslastung berechnen
    {
        float Auslastung;
        uint16_t lsw = TMR2;
        uint16_t msw = TMR3HLD;

        float Auslastung1 = (float)lsw * 10;
        float Auslastung2 = (float)msw * 65535 * 10;
        Auslastung = (Auslastung1 + Auslastung2) / (FCY);
        
        latest_cpu_load=Auslastung;
        
        time = 0;
        TMR3HLD = 0;
        TMR3 = 0; // Clear 32-bit Timer (msw)
        TMR2 = 0;
    }
    
 }/*measureProcesstime()*/


void display_UART_RX()
{
    if (UART_RX_count > 0)
    {
       lcd_set_pos(2,1);
       writeStrLCD("                ");
       lcd_set_pos(2,1);
       writeStrLCD(received_UART);
       memset(received_UART,0,sizeof(received_UART));
       UART_RX_count = 0;
    }
    
}/*display_UART_RX()*/

void send_lcd_status()
{
       char str[32]; 
       sprintf(str,"Status: %02X",lcd_get_status());
       putsUART(str);
       char lf[2];
       sprintf(lf, "\n"); 
       putsUART(lf);
       
}/*send_lcd_status()*/


void display_temp_load()
{
   static uint16_t time = 0;
   static bool switch_value = 1;
   time++;
   
   if (time >= 2000) //Alle 2s Refresh
   {
       time = 0;

       if (switch_value) //Temperatur
       {
            char str[32]; 
            sprintf(str,"Temperatur: %.1f",latest_temperatur);
            lcd_set_pos(1,1);
            writeStrLCD(str);
            switch_value = 0;
       }
       
       else //Auslastung
       {
           char str[32]; 
           sprintf(str,"Auslastung: %.2f",(double)latest_cpu_load);
           lcd_set_pos(1,1);
           writeStrLCD(str);
           switch_value = 1;
       }
       
       display_UART_RX();
       
#if 0
       uint8_t data1[3] = {128, 0, 30};
       uint8_t data2[3] = {254, 13, 76};
       uint8_t data3[3] = {255, 255, 255};
       
       writeDataEEPROM(0x00000000,data1,3); //Pro Adresse ein Byte m?glich
       writeDataEEPROM(0x00000003,data2,3);
       writeDataEEPROM(0x00000006,data3,3);
       
       delay_ms(10);
#endif    
       uint8_t recv_data1[3];
       uint8_t recv_data2[3];
       uint8_t recv_data3[3];
       
       readDataEEPROM(0x00000000, recv_data1, 3);
       readDataEEPROM(0x00000003, recv_data2, 3);
       readDataEEPROM(0x00000006, recv_data3, 3);
       char str[64]; 
       sprintf(str, "%i %i %i",recv_data1[0], recv_data2[1], recv_data3[2]);
       lcd_set_pos(2,1);
       writeStrLCD(str);
   }
   
}/*display_temp_load()*/


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    uint16_t Count = 0;

    ConfigureOscillator();
    initUART();
    init_timer1();              //Jede Sekunde I2C Anfrage
    init_ms_t4();               //Heartbeat in Superloop
    init_t2_t3();
    initI2C();
    //lcd_init();
    initPMP();
    initSPI();
    
    writeStrLCD("Hello World");
    delay_ms(2000);
    lcd_clear();
    
    char str[32];
    sprintf(str,"Signatur: 0x%02X",readSignatureEEPROM());
    putsUART(str);
    char lf[2];
    sprintf(lf, "\n"); 
    putsUART(lf);
    

    _TRISF0 = 0;                //F?r Asuslastungsberechnung
    _T2CKR = 96;                //LATF0 auf Gated Timer Input mappen

    
    while(1)
    {
        _LATF0 = 0;             //Gated Timer stoppen
        
        if(_T4IF)
        {
            _T4IF = 0;
            Count++;
            
            if (Count >= HEARTBEAT_MS)
            {
                _LATF0 = 1;     //Gated Timer starten
                Count = 0;
                
                doI2C();
                measureProcesstime();
                display_temp_load(); 
            }         
        }
    }
    
}/*main()*/
