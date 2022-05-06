/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* Device header file                              */

#include <stdio.h>        
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <stdlib.h>
#include <string.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */


/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/

void initPMP(void)
{
    _PMPEN = _PMPEN | 1;
    
}/*initPMP()*/
