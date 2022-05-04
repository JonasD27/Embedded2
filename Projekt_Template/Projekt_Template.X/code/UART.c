/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "UART.h"
#include "system.h"
#include "lcd_gpio.h"


Buffer FIFO = {{}, 0, 0}; //FIFO zum Versenden über UART
Buffer FIFO_RX = {{}, 0, 0}; //FIFO zum Empfangen über UART
/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

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
    //_U1RXEN = 1;
    
    _U1RXIE = 1; // Enable UART RX interrupt
    
    U1MODEbits.UARTEN = 1; // Enable UART
    delay_ms(2);
    U1STAbits.UTXEN = 1; // Enable UART TX
    
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    delay_ms(2); 
    _U1TXIE = 1; // Enable UART TX interrupt
    
} /* initUART() */


void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{ 
    _U1TXIF = 0; // Clear TX Interrupt flag
    getcFIFO_TX(&U1TXREG);
    
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{   
    _U1RXIF = 0;
    char dummy;
    static int aufrufe = 0;
    if (aufrufe != 0)
    {
        //putcFIFO_RX(U1RXREG);
        received_UART[UART_RX_count]=U1RXREG;
        received_UART[UART_RX_count+1]='\0';
        UART_RX_count++;

    }
    
    else
    {
        dummy = U1RXREG;
    }
    aufrufe = 1;
}

int16_t putcFIFO_TX(char c)
{
  _GIE=0;  
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhöht sicherheit
  _LATF0 = 1;
  if ( ( FIFO.write + 1 == FIFO.read ) ||
       ( FIFO.read == 0 && FIFO.write + 1 == BUFFER_SIZE ) )
  {
    _GIE=1;   
    return BUFFER_FAIL; // voll
  }

  FIFO.data[FIFO.write] = c;

  FIFO.write++;
  if (FIFO.write >= BUFFER_SIZE)
  {
    FIFO.write = 0;
  }
  _GIE=1; 
  return BUFFER_SUCCESS;
  
} /* putcFIFO_TX() */

int16_t getcFIFO_TX(volatile uint16_t *c)
{
  _GIE=0; 
  if (FIFO.read == FIFO.write)
  {
    _GIE=1;  
    return BUFFER_FAIL;
  }
  *c = FIFO.data[FIFO.read];

  FIFO.read++;
  if (FIFO.read >= BUFFER_SIZE)
  {
    FIFO.read = 0;
  }
  _GIE=1;
  return BUFFER_SUCCESS;
  
} /* getcFIFO_TX() */

int16_t putcUART(char c)
{
    _LATF0 = 1;
    _GIE = 0; // Interrupts ausschalten
    int16_t erfolg = putcFIFO_TX(c);
    _GIE = 1;
    return erfolg;
    
} /* putcUART() */

int16_t putsUART(const char *str) 
{
    uint16_t i;
    uint16_t length = strlen(str);
    
    _GIE = 0;   //Global Interrupt disable
    for(i = 0; i < length; i++) 
    {
        if(! putcFIFO_TX(str[i]))
            break;
    }
    _GIE = 1;
    int16_t erfolg = -i;
    if(erfolg == -length)
        erfolg *= -1;
    _U1TXIF = 1; //Interuppt Routine Starten um FIFO-Inhalt zu senden
    return erfolg;
    
} /* putsUART() */


int16_t putcFIFO_RX(char c)
{
  _GIE=0;
  //if (buffer.write >= BUFFER_SIZE)
  //  buffer.write = 0; // erhöht sicherheit
  if ( ( FIFO_RX.write + 1 == FIFO_RX.read ) ||
       ( FIFO_RX.read == 0 && FIFO_RX.write + 1 == BUFFER_SIZE ) )
  {
     return BUFFER_FAIL; // voll
     _GIE=1;
  }
    

  FIFO_RX.data[FIFO_RX.write] = c;

  FIFO_RX.write++;
  if (FIFO_RX.write >= BUFFER_SIZE)
  {
    FIFO_RX.write = 0;
  }
  _GIE=1;
  return BUFFER_SUCCESS;
}


int16_t getcFIFO_RX(char *c)
{
  _GIE=0;
  if (FIFO_RX.read == FIFO_RX.write)
  {
    _GIE=1;  
    return BUFFER_FAIL;
  }

  *c = FIFO_RX.data[FIFO_RX.read];

  FIFO_RX.read++;
  if (FIFO_RX.read >= BUFFER_SIZE)
  {
    FIFO_RX.read = 0;
  }
  _GIE=1;
  return BUFFER_SUCCESS;
}