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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
#define HEARTBEAT_MS 1
//UART

#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1
//FIFO

#define BUFFER_FAIL     0
#define BUFFER_SUCCESS  1
#define BUFFER_SIZE 128
uint32_t DELAY_ANPASSUNG;

//I2C
#define I2C_SCL      _RA2
#define I2C_SDA      _RA3
#define I2C_SCL_TRIS _TRISA2
#define I2C_SDA_TRIS _TRISA3 
uint8_t data[2];

/*Typen-Definitionen***********************************************************/

typedef struct {
  uint8_t data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
}Buffer;

Buffer FIFO = {{}, 0, 0};

typedef void *(*StateFunc)();


/*Prototypes*******************************************************************/
void init_ms_t4(void);

int16_t putsUART(const char *str);
int16_t getcFIFO_TX(volatile uint16_t *c);
//int16_t getcFIFO_RX(char *c);

int16_t putcFIFO_TX(char c);
//int16_t putcFIFO_RX(char c);

void *FSM2_Idle(void);
void *FSM2_Start(void);
void *FSM2_Adresse(void);
void *FSM2_ACK_Receive(void);
void *FSM2_Data_Receive(void);
void *FSM2_Stop(void);
void Temp_FSM2(void);

/*Funktionen*******************************************************************/
/**
 * @brief Delay in ms
 * Blockierende Delay Funktion, eventuell nicht perfekte Verzögerung mit Hilfe einer for Schleife
 * @param [in] milliseconds Verzögerungszeit in millisekunden
 */
void delay_ms(uint16_t milliseconds) {    
    uint32_t i=0;
    for (i=0;i<(DELAY_ANPASSUNG*(uint32_t)milliseconds);i++){
         }
}

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
    {
        _T1IF = 0; //Clear Timer1 interrupt flag
        
        putsUART("Hello World\n");  
        
    }

//UART
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
    U1STAbits.URXISEL = 0;
    U1STA = U1STA | 0b0001000000000000;
    //_URXEN = 1;
    
    //_U1RXIE = 1; // Enable UART RX interrupt
    
    U1MODEbits.UARTEN = 1; // Enable UART
    //delay_ms(2);
    U1STAbits.UTXEN = 1; // Enable UART TX
    
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    delay_ms(2); 
    _U1TXIE = 1; // Enable UART TX interrupt
    
}

void __attribute__((__interrupt__)) _U1TXInterrupt(void)
{ 
    _U1TXIF = 0; // Clear TX Interrupt flag
    
    getcFIFO_TX(&U1TXREG);
    
}




int16_t putcFIFO_TX(char c)
{
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhöht sicherheit
  _LATF0 = 1;
  if ( ( FIFO.write + 1 == FIFO.read ) ||
       ( FIFO.read == 0 && FIFO.write + 1 == BUFFER_SIZE ) )
    return BUFFER_FAIL; // voll

  FIFO.data[FIFO.write] = c;

  FIFO.write++;
  if (FIFO.write >= BUFFER_SIZE)
    FIFO.write = 0;

  return BUFFER_SUCCESS;
}

int16_t getcFIFO_TX(volatile uint16_t *c)
{
  _LATF0 = 1;
  if (FIFO.read == FIFO.write)
    return BUFFER_FAIL;

  *c = FIFO.data[FIFO.read];

  FIFO.read++;
  if (FIFO.read >= BUFFER_SIZE)
    FIFO.read = 0;

  return BUFFER_SUCCESS;
}

int16_t putcUART(char c){
    _LATF0 = 1;
    _GIE = 0; // Interrupts ausschalten
    int16_t erfolg = putcFIFO_TX(c);
    _GIE = 1;
    return erfolg;


}

int16_t putsUART(const char *str) {
    _LATF0 = 1;
    uint16_t i;
    uint16_t length = strlen(str);
    
    _GIE = 0;   //Global Interrupt disable
    for(i = 0; i < length; i++) {
        //uint16_t ret = putcFIFO_TX(str[i]);
        if(! putcFIFO_TX(str[i]))
            break;
    }
    _GIE = 1;
    int16_t erfolg = -i;
    if(erfolg == -length)
        erfolg *= -1;
    _U1TXIF = 1; //Interuppt Routine Starten um FIFO-Inhalt zu senden
    return erfolg;
}

//Timer1
void init_timer1(){
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

//I2C

void Temp_FSM2(void)
{
     static StateFunc statefunc = FSM2_Idle;
     
     statefunc = (StateFunc)(*statefunc)();
}

void initI2C(){
    I2C2CONbits.A10M = 0;
    I2C2BRG = 245; //100kHz 

    // Einschalten I2C mit eigenem Workaround, I2C Peripheriemodul kann hier leider nicht verwendet werden
    I2C_SDA_TRIS = 1;    // Pins wie einen Open-Kollektor-Treiber verwenden, d.h. 1 - hochohmig, 0 wird getrieben
    I2C_SCL_TRIS = 1;
    I2C_SDA = 0;    
    I2C_SCL = 0;

        int j;
        for (j=0; j<=9; j++)    // takten bis min 1 Byte
        {
          I2C_SCL_TRIS = 0; delay_ms(1); // 5 us wÃ¤ren ausreichend ...100 kBaud
          I2C_SCL_TRIS = 1; delay_ms(1);
        }
        // Start Condition senden
        I2C_SCL_TRIS = 0;  delay_ms(1);
        I2C_SDA_TRIS = 0;  delay_ms(1);
        // Stop Condition senden
        I2C_SCL_TRIS = 1;  delay_ms(1);
        I2C_SDA_TRIS = 1;  delay_ms(1);

        // Nun I2C erst anschalten
        _MI2C2IF = 0; //Interrupt falls noetig
        _MI2C2IE = 0;  
        I2C2CONbits.I2CEN = 1;
        
    //Sensor Pointer auf TEMP Register setzten    
    I2C2CONbits.SEN=1; //start
    while(I2C2CONbits.SEN==1){} 

    //Tx Device address + Write bit
    I2C2TRN=0b10010000;
    while(I2C2STATbits.TRSTAT==1){}

    if (I2C2STATbits.ACKSTAT==1){   //if NACK received, generate stop condition and exit
        I2C2STATbits.ACKSTAT=0;
        I2C2CONbits.PEN=1;
        while(I2C2CONbits.PEN==1){} //wait for the stop interrupt;
        return;
    }

    //Tx Register Address
    I2C2TRN=0b00000000; //Pointer auf TEMP REGISTER setzten
    while(I2C2STATbits.TRSTAT==1){}

    if (I2C2STATbits.ACKSTAT==1){   //if NACK received, generate stop condition and exit
        I2C2STATbits.ACKSTAT=0;
        I2C2CONbits.PEN=1;
        while(I2C2CONbits.PEN==1){} //wait for the stop interrupt;
        return;
    }
    
    I2C2CONbits.PEN=1; //stop
    while(I2C2CONbits.PEN==1){} //wait for the stop interrupt
}



void *FSM2_Idle(void)
{
    static int c = 0;
    if (c>=999){
        c=0;
        return FSM2_Start;
    }
    c++;
    return FSM2_Idle;

}  

void *FSM2_Start(void)
{
    I2C2CONbits.SEN=1; //Start
    while(I2C2CONbits.SEN==1){} 
    return FSM2_Adresse;
} 

void *FSM2_Adresse(void)
{
    //Tx Device address + Read bit
    I2C2TRN=0b10010001; 
    while(I2C2STATbits.TRSTAT==1){} //Warten solange übertragen wird 
    return FSM2_ACK_Receive;
} 

void *FSM2_ACK_Receive(void)
{
    if (I2C2STATbits.ACKSTAT==1){   //if NACK received, generate stop condition and exit
        I2C2STATbits.ACKSTAT=0;
        return FSM2_Stop;
    }
    return FSM2_Data_Receive;
}

void *FSM2_Data_Receive(void)
{
    int N=2; //2 bytes empfangen  
    int i;
    
    for(i=0;i<N;i++){
        I2C2CONbits.RCEN=1; //Empfangen aktivieren
        while(I2C2CONbits.RCEN==1){} //RCEN cleared automatically when SSP1IF goes high

        data[i]=I2C2RCV;

        //ACK sequence
        if (i<N-1){ I2C2CONbits.ACKDT=0; } //jedes byte mit ACK bestätigen
        else {I2C2CONbits.ACKDT=1;} //send NACK if this is the last Byte

        I2C2CONbits.ACKEN=1; //start ack/nack sequence
        while(I2C2CONbits.ACKEN==1){}

    } //end for loop
    return FSM2_Stop;
}

void *FSM2_Stop(void)
{
    I2C2CONbits.PEN=1;
    while(I2C2CONbits.PEN==1){} //wait for the stop interrupt
    
    float temp = data[0]<<8|data[1];
    char str[16];
    sprintf(str,"%f",temp/256);
    putsUART("Temperatur: ");
    putsUART(str);
    putsUART("°C");
    putsUART("\n");
 
    return FSM2_Idle;
}


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/


/**************************************************************************/

int16_t main(void)
{
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
    //uint16_t Count = 0;
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
    InitApp();
    
    //initUART();
    //init_timer1();
    init_ms_t4();
    //initI2C();
    
    
    TRISBbits.TRISB8 = 0; //LED0 als Ausgang
    ANSELBbits.ANSB8 = 0; //LED0 als Digitaler Ausgang
    
    TRISBbits.TRISB9 = 0; //LED als Ausgang
    ANSELBbits.ANSB9 = 0;
    
   //Taster als Eingänge
    _TRISG12 = 1;
    //Pull-up Widerstände einschalten
    _CNPUG12 = 1;
  

    _RP66R = _RPOUT_U1TX; //UART Pin Mapping
    RPINR18bits.U1RXR = 0b1011000;
    /* TODO <INSERT USER APPLICATION CODE HERE> */
    
    while(1)
    {
        PORTBbits.RB8=1;
        delay_ms(200);
        PORTBbits.RB8=0;
        delay_ms(200);
        //if(_T4IF)
        //{
            //_T4IF=0;
            //Count++;
            //if (Count >= HEARTBEAT_MS)
            //{
                //Count = 0;
                //Temp_FSM2();
            //}
            
        //}
    }
}
