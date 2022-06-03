/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define LED0 _LATB8
#define LED1 _LATB9
#define LED2 _LATB10
#define LED3 _LATB11
#define T0 !_RG12
#define T1 !_RG13
#define T2 !_RG14
#define T3 !_RG15
#define INC_T4 !_RG9
#define TLED0() asm("BTG LATB,#8")
#define TLED1() asm("BTG LATB,#9")
#define TLED2() asm("BTG LATB,#10")
#define TLED3() asm("BTG LATB,#11")
/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
void initBetriebszeit(uint32_t betriebszeit); 
void setBetriebszeit(uint32_t betriebszeit);
uint32_t getBetriebszeit(void);
