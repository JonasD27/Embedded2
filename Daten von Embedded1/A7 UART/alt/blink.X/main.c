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


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
#define HEARTBEAT_MS 10
#define MESSZEIT_MS 1000 //gilt nur für HEARTBEAT_MS=10
#define BLINKZEIT 500
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1
#define BUFFER_FAIL     0
#define BUFFER_SUCCESS  1
#define BUFFER_SIZE 16
/* i.e. uint16_t <variable_name>; */
uint32_t u32_uptime_seconds = 0;
uint32_t u32_inactive_seconds = 0;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

/**
 * Software Delay mit for Schleife, Blockierung
 * 167 muss evtl. angepasst werden
 *
 * @param milliseconds Wartezeit, blockierend
 * @todo Anpassung an unterschiedliche Systemfrequenzen
 */

void delay_ms_t1(uint16_t milliseconds){
    int Count = 0;
    while(Count < milliseconds){
        while (! _T1IF){
            Nop();
            }
    
        _T1IF = 0;
        Count++;
        }   
    }


uint32_t DELAY_ANPASSUNG;

void delay_ms(uint16_t milliseconds) {    
    uint32_t i=0;
    for (i=0;i<(DELAY_ANPASSUNG*(uint32_t)milliseconds);i++){
         }
}

//Interrupt Service Routine
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
    {
        _T1IF = 0; //Clear Timer1 interrupt flag
        u32_uptime_seconds++;
        //u32_inactive_seconds++;
        /*if (u32_inactive_seconds >= 10){
            LED0 = 0;
            //Sleep();
            Idle();
        }  */
    }
//Change Notification Interrupt Routine
 /*void __attribute__((interrupt,auto_psv)) _CNInterrupt(void)               
 {    
     //LED0 = 1;
     //asm("BSET 0xE15, #0");
     _CNIF = 0;
  
     u32_inactive_seconds = 0;
 }*/

void initBetriebszeit(uint32_t betriebszeit){
    u32_uptime_seconds = betriebszeit;
    __builtin_write_OSCCONL(0b00000011); //SOSC aktivieren
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCS = 1; // Select external clock
    T1CONbits.TSYNC = 0; // Disable Synchronization
    T1CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR1 = 0x00; // Clear timer register
    PR1 = 32767; // Load the period value, Quarztakt
    
    IPC0bits.T1IP = 2; // Set Timer 1 Interrupt Priority Level
    IFS0bits.T1IF = 0; // Clear Timer 1 Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer1 interrupt
    T1CONbits.TON = 1; // Start Timer
}

void setBetriebszeit(uint32_t betriebszeit){
    _GIE = 0;
    u32_uptime_seconds = betriebszeit;
    _GIE = 1;
}

uint32_t getBetriebszeit(void){
    _GIE = 0;
    uint32_t temp = u32_uptime_seconds;
    _GIE = 1;
    return temp;
}

void ShowBetreibszeitLED(){
    static uint16_t counter;
    counter++;
    if (counter == 980){
        LATB = ((0<<8) | (0<<9) | (0<<10) | (0<<11));
        LATB |= ((!(bool)(u32_uptime_seconds & 0b00000100) << 8) | (!(bool)(u32_uptime_seconds & 0b00001000) << 9) | (!(bool)(u32_uptime_seconds & 0x10) << 10) | (!(bool)(u32_uptime_seconds & 0x20) << 11)); 

    }
    if (counter == 1000){
        counter =0;
        LATB = ((0<<8) | (0<<9) | (0<<10) | (0<<11));
        LATB |= (((bool)(u32_uptime_seconds & 0b00000100) << 8) | ((bool)(u32_uptime_seconds & 0b00001000) << 9) | ((bool)(u32_uptime_seconds & 0x10) << 10) | ((bool)(u32_uptime_seconds & 0x20) << 11)); 
    }

}

void initUART(){
    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.ABAUD = 0; // Auto-Baud disabled
    U1MODEbits.UEN = 0;
    U1MODEbits.LPBACK = 0;
    U1MODEbits.RXINV = 0;
    //U1MODEbits.ALTIO = 0;
    
    U1MODEbits.URXINV = 0;
    U1MODEbits.RTSMD = 0;
    
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    U1BRG = BRGVAL; // Baud Rate setting for 9600
    
    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.UTXBRK = 0;
    U1STAbits.ADDEN = 0;
    U1STAbits.UTXINV = 0;
    
    U1MODEbits.UARTEN = 1; // Enable UART
    //delay_ms(2);
    U1STAbits.UTXEN = 1; // Enable UART TX
    
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    delay_ms(2); 
    _U1TXIE = 1; // Enable UART TX interrupt
}
//IMmer wenn FIFO Register leer sind wird ein interrupt generiert

void __attribute__((__interrupt__)) _U1TXInterrupt(void)
{ 
    _U1TXIF = 0; // Clear TX Interrupt flag
    //getcFIFO_TX(&U1TXREG);
    //U1TXREG = 'f'; // Transmit one character
    
}
/*----------------------------------*/

typedef struct {
  uint8_t data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
}Buffer;

Buffer FIFO = {{}, 0, 0};

int16_t putcFIFO_TX(char c)
{
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhöht sicherheit

  if ( ( FIFO.write + 1 == FIFO.read ) ||
       ( FIFO.read == 0 && FIFO.write + 1 == BUFFER_SIZE ) )
    return BUFFER_FAIL; // voll

  FIFO.data[FIFO.write] = c;

  FIFO.write++;
  if (FIFO.write >= BUFFER_SIZE)
    FIFO.write = 0;

  return BUFFER_SUCCESS;
}

int16_t getcFIFO_TX(char *c)
{
  if (FIFO.read == FIFO.write)
    return BUFFER_FAIL;

  *c = FIFO.data[FIFO.read];

  FIFO.read++;
  if (FIFO.read >= BUFFER_SIZE)
    FIFO.read = 0;

  return BUFFER_SUCCESS;
}

int16_t putcUART(char c){
    _GIE = 0; // Interrupts ausschalten
    uint16_t erfolg = putcFIFO_TX(c);
    _GIE = 1;
    return erfolg;


}

int16_t main(void)
{
    putcUART('a');
    putcUART('b');
    putcUART('c');
    
   

    //Sleep();
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
   
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();
    //init_ms_t1();
    init_ms_t2(); //Timer 2 
    initBetriebszeit(0); //Timer 1 für Betriebszeit zählen
    initUART();
    
    //Alle LEDs als Ausgang
    _TRISB8 = 0;
    _TRISB9 = 0;
    _TRISB10 = 0;
    _TRISB11 = 0;
    _TRISF0 = 0;
    //_TRISD2 = 0;
    //Alle LEDs als digital
    _ANSB8 = 0;
    _ANSB9 = 0;
    _ANSB10 = 0;
    _ANSB11 = 0;
    _RP66R = _RPOUT_U1TX;
    //_ANSD2 = 0;
    //Taster INC G9 als digital
    _ANSG9 = 0;
    //Taster als Eingang
    _TRISG14 = 1;
    _TRISG15 = 1;
    _TRISG12 = 1;
    _TRISG13 = 1;
    _TRISG9 = 1;
    //Pull-up Widerstände einschalten
    _CNPUG14 = 1;
    _CNPUG15 = 1;
    _CNPUG12 = 1;
    _CNPUG13 = 1;
    _CNPUG9 = 1;

    uint16_t Count = 0;
    
    //Timer Input auf RP96 mappen
    //_T2CKR = 96;
    
    //_CNIE = 1; //Change Notification Interrupt Enable 
    _CNIP = 1; // Interupt priority heruntersetzten
    _CNIEG14 = 1; //CN für Taster aktivieren
    _CNIEG15 = 1;
    _CNIEG12 = 1;
    _CNIEG13 = 1;
    _CNIEG9 = 1;
    

    
    
    while(1)
    {
     //_LATF0 = 0; 
      
        if (_T2IF) {
            _T2IF = 0; //Flag clearen
            Count++;
            if (Count >= HEARTBEAT_MS){ 
                //_LATF0 = 1;
                Count = 0;
                //ShowBetreibszeitLED();
                //U1TXREG = 'a';
                //doIdle();
            }
        } 
    }
}



/*------------------------------------------------------*/







