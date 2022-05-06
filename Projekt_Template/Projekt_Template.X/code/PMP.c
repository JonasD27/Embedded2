/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Device header file                              */

#include <stdio.h>        
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdlib.h>
#include <string.h>

#include "PMP.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

#include "libpic30.h"       /*Beinhaltet Delay-Funktionen


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

void initPMP(void)
{
    //CON
    PMCONbits.PMPEN = 1;
    PMCONbits.PSIDL = 0;
    PMCONbits.ADRMUX = 0b00;
    PMCONbits.PTBEEN = 0;
    PMCONbits.PTWREN = 1;
    PMCONbits.PTRDEN = 1;
    PMCONbits.CSF = 0b00; 
    PMCONbits.ALP = 1;
    PMCONbits.CS2P = 0;
    PMCONbits.CS1P = 0;
    PMCONbits.BEP = 1;
    PMCONbits.WRSP = 1;
    PMCONbits.RDSP = 1;

    //MODE
    PMMODEbits.IRQM   = 0b00;
    PMMODEbits.INCM   = 0b00;
    PMMODEbits.MODE16 = 0;
    PMMODEbits.MODE   = 0b11; //Master Mode 1
    PMMODEbits.WAITB  = 0b11;
    PMMODEbits.WAITM  = 0b1111;
    PMMODEbits.WAITE  = 0b11;

    //ADRESS ENABLE
    PMAEN = 0x0001;             // PMA0 enabled
    delay_ms(40);       //40 ms warten
    
    /**************************************************************************/
    
    PMADDR = 0; //RS auf 0
    PMDIN1 = LCD_CMD_INIT; //Function Set 1
    delay_ms(5);
    PMDIN1 = LCD_CMD_INIT; //Function Set 2
    __delay_cycles(6000); //
    PMDIN1 = LCD_CMD_INIT; //Function Set 3
    __delay_us(38);
    
    PMDIN1 = LCD_FUNCTION_SET; 
    __delay_us(38);
    PMDIN1 = LCD_DISPLAY_OFF; 
    __delay_us(38);
    PMDIN1 = LCD_DISPLAY_CLEAR; 
    __delay_us(1520);
    PMDIN1 = LCD_ENTRY_MODE;
    __delay_us(38);
    PMDIN1 =LCD_DISPLAY_ON;
    __delay_us(38);
    
    PMADDR = 1; //RS auf 1
    PMDIN1 = 'T';
    
    
    
    
    
    
}/*initPMP()*/
