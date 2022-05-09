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




extern uint32_t DELAY_ANPASSUNG;



/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */


//System Prototypen
void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
void delay_ms(uint16_t milliseconds); 

void init_timer1(void);
void init_ms_t4(void);

void init_t2_t3(void);

