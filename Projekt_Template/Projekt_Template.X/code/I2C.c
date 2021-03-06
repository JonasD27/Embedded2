/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Beinhaltet Konstanten, Typdefs, globale Variablen und Prototypen f?r main  */
#include "I2C.h"
#include "UART.h"

#include "system.h"        /* System - Funktion/Parameter                     */
#include "user.h"          /* Benutzer - Funktion/Parameter                   */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

I2C_struct I2C_test_struct = {0,0,NULL,0,NULL,NULL};

Buffer_I2C_FSM FIFO_I2C = {{},0,0}; //FIFO f?r die I2C FSM

/******************************************************************************/
/* Funktionen                                                                 */
/******************************************************************************/

/**
 * Legt eine I2C-Anfrage in dem I2C-FIFO ab
 * @param s I2C-Anfrage in Form eines Structs des Typs I2C_struct
 * @retval BUFFER_FAIL im Fehlerfall
 * @retval ansonsten BUFFER_SUCCESS, wenn erfolgreich
 */
int16_t put_I2C_struct_FIFO(I2C_struct s)
{
  _GIE=0;
  if ( ( FIFO_I2C.write + 1 == FIFO_I2C.read ) ||
       ( FIFO_I2C.read == 0 && FIFO_I2C.write + 1 == BUFFER_SIZE ) )
  {
    _GIE=1;  
    return BUFFER_FAIL; // voll
  }

  FIFO_I2C.data[FIFO_I2C.write] = s;

  FIFO_I2C.write++;
  if (FIFO_I2C.write >= BUFFER_SIZE)
  {
    FIFO_I2C.write = 0;
  }
  _GIE=1;
  return BUFFER_SUCCESS;
  
} /* put_I2C_struct_FIFO() */


/**
 * Entnimmt I2C-Anfrage aus dem I2C-FIFO
 * @param *s Zeiger auf I2C-Anfrage in Form eines Structs des Typs I2C_struct
 * @retval BUFFER_FAIL im Fehlerfall
 * @retval ansonsten BUFFER_SUCCESS, wenn erfolgreich
 */
int16_t get_I2C_struct_FIFO(volatile I2C_struct *s)
{
  _GIE=0;  
  if (FIFO_I2C.read == FIFO_I2C.write)
  {
    _GIE=1;  
    return BUFFER_FAIL;
  }
  *s = FIFO_I2C.data[FIFO_I2C.read];

  FIFO_I2C.read++;
  if (FIFO_I2C.read >= BUFFER_SIZE)
  {
    FIFO_I2C.read = 0;
  }
  _GIE=1;
  return BUFFER_SUCCESS;
  
} /* get_I2C_struct_FIFO() */


/**
 * ?bergibt angeforderte I2C-Anfrage an das FIFO und liefert den akutellen 
 * Status 
 * @param address 7 Bit Adresse des Slaves
 * @param num_write Anzahl der zu sendenen Bytes, bei 0 keine Write Zugriff
 * @param writebuf Zeiger auf zu schreibende Daten
 * @param num_read Anzahl der zu lesenden Bytes, bei 0 keine Read Zugriff
 * @param readbuf Zeiger auf Bereich, in welchem Daten abgespeichert werden 
 * sollen
 * @param status Zeiger, um akuellen Status zur?ckzugeben
 * @param callback Zeiger auf eine Callback-Funktion, welche nach Abschluss der 
 * Anfrage aufgerufen werden soll
 * @param ID ID zur Identifikation der Anfrage 
 * @retval 1, Anforderung wurde angenommen, die FSM wird getriggert
 * @retval 0, FSM ist besch?ftig, Anforderung kann nicht angeommen werden
 */
int16_t exchangeI2C(uint8_t address, uint16_t num_write, uint8_t *writebuf, uint16_t num_read, uint8_t *readbuf, i2c_status_t *status, I2C_Callback_t callback, int16_t ID)
{
    I2C_struct temporary_struct = {address,num_write,writebuf,num_read,readbuf,status,callback,ID};
  
    if (put_I2C_struct_FIFO(temporary_struct))   //Erfolgreich in FIFO abgelegt?
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
} /* exchangeI2C() */


/**
 * Wird jede ms in der Superloop ausgef?hrt und beinhalten die FSM f?r die 
 * I2C-Kommunikation.
 * Falls das FIFO neue Anfragen enth?lt wird die FSM getriggert.
 */
void doI2C()
{
  static StateFunc statefunc = FSM_Idle;
  statefunc = (StateFunc)(*statefunc)(); 
} /* doI2C() */

/**
 * Initialisiert die I2C-Kommunikation
 */
void initI2C()
{
    I2C2CONbits.A10M = 0;
    I2C2BRG = 245; //100kHz 

    /**
     * Einschalten I2C mit eigenem Workaround, I2C Peripheriemodul kann hier 
     * leider nicht verwendet werden.
     * Pins wie einen Open-Kollektor-Treiber verwenden, d.h. 1 - hochohmig, 
     * 0 wird getrieben
    */
    I2C_SDA_TRIS = 1;    
    I2C_SCL_TRIS = 1;
    I2C_SDA = 0;    
    I2C_SCL = 0;

        int j;
        for (j=0; j<=9; j++)    // takten bis min 1 Byte
        {
          I2C_SCL_TRIS = 0; delay_ms(1); // 5 us wären ausreichend ...100 kBaud
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
        
        /**
         * Aktiviert das I2C Modul und konfiguriert die Pins SDAx und SCLx 
         * als serielle PORT-Pins.
        */ 
        I2C2CONbits.I2CEN = 1;
        
} /* initI2C() */


/**
 * Kopiert die Anfrage aus dem FIFO und leitet Start-Sequenz ein. 
 * @return FSM_Start, sobald die Startbedingungen an die Pins SDAx und SCLx 
 * weitergeleiet worden sind
 */
void *FSM_Idle(void)
{
    if (get_I2C_struct_FIFO(&I2C_test_struct)) //Anfrage aus FIFO holen m?glich
    {
        I2C2CONbits.SEN=1; // Leite Start-Bedingungen weiter
        return FSM_Start;
    }
    else
    {
        return FSM_Idle;
    }
} /* *FSM_Idle() */


/**
 * Beschreibt das Trancieve-Register mit der Adresse
 * @return FSM_Adresse_Write, sobald geschrieben werden kann
 * @retrun FSM_Adresse_Read, sobald gelesen werden kann
 */
void *FSM_Start(void)
{
    
    if (I2C2CONbits.SEN==0) // Wenn Startbedigungen erf?llt wurden
    {
        if (I2C_test_struct.num_write>0) //Schreiben
        {
            I2C2TRN=(I2C_test_struct.address<<1);
            return FSM_Adresse_Write;
        }
        
        else if (I2C_test_struct.num_read>0)  //Lesen
        {
            I2C2TRN=(I2C_test_struct.address<<1) | 0b1;
            return FSM_Adresse_Read;
        }
        
        
    }
    return FSM_Start; 
    
} /* *FSM_Start() */


/**
 * Schreibt die zu ?bertragende Daten in das Tranceive-Register
 * @param count Zaelervariable
 * @return FSM_Stop, sobald ein Fehler bei der Kommunikation auftritt, z.B kein
 * ACK vom Slave. Es wird die Callback Funktion aufgerufen
 * @return FSM_Adresse_Write, sobald keine Bytes mehr zu senden gibt
 * @return FSM_Repeated_Start, sobald die Bedigungen f?r den wiederholten Start
 * an die Pins SDAx und SClx weitergeleitet wurde.
 */
void *FSM_Adresse_Write(void)
{
    if(I2C2STATbits.TRSTAT==0) //Wenn erfolgreich ?bertragen
    {
        if (I2C2STATbits.ACKSTAT==1) // Leitet Stop-Bedigungen weiter
        {  
            I2C2CONbits.PEN=1;  //Fehler bei Kommunikation
            *I2C_test_struct.status=Error;
            if (I2C_test_struct.callback != NULL)
            {
                 I2C_test_struct.callback(I2C_test_struct.readbuf, I2C_test_struct.num_read, I2C_test_struct.status, I2C_test_struct.ID);
            }
            return FSM_Stop;
        }
        
        if (I2C2STATbits.ACKSTAT==0) //Wenn ACK von Slave erhalten
        {
            static int count=0;
            
            if (count < I2C_test_struct.num_write) //Noch Bytes zu senden
            {
                
                I2C2TRN=I2C_test_struct.writebuf[count];
                count++;
                return FSM_Adresse_Write;
            }
            
            else //Nichts mehr zu schicken
            { 
               count=0;
               I2C2CONbits.RSEN=1;  // Leitet Bedingungen f?r den Restart weiter
               return FSM_Repeated_Start;
            }
            
        }
        
    }
    return FSM_Adresse_Write;  
    
} /* *FSM_Adresse_Write() */

/**
 * Leitet einen Repeated Start ein und beschreibt das Tranceive Register mit
 * der Adresse
 * @return FSM_Adresse_Read, sobald es einen Restart gibt
 */
void *FSM_Repeated_Start(void)
{
    if (I2C2CONbits.RSEN==0)    // Wenn der Restart erfogreich war
    {
        I2C2TRN=(I2C_test_struct.address<<1) | 0b1;
        return FSM_Adresse_Read;
    }
    return FSM_Repeated_Start;
    
} /* *FSM_Repeated_Start() */


/**
 * Initiiert das Lesen der Daten des Slaves
 * @param count Zaelervariable
 * @return FSM_Stop, sobald ein Fehler bei der Kommunikation auftritt, z.B kein
 * ACK vom Slave. Es wird die Callback Funktion aufgerufen
 * @return FSM_RECV_EN, sobald der Empfangsmodus f?r I2C aktiviert wurde
 * @return FSM_Stop, sobald die Stop-Bedigungen an die Pins SDAx und SCLx 
 * weitergeleitet wurden. Es wird die Callback Funktion aufgerufen
 * @return FSM_Adresse_Read, wenn kein ACK vom Slave erhalten oder das Bit der 
 * ACK-Sequenz nicht freigegeben ist 
 */
void *FSM_Adresse_Read(void)
{
    if(I2C2STATbits.TRSTAT==0) //Wenn erfolgreich ?bertragen
    {
        if (I2C2STATbits.ACKSTAT==1) //Wenn NACK von Slave erhalten
        {  
            I2C2CONbits.PEN=1;  // Leitet Stop-Bedigungen weiter 
            *I2C_test_struct.status=Error;   //Fehler bei Kommunikation
            if (I2C_test_struct.callback != NULL)
            {
                 I2C_test_struct.callback(I2C_test_struct.readbuf, I2C_test_struct.num_read, I2C_test_struct.status, I2C_test_struct.ID);
            }
            return FSM_Stop;
        }
        
        if (I2C2STATbits.ACKSTAT==0)    //Wenn ACK von Slave erhalten
        {
            if (I2C2CONbits.ACKEN==0)   //Wenn Bit der ACK-Sequenz freigegeben
            {
                static int count = 0;

                if (count < I2C_test_struct.num_read) //Noch Bytes zu empfangen
                {
                    count++;
                    I2C2CONbits.RCEN=1; // Aktiviert den Empfangsmodus f?r I2C
                    return FSM_RECV_EN;
                }

                else //Nichts mehr zu empfangen
                { 
                   count = 0; 
                   I2C2CONbits.PEN=1;   // Leitet Stop-Bedigungen weiter
                   
                   *I2C_test_struct.status=Finished; //Anforderung abgearbeitet
                   //Callback Funktion aufrufen
                   if (I2C_test_struct.callback != NULL)
                   {
                        I2C_test_struct.callback(I2C_test_struct.readbuf, I2C_test_struct.num_read, I2C_test_struct.status, I2C_test_struct.ID);
                   }
                   return FSM_Stop;
                   
                }
            }
            else
            {
                return FSM_Adresse_Read;
            }
            
        }
        
    }
    return FSM_Adresse_Read;  
    
} /* *FSM_Adresse_Read() */

/**
 * Auslesen des Receive Registers und Best?tigung mit ACK bzw. NACK
 * @param count Zaelervariable
 * @return FSM_Adresse_Read, sobald die Acknowledge-Sequenz an den Pins SDAx
 * und SCLx initiiert wurde und das ACKDT Datenbit ?bertragen wurde
 * @return FSM_RECV_EN, Wenn die Empfangssequenz nicht ausgef?hrt wurde
 */
void *FSM_RECV_EN(void)
{
    if (I2C2CONbits.RCEN==0)    //Wenn der Empfangsmodus aktiviert wurde
    {
        static int count = 0;
        I2C_test_struct.readbuf[count]=I2C2RCV;
        count++;
        
        if (count>=I2C_test_struct.num_read) //Wenn letztes Byte empfangen wurde
        {
            count=0;
            I2C2CONbits.ACKDT=1;   //Sendet einen NACK w?hrend eines Acknowledge
        }
        else
        {
            I2C2CONbits.ACKDT=0;   //Sendet einen ACK w?hrend eines Acknowledge
        }
        
        I2C2CONbits.ACKEN=1;    //Initiiert die Acknowledge-Sequenz
        return FSM_Adresse_Read;
    }
    
    return FSM_RECV_EN;
    
} /* *FSM_RECV_EN() */

/**
 * ?berpr?fung auf Abschluss der Stop-Sequenz und R?ckkehr in den Idle-State
 * @return FSM_Idle, wenn die Stop-Bedingungen erfolgreich an den Pins SDAx und
 * SCLx weitergeleitet wurden
 * @return FSM_Stop, wenn keine Stop-Bedingungen weitergeleitet wurden
 */
void *FSM_Stop(void)
{
    if(I2C2CONbits.PEN==0)  //Wenn die Stop-Bedingungen weitergeleitet wurden
    {  
        return FSM_Idle;
    } 
    return FSM_Stop;
    
} /* *FSM_Stop() */


/**
 * Callback-Funktion zum Augeben der Temperaturwerte
 * @param readbuf Zeiger auf die ausgelesenen Daten
 * @param num_read Anzahl der ausgelesenen Bytes
 * @param status Status mit welchem die FSM die Callback-Funktion aufgerufen 
 * hat. Im Fall Error wird eine Fehlermeldung ausgegeben. Im Fall Finished 
 * werden die Daten interpretiert und ausgegeben.
 * @param ID
 */
void I2C_TempSens_Callback(uint8_t *readbuf, uint16_t num_read, i2c_status_t *status, int16_t ID)
{
    if (ID==0) //Temperatur
    {
        if (*status==Finished)
        {
            
            double temp = readbuf[0] << 8 | readbuf[1];
            temp = temp / 256;

            char str[32]; 
            sprintf(str,"Temperatur: %.1f Grad",temp);
            putsUART(str);
            char lf[2];
            sprintf(lf, "\n"); 
            putsUART(lf);

            latest_temperatur=temp;
            *status=Pending;
        }
        if(*status==Error)
        {
            char str[32]; 
            sprintf(str,"Fehler Auslesen des Temp-Sensors!");
            putsUART(str);
            char lf[2];
            sprintf(lf, "\n"); 
            putsUART(lf);
        }
    }

}

/**
 * Callback-Funktion zum Augeben der Lichtwerte
 * @param readbuf Zeiger auf die ausgelesenen Daten
 * @param num_read Anzahl der ausgelesenen Bytes
 * @param status Status mit welchem die FSM die Callback-Funktion aufgerufen 
 * hat. Im Fall Error wird eine Fehlermeldung ausgegeben. Im Fall Finished 
 * werden die Daten interpretiert und ausgegeben.
 * @param ID
 */
void I2C_LightSens_Callback(uint8_t *readbuf, uint16_t num_read, i2c_status_t *status, int16_t ID)
{
    if (ID==1)//Licht
    {
        if (*status==Finished)
        {
        
        double light = readbuf[0] << 8 | readbuf[1];
        light = light / 1.2;

        char str[16]; 
        sprintf(str,"Licht: %.1f lux",light);
        putsUART(str);
        char lf[2];
        sprintf(lf, "\n"); 
        putsUART(lf);

        *status=Pending;
        }
        
        if(*status==Error)
        {
            char str[32]; 
            sprintf(str,"Fehler Auslesen des Licht-Sensors!");
            putsUART(str);
            char lf[2];
            sprintf(lf, "\n"); 
            putsUART(lf);
        }
    }

}/*I2C_LightSens_Callback()*/

