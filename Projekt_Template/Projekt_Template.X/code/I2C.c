#include "I2C.h"
#include "system.h"
#include "UART.h"

//Funktionen

int16_t put_I2C_struct_FIFO(I2C_struct s)
{
  if ( ( FIFO_I2C.write + 1 == FIFO_I2C.read ) ||
       ( FIFO_I2C.read == 0 && FIFO_I2C.write + 1 == BUFFER_SIZE ) )
  {
    return BUFFER_FAIL; // voll
  }

  FIFO_I2C.data[FIFO_I2C.write] = s;

  FIFO_I2C.write++;
  if (FIFO_I2C.write >= BUFFER_SIZE)
  {
    FIFO_I2C.write = 0;
  }
  return BUFFER_SUCCESS;
}

int16_t get_I2C_struct_FIFO(volatile I2C_struct *s)
{
  if (FIFO_I2C.read == FIFO_I2C.write)
  {
    return BUFFER_FAIL;
  }
  *s = FIFO_I2C.data[FIFO_I2C.read];

  FIFO_I2C.read++;
  if (FIFO_I2C.read >= BUFFER_SIZE)
  {
    FIFO_I2C.read = 0;
  }
  return BUFFER_SUCCESS;
}

int16_t exchangeI2C(uint8_t address, uint16_t num_write, uint8_t *writebuf, uint16_t num_read, uint8_t *readbuf, i2c_status_t *status)
{
    I2C_struct temporary_struct = {address,num_write,writebuf,num_read,readbuf,Pending};
    put_I2C_struct_FIFO(temporary_struct);
    
    *status = I2C_test_struct.status;
    
    if (I2C_test_struct.status==Finished)
    {
     
        return 1;
    }
    else
    {
        return 0;
    }
}

void doI2C()
{
  static StateFunc statefunc = FSM_Idle;
  
  if (!(FIFO_I2C.read == FIFO_I2C.write)) //Wenn Ihnalt im FIFO ist
  {
      trigger_FSM=1;  
  }
  
  if (trigger_FSM==1)
  {
      statefunc = (StateFunc)(*statefunc)(); 
  }
   
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



void *FSM_Idle(void)
{
    get_I2C_struct_FIFO(&I2C_test_struct);
    I2C2CONbits.SEN=1; //Start
    return FSM_Start;

}  

void *FSM_Start(void)
{
    
    if (I2C2CONbits.SEN==0)
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
    
} 

void *FSM_Adresse_Write(void)
{
    if(I2C2STATbits.TRSTAT==0) //Wenn erfolgreich übertragen
    {
        if (I2C2STATbits.ACKSTAT==1) //if NACK received, generate stop condition and exit
        {  
            I2C2CONbits.PEN=1;
            I2C_test_struct.status=Error;
            return FSM_Stop;
        }
        
        if (I2C2STATbits.ACKSTAT==0) //ACK von Slave erhalten
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
               I2C2CONbits.RSEN=1;
               return FSM_Repeated_Start;
            }
            
        }
        
    }
    return FSM_Adresse_Write;  
    
} 

void *FSM_Repeated_Start(void)
{
    if (I2C2CONbits.RSEN==0)
    {
        I2C2TRN=(I2C_test_struct.address<<1) | 0b1;
        return FSM_Adresse_Read;
    }
    return FSM_Repeated_Start;
}

void *FSM_Adresse_Read(void)
{
    if(I2C2STATbits.TRSTAT==0) //Wenn erfolgreich übertragen
    {
        if (I2C2STATbits.ACKSTAT==1) //if NACK received, generate stop condition and exit
        {  
            I2C2CONbits.PEN=1;
            I2C_test_struct.status=Error;
            return FSM_Stop;
        }
        
        if (I2C2STATbits.ACKSTAT==0)
        {
            static int count = 0;
            
            if (count < I2C_test_struct.num_read) //Noch Bytes zu empfangen
            {
                count++;
                I2C2CONbits.RCEN=1;
                return FSM_RECV_EN;
            }
            
            else //Nichts mehr zu empfangen
            { 
               count = 0; 
               I2C2CONbits.PEN=1;
               I2C_test_struct.status=Finished;
               return FSM_Stop;
            }
            
        }
        
    }
    return FSM_Adresse_Read;  
    
} 

void *FSM_RECV_EN(void)
{
    if (I2C2CONbits.RCEN==0)
    {
        static int count = 0;
        I2C_test_struct.readbuf[count]=I2C2RCV;
        count++;
        
        if (count>=I2C_test_struct.num_read) //Wenn letztes Byte empfangen wurde
        {
            count=0;
            I2C2CONbits.ACKDT=1; //NACK
        }
        else
        {
            I2C2CONbits.ACKDT=0; //ACK
        }
        
        I2C2CONbits.ACKEN=1;
        return FSM_RECV_ACK_EN;
    }
    
    return FSM_RECV_EN;
    
}

void *FSM_RECV_ACK_EN(void)
{
    if (I2C2CONbits.ACKEN==0)
    {
        return FSM_Adresse_Read;
    }
    
    return FSM_RECV_ACK_EN;
    
}


void *FSM_Stop(void)
{
    if(I2C2CONbits.PEN==0)
    {  
        trigger_FSM=0;
        return FSM_Idle;
    } 
    return FSM_Stop;
}

void print_sensor_values()
{
    //Temperatur
    double temp = read_data_buffer_temp[0]<<8|read_data_buffer_temp[1];
    char str[16];
    sprintf(str,"%.1f",temp/256);
    putsUART("Temperatur: ");
    putsUART(str);
    putsUART("°C");
    putsUART("\n");
    
    //Licht
    double light = read_data_buffer_light[0]<<8 | read_data_buffer_light[1];
    
    sprintf(str,"%.1f",light/1.2);
    putsUART("Licht: ");
    putsUART(str);
    putsUART(" lux");
    putsUART("\n");
 
}