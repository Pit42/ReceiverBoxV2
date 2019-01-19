// *************************************************************************************************
// *__/\\\\\\\\\\\\\____/\\\\\\\\\\\__/\\\\\\\\\\\\\\\____________/\\\_______/\\\\\\\\\____________*
// *__\/\\\/////////\\\_\/////\\\///__\///////\\\/////___________/\\\\\_____/\\\///////\\\_________*
// *___\/\\\_______\/\\\_____\/\\\___________\/\\\______________/\\\/\\\____\///______\//\\\_______*
// *____\/\\\\\\\\\\\\\/______\/\\\___________\/\\\____________/\\\/\/\\\______________/\\\/_______*
// *_____\/\\\/////////________\/\\\___________\/\\\__________/\\\/__\/\\\___________/\\\//________*
// *______\/\\\_________________\/\\\___________\/\\\________/\\\\\\\\\\\\\\\\_____/\\\//__________*
// *_______\/\\\_________________\/\\\___________\/\\\_______\///////////\\\//____/\\\/____________*
// *________\/\\\______________/\\\\\\\\\\\_______\/\\\_________________\/\\\_____/\\\\\\\\\\\\\\\_*
// *_________\///______________\///////////________\///__________________\///_____\///////////////_*
// *_______________________________________________________________________________________________*
// *************************************************************************************************
// * Project Name:
// * File Name:         Eusart.c
// * Created By:        Bendixen
// * Date Created:      April 22, 2014, 7:21 PM
// * Descsription:
// *************************************************************************************************

/*INCLUDE FILES*/
#include "Include.h"

/*LOCAL DEFINITIONS*/

unsigned char SendBuffer[40];
unsigned char BufferHead = 0;
unsigned char BufferTail = 0;

/*PRIVATE FUNCTION PROTOTYPES*/

/*PRIVATE FUNCTIONS*/

/*PUBLIC FUNCTIONS*/

void Init_Transmitter (void)
{
      // Selecting asynchronous mode
    TXSTAbits.SYNC = 0;

    // Enabling serial port
    RCSTAbits.SPEN = 1;

    // Selecting 8bit transmit mode
    TXSTAbits.TX9 = 0;
    
    // Selecting high speed baud rate
    TXSTAbits.BRGH = 1;

    // Selecting 8 bit baud rate generator
    BAUDCTLbits.BRG16 = 1;

    // Setting baudrate to 9600 Hz
    SPBRG = 103;
    
    /*
    // Peripheral interrupts enabled
    INTCONbits.PEIE = 1;

    // Global interrupt enabled
    INTCONbits.GIE = 1;*/

    // Clearing transmit buffer
    TXREG = 0;
         
    // Enabling transmitting
    TXSTAbits.TXEN = 1;
}

void Init_Receiver (void)
{
    // Enabling serial port
    //RCSTAbits.SPEN = 1;

    // Selecting 8bit receive mode
    RCSTAbits.RX9 = 0;

    // Selecting asynchronous mode
    //TXSTAbits.SYNC = 0;

    // Selecting high speed baud rate
    //TXSTAbits.BRGH = 0;
    //TXSTAbits.BRGH = 1;
    // Selecting 8 bit baud rate generator
    //BAUDCTLbits.BRG16 = 1;

    // Setting baudrate to 9600 Hz
    //SPBRG = 12;
    //SPBRG = 103;

    // Receive interrupt enabled
    PIE1bits.RCIE = 1;

    // Peripheral interrupts enabled
    INTCONbits.PEIE = 1;

    // Global interrupt enabled
    INTCONbits.GIE = 1;

    // Clearing receive buffer
    RCREG = 0;

    // Enabling Receiver
    RCSTAbits.CREN = 1;
}

unsigned char Read_Byte (void)
{
    unsigned char Data = RCREG;

    return Data;
}

void Write_Byte (unsigned char Data)
{
    //while (!TXIF);  // wait until TXREG is not busy   
    //while (!TRMT);
    TXREG = Data;
}

void AddByteToBuffer (unsigned char Byte)
{
    SendBuffer[BufferHead] = Byte;
    
    if (BufferHead == 39)
    {
        BufferHead = 0;
    }
    else
    {
        BufferHead++;
    }
       
    // Transmit interrupt enabled
    PIE1bits.TXIE = 1;
    
}

void WriteByteFromBuffer (void)
{
    if (BufferHead != BufferTail)
    {
        Write_Byte(SendBuffer[BufferTail]);
        
        if (BufferTail == 39)
        {
            BufferTail = 0;
        }
        else
        {
            BufferTail++;
        }
        
    }
    
    if (BufferHead == BufferTail)
    {
        PIE1bits.TXIE = 0;
    }

}

/*
char UART_TX_Empty()
{
  return TRMT;
}

void Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
    Write_Byte(text[i]);
}
*/
 
// ***************************************** Pit42 2014 © ******************************************