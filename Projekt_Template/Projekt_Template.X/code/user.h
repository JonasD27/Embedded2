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
/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */

void setLED(uint16_t nr);