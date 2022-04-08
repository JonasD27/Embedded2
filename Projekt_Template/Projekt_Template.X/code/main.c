/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#include <xc.h>

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
uint32_t DELAY_ANPASSUNG;

//UART
#define BAUDRATE 9600
#define BRGVAL ((FCY/BAUDRATE)/16)-1

//FIFO
#define BUFFER_FAIL     0
#define BUFFER_SUCCESS  1
#define BUFFER_SIZE 128


//I2C
#define I2C_SCL      _RA2
#define I2C_SDA      _RA3
#define I2C_SCL_TRIS _TRISA2
#define I2C_SDA_TRIS _TRISA3 

uint8_t write_data_buffer_temp;
uint8_t read_data_buffer_temp[2];

bool repeated_start=0;

/*Typen-Definitionen***********************************************************/
typedef enum {Pending, Finished, Error} i2c_status_t;

typedef struct 
{
  uint8_t data[BUFFER_SIZE];
  uint8_t read; // zeigt auf das Feld mit dem ältesten Inhalt
  uint8_t write; // zeigt immer auf leeres Feld
}Buffer;

typedef struct
{
    uint8_t address;
    uint16_t num_write;
    uint8_t *writebuf;
    uint16_t num_read;
    uint8_t *readbuf;
    i2c_status_t status;
}I2C_struct;

I2C_struct I2C_test_struct = {0,0,NULL,0,NULL,Error};

Buffer FIFO = {{}, 0, 0}; //FIFO zum Versenden über UART

typedef void *(*StateFunc)();


/*Prototypes*******************************************************************/
int16_t exchangeI2C(uint8_t address, uint16_t num_write, uint8_t *writebuf, uint16_t num_read, uint8_t *readbuf, i2c_status_t status);

void doI2C(void);

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
void *FSM2_Data_Write(void);
void *FSM2_Stop(void);

/*Funktionen*******************************************************************/

/**
 * Verzögerungsfunktion, blockierend
 * @param milliseconds Anzahl der zu verzögernden Millisekunden
 */
void delay_ms(uint16_t milliseconds) 
{    
    uint32_t i=0;
    for (i=0;i<(DELAY_ANPASSUNG*(uint32_t)milliseconds);i++)
    {
    }
}

#if 0
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    _T1IF = 0; //Clear Timer1 interrupt flag
        
    putsUART("Hello World\n"); 
    exchangeI2C(0b1001000, 0, &write_data_buffer, 2, read_data_buffer, Pending);
        
}
#endif

//UART
void initUART()
{
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

void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
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
  {
    return BUFFER_FAIL; // voll
  }

  FIFO.data[FIFO.write] = c;

  FIFO.write++;
  if (FIFO.write >= BUFFER_SIZE)
  {
    FIFO.write = 0;
  }
  return BUFFER_SUCCESS;
}

int16_t getcFIFO_TX(volatile uint16_t *c)
{
  _LATF0 = 1;
  if (FIFO.read == FIFO.write)
  {
    return BUFFER_FAIL;
  }
  *c = FIFO.data[FIFO.read];

  FIFO.read++;
  if (FIFO.read >= BUFFER_SIZE)
  {
    FIFO.read = 0;
  }
  return BUFFER_SUCCESS;
}

int16_t putcUART(char c)
{
    _LATF0 = 1;
    _GIE = 0; // Interrupts ausschalten
    int16_t erfolg = putcFIFO_TX(c);
    _GIE = 1;
    return erfolg;
}

int16_t putsUART(const char *str) 
{
    _LATF0 = 1;
    uint16_t i;
    uint16_t length = strlen(str);
    
    _GIE = 0;   //Global Interrupt disable
    for(i = 0; i < length; i++) 
    {
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

//I2C

int16_t exchangeI2C(uint8_t address, uint16_t num_write, uint8_t *writebuf, uint16_t num_read, uint8_t *readbuf, i2c_status_t status)
{
    I2C_test_struct.address=address;
    I2C_test_struct.num_write=num_write;
    I2C_test_struct.writebuf=writebuf;
    I2C_test_struct.num_read=num_read;
    I2C_test_struct.readbuf=readbuf;
    I2C_test_struct.status=status;
    _GIE=0;
    doI2C();
    _GIE=1;
    if (I2C_test_struct.status==Finished)
    {
        return 1;
    }
    else if(I2C_test_struct.status==Pending)
    {
        return 0;
    }
}

void doI2C()
{
  static StateFunc statefunc = FSM2_Idle;
  statefunc = (StateFunc)(*statefunc)();  
}

void initI2C()
{
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
}



void *FSM2_Idle(void)
{
    static int c = 0;
    if (c>=999)
    {
        c=0;
        I2C_test_struct.status=Pending;
        I2C2CONbits.SEN=1; //Start
        return FSM2_Start;
        
    }
    c++;
    return FSM2_Idle;

}  

void *FSM2_Start(void)
{
    
    if (I2C2CONbits.SEN==0)
    {
        return FSM2_Adresse;
    }
    return FSM2_Start; 
    
} 

void *FSM2_Adresse(void)
{
    //1 Lesen, 0 Schreiben
    if ((I2C_test_struct.num_write>0) && (repeated_start==0)) //Schreiben
    {
        //Verschieben in Übergang
       I2C2TRN=(I2C_test_struct.address<<1);
       while(I2C2STATbits.TRSTAT==1){} //Warten solange übertragen wird 
       return FSM2_ACK_Receive;
    }
    else if (I2C_test_struct.num_read>0)  //Lesen
    {
       I2C2TRN=(I2C_test_struct.address<<1) | 0b1;
       while(I2C2STATbits.TRSTAT==1){} //Warten solange übertragen wird 
       return FSM2_ACK_Receive;
    }
    else
    {
        return FSM2_Stop;
    }
    
    
} 

void *FSM2_ACK_Receive(void)
{
    if (I2C2STATbits.ACKSTAT==1) //if NACK received, generate stop condition and exit
    {   
        I2C2STATbits.ACKSTAT=0;
        I2C_test_struct.status=Error;
        return FSM2_Stop;
    }
    
    if ((I2C_test_struct.num_write>0) && (repeated_start==0)) //Schreiben
    {
        return FSM2_Data_Write;
    }
    else if (I2C_test_struct.num_read>0) //Lesen
    {
        return FSM2_Data_Receive;
    }
    else
    {
        return FSM2_Stop;
    }
}

void *FSM2_Data_Receive(void)
{
    int N=I2C_test_struct.num_read; 
    int i;
    
    for(i=0;i<N;i++)
    {
        I2C2CONbits.RCEN=1; //Empfangen aktivieren
        while(I2C2CONbits.RCEN==1){} //RCEN cleared automatically when SSP1IF goes high

        I2C_test_struct.readbuf[i]=I2C2RCV;

        //ACK sequence
        if (i<N-1)
        { 
            I2C2CONbits.ACKDT=0; 
        } //jedes byte mit ACK bestätigen
        else {I2C2CONbits.ACKDT=1;} //send NACK if this is the last Byte

        I2C2CONbits.ACKEN=1; //start ack/nack sequence
        while(I2C2CONbits.ACKEN==1){}

    } //end for loop
    return FSM2_Stop;
}

void *FSM2_Data_Write(void)
{
    int N=I2C_test_struct.num_write;
    int i;
    
    //auch blockierend, sollte immer weider zurück in anderen Zustand gehen
    for(i=0;i<N;i++)
    {
      I2C2TRN = *(I2C_test_struct).writebuf;
      while(I2C2STATbits.TRSTAT==1)
      {}
      if (I2C2STATbits.ACKSTAT==1) //if NACK received, generate stop condition and exit
      {   
        I2C2STATbits.ACKSTAT=0;
        I2C_test_struct.status=Error;
        return FSM2_Stop;
      }
    }
    
    
    // Repeated Start
    repeated_start=1;
    I2C2CONbits.RSEN=1;
    while(I2C2CONbits.RSEN==1){} 
    return FSM2_Adresse;
}

void *FSM2_Stop(void)
{
    I2C2CONbits.PEN=1;
    while(I2C2CONbits.PEN==1){} //wait for the stop interrupt
    
    //Repeated Start
    repeated_start=0;
    I2C_test_struct.status=Finished;
    return FSM2_Idle;
}

void print_Temp()
{
    static int c = 0;
    if (c>=999)
    {
        c=0;
        double temp = read_data_buffer_temp[0]<<8|read_data_buffer_temp[1];
        char str[16];
        sprintf(str,"%f",temp/256);
        putsUART("Temperatur: ");
        putsUART(str);
        putsUART("°C");
        putsUART("\n");
    }
    c++;  
}


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    DELAY_ANPASSUNG = ((SYS_FREQ/96)*2180ull)/1000000ull; //Berechnung der Delay Anpassung
    uint16_t Count = 0;
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    /* Initialize IO ports and peripherals */
    //InitApp();
    initUART();
    #if 0
    init_timer1();
    #endif
    init_ms_t4();
    initI2C();

    _RP66R = _RPOUT_U1TX; //UART Pin Mapping
    RPINR18bits.U1RXR = 0b1011000;
    
    
    write_data_buffer_temp=0b00000000;
    while(1)
    {
        if(_T4IF)
        {
            _T4IF=0;
            Count++;
            if (Count >= HEARTBEAT_MS)
            {
                Count = 0;
                //Temp
                exchangeI2C(0b1001000, 1, &write_data_buffer_temp, 2, read_data_buffer_temp, Pending);
                print_Temp();
                //exchange wird zum bsp alle 10s aufgerufen, um daten anzufordern
                //doI2C sollte zyklisch, zb alle 1ms aufgerufen werden, schaut nach ob es gerade eine Anforderung gibt, wenn ja FSM durchlaufen
                //Wenn Tempepratur und Licht, dann muus geschaut werden dass nicht eine neue Anforderung gestellt werden kann wenn noch eine andere läuft
                
                //TODO
                //FSM fixen, nicht blockierend, kein while{}
                //exchange alle 1s, doI2C in superloop

            }
            
        }
    }
}
