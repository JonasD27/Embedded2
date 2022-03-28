/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

/* Microcontroller MIPs (FCY) */
//#define SYS_FREQ        7370000L
//#define SYS_FREQ        8000000L
#define SYS_FREQ        50000000L
//#define SYS_FREQ        70000000L
//#define SYS_FREQ        100000000L
//#define SYS_FREQ        140000000L

#define FCY             SYS_FREQ/2


/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */


//System Prototypen
void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

