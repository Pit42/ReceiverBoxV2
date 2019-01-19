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
// * Project Name:      Remote OGC
// * File Name:         Main.c
// * Created By:        Bendixen
// * Date Created:      April 7, 2014, 5:01 PM
// * Descsription:
// *************************************************************************************************

/*INCLUDE FILES*/
#define _XTAL_FREQ 8000000
#include "Include.h"

/*LOCAL DEFINITIONS*/
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)

/*
typedef struct
{
    unsigned char Start; 
    unsigned char Type; // The type of this Device (a-zA-Z0-9)
    unsigned char Unique; // The unique ID of this Device (a-zA-Z0-9)
    unsigned char Address; // The address of the Input/Output that should be controlled (a-zA-Z0-9)
    unsigned char Commandlength; // Length in Bytes of the following command (128 - 255)
} UglyType;
 */

enum Modes
{
    Running,
    Config
};

unsigned char CurrentMode;


/*PRIVATE FUNCTION PROTOTYPES*/
void Init_Ports (void);
void CheckStopTime (void);
void SetStopTime(unsigned char *RelayStopTime, unsigned char Time10sec, unsigned char Time1sec, unsigned char Time100ms);
void HandleConfigByte (void);
void __interrupt () Handle_Interrupts (void);
void Handle_Telegram (TelegramType ThisTelegram);
void Say_Hello (void);

/*PRIVATE FUNCTIONS*/
void main (void)
{

    Init_Ports();
    Init_Timer();
    Init_Relays();
    Init_Transmitter();
    Init_Receiver();
    Start_Timer();

    AddByteToBuffer('X');
    //Say_Hello();

    CurrentMode = Running;
    
    while (1)
    {

        
        if (StopRelay1OnTime == 1 || StopRelay2OnTime == 1 || StopRelay3OnTime == 1 || StopRelay4OnTime == 1 || StopRelay5OnTime == 1 || StopRelay6OnTime == 1)
        {
            CheckStopTime();
        }

        if (TelegramReadyFlag == 1)
        {
            TelegramReadyFlag = 0;
            Handle_Telegram(Telegram);
        }
            
    }
}

void Say_Hello (void)
{
    TelegramType ThisTelegram;
    ThisTelegram.Start = ':';
    ThisTelegram.Address = '0';
    ThisTelegram.Commandlength = 5;
    ThisTelegram.Command[0] = 'H';
    ThisTelegram.Command[1] = 'E';
    ThisTelegram.Command[2] = 'L';
    ThisTelegram.Command[3] = 'L';
    ThisTelegram.Command[4] = 'O';
    ThisTelegram.End[0] = 0x0D;
    ThisTelegram.End[1] = 0x0A;
    
    SendTelegram(ThisTelegram);
}

void Init_Ports (void)
{
    // Setting RA4 as Input
    // RA4 is used to reset the SiteID of the TinyMesh Module
    TRISAbits.TRISA4 = 1;
    
    // Setting RA5 as Input
    // RA5 connects to the Config Pin on the TinyMesh Module which is active low
    TRISAbits.TRISA5 = 1;
 
    //Setting corresponding tris pin
    TRISCbits.TRISC4 = 1;
    
    // Setting corresponding tris pin
    TRISCbits.TRISC5 = 1;


    // Disabling comparator, setting assosiated ports as I/O ports
    CMCON0bits.CM = 7;

    // Setting all A/D ports to I/O ports
    ANSEL = 0;

    // Enabling External Interrupt
    INTCONbits.RAIE = 1;
    
    // Enabling Interrupt on RA4
    IOCAbits.IOC4 = 1;
    
    // Clearing External Interrupt Flag
    INTCONbits.RAIF = 0;
    
    // Setting Internal Oscillator Frequency to 8 MHz
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
}


void CheckStopTime (void)
{
    if (StopRelay1OnTime == 1 && Relay1StopTime[0] == Counter_10sec && Relay1StopTime[1] == Counter_1sec && Relay1StopTime[2] == Counter_100ms)
    {
        StopRelay1OnTime = 0;
        Set_Relay(Relay1, Off);
    }
    
    if (StopRelay2OnTime == 1 && Relay2StopTime[0] == Counter_10sec && Relay2StopTime[1] == Counter_1sec && Relay2StopTime[2] == Counter_100ms)
    {
        StopRelay2OnTime = 0;
        Set_Relay(Relay2, Off);
    }
    
    if (StopRelay3OnTime == 1 && Relay3StopTime[0] == Counter_10sec && Relay3StopTime[1] == Counter_1sec && Relay3StopTime[2] == Counter_100ms)
    {
        StopRelay3OnTime = 0;
        Set_Relay(Relay3, Off);
    }
    
    if (StopRelay4OnTime == 1 && Relay4StopTime[0] == Counter_10sec && Relay4StopTime[1] == Counter_1sec && Relay4StopTime[2] == Counter_100ms)
    {
        StopRelay4OnTime = 0;
        Set_Relay(Relay4, Off);
    }
    
    if (StopRelay5OnTime == 1 && Relay5StopTime[0] == Counter_10sec && Relay5StopTime[1] == Counter_1sec && Relay5StopTime[2] == Counter_100ms)
    {
        StopRelay5OnTime = 0;
        Set_Relay(Relay5, Off);
    }
    
    if (StopRelay6OnTime == 1 && Relay6StopTime[0] == Counter_10sec && Relay6StopTime[1] == Counter_1sec && Relay6StopTime[2] == Counter_100ms)
    {
        StopRelay6OnTime = 0;
        Set_Relay(Relay6, Off);
    }
}

void SetStopTime(unsigned char *RelayStopTime, unsigned char Time10sec, unsigned char Time1sec, unsigned char Time100ms)
{
    if (Counter_100ms + Time100ms >= 10)
    {
        *(RelayStopTime + 2) = Counter_100ms + Time100ms - 10;
        //*(RelayStopTime + 1) = *(RelayStopTime + 1) + 1;
        Time1sec = Time1sec + 1;
    }
    else
    {
        *(RelayStopTime + 2) = Counter_100ms + Time100ms;
    }
    
    if (Counter_1sec + Time1sec >= 10)
    {
        *(RelayStopTime + 1) = Counter_1sec + Time1sec - 10;
        //*(RelayStopTime + 0) = *(RelayStopTime + 0) + 1;
        Time10sec = Time10sec + 1;
    }
    else
    {
        *(RelayStopTime + 1) = Counter_1sec + Time1sec;
    }
    
    if (Counter_10sec + Time10sec >= 10)
    {
        *(RelayStopTime + 0) = Counter_10sec + Time10sec - 10;
    }
    else
    {
        *(RelayStopTime + 0) = Counter_10sec + Time10sec;
    }
    
    /*
    TelegramType ThisTelegram;
    ThisTelegram.Start = ':';
    ThisTelegram.Address = '0';
    ThisTelegram.Commandlength = 10;
    ThisTelegram.Command[0] = 'R';
    ThisTelegram.Command[1] = 'e';
    ThisTelegram.Command[2] = 'l';
    ThisTelegram.Command[3] = 'a';
    ThisTelegram.Command[4] = 'y';
    ThisTelegram.Command[5] = '1';
    ThisTelegram.Command[6] = '=';
    ThisTelegram.Command[7] = *(RelayStopTime + 0);
    ThisTelegram.Command[8] = *(RelayStopTime + 1);
    ThisTelegram.Command[9] = *(RelayStopTime + 2);
    ThisTelegram.End[0] = 0x0D;
    ThisTelegram.End[1] = 0x0A;
    
    SendTelegram(ThisTelegram);*/

}



void Handle_Telegram (TelegramType ThisTelegram)
{
    unsigned char state = 255;
    unsigned char error = 0;
    
    if (ThisTelegram.Command[0] == 'A')
    {
        state = On;      
    }
    else if (ThisTelegram.Command[0] == 'B')
    {
        state = Off;    
    }
    else if (ThisTelegram.Command[0] == 'S')
    {
        ThisTelegram.Commandlength = 7;
        ThisTelegram.Command[0] = 'S';
        ThisTelegram.Command[1] = (Read_Relay(Relay1) + 48);
        ThisTelegram.Command[2] = (Read_Relay(Relay2) + 48);
        ThisTelegram.Command[3] = (Read_Relay(Relay3) + 48);
        ThisTelegram.Command[4] = (Read_Relay(Relay4) + 48);
        ThisTelegram.Command[5] = (Read_Relay(Relay5) + 48);
        ThisTelegram.Command[6] = (Read_Relay(Relay6) + 48);

        error = 1;
    }
    else
    {
        error = 4;
    }
    
    SendTelegram(ThisTelegram);

    if (error == 0)
    { 
        if (state == On && (ThisTelegram.Command[1] !='0' || ThisTelegram.Command[2] !='0' || ThisTelegram.Command[3] !='0'))
        {
            switch (ThisTelegram.Address)
            {
                case '1':
                    Set_Relay(Relay1, state);
                    StopRelay1OnTime = 1;
                    SetStopTime(Relay1StopTime, ThisTelegram.Command[1] - 48, ThisTelegram.Command[2] - 48, ThisTelegram.Command[3] - 48);
                    break;

                case '2':
                    Set_Relay(Relay2, state); 
                    StopRelay2OnTime = 1;
                    SetStopTime(Relay2StopTime, ThisTelegram.Command[1] - 48, ThisTelegram.Command[2] - 48, ThisTelegram.Command[3] - 48);
                    break;

                case '3':
                    Set_Relay(Relay3, state);
                    StopRelay3OnTime = 1;
                    SetStopTime(Relay3StopTime, ThisTelegram.Command[1] - 48, ThisTelegram.Command[2] - 48, ThisTelegram.Command[3] - 48);
                    break;

                case '4':
                    Set_Relay(Relay4, state);
                    StopRelay4OnTime = 1;
                    SetStopTime(Relay4StopTime, ThisTelegram.Command[1] - 48, ThisTelegram.Command[2] - 48, ThisTelegram.Command[3] - 48);
                    break;

                case '5':
                    Set_Relay(Relay5, state);
                    StopRelay5OnTime = 1;
                    SetStopTime(Relay5StopTime, ThisTelegram.Command[1] - 48, ThisTelegram.Command[2] - 48, ThisTelegram.Command[3] - 48);
                    break;

                case '6':
                    Set_Relay(Relay6, state);
                    StopRelay6OnTime = 1;
                    SetStopTime(Relay6StopTime, ThisTelegram.Command[1] - 48, ThisTelegram.Command[2] - 48, ThisTelegram.Command[3] - 48);
                    break;                                         
            }
        }
        else
        {
            switch (ThisTelegram.Address)
            {
                case '1':
                    Set_Relay(Relay1, state);
                    StopRelay1OnTime = 0;
                    //SetStopTime(3, 5, 5);
                    break;

                case '2':
                    Set_Relay(Relay2, state); 
                    StopRelay2OnTime = 0;
                    break;

                case '3':
                    Set_Relay(Relay3, state);
                    StopRelay3OnTime = 0;
                    break;

                case '4':
                    Set_Relay(Relay4, state);
                    StopRelay4OnTime = 0;
                    break;

                case '5':
                    Set_Relay(Relay5, state);
                    StopRelay5OnTime = 0;
                    break;

                case '6':
                    Set_Relay(Relay6, state);
                    StopRelay6OnTime = 0;
                    break;                                         

            }
               
        }
    }
}

void HandleConfigByte (void)
{
    static unsigned char ByteCounter = 0;
    
    unsigned char Byte;
    Byte = Read_Byte();
    
    if (Byte == '>')
    {
        switch (ByteCounter)
        {
            case 0:
                ByteCounter = ByteCounter + 1;
                
                // Setting RA5 High 
                // Since the TinyMesh Module is now in ConfigMode
                PORTAbits.RA5 = 1;  
                TRISAbits.TRISA5 = 1;
                
                // Starting Memory Write
                AddByteToBuffer('M');
                break;
                
            case 1:
                ByteCounter = ByteCounter + 1;
                
                // Setting System ID0 to 1
                AddByteToBuffer(0x31);
                AddByteToBuffer(0x01);
                
                // Setting System ID1 to 0
                AddByteToBuffer(0x32);
                AddByteToBuffer(0x00);
                
                // Setting System ID2 to 0
                AddByteToBuffer(0x33);
                AddByteToBuffer(0x00);
                
                // Setting System ID3 to 0
                AddByteToBuffer(0x34);
                AddByteToBuffer(0x00);
                
                // Ending Memory Write
                AddByteToBuffer(0xFF);
                break;
                
            case 2:               
                // Clearing Counter 
                ByteCounter = 0;
                
                // Exit from ConfigMode
                AddByteToBuffer('X');
                                
                Set_Relay(Relay1, Off);
                Set_Relay(Relay2, Off);
                Set_Relay(Relay3, Off);
                Set_Relay(Relay4, Off);
                Set_Relay(Relay5, Off);
                Set_Relay(Relay6, Off);
                
                CurrentMode = Running;
                      
                // Enabling External Interrupt
                INTCONbits.RAIE = 1;
                           
        }
        
    }

}

void __interrupt () Handle_Interrupts (void)
{    
    if (PIR1bits.TXIF)
    {
        WriteByteFromBuffer();
    }    
        
    if (PIR1bits.RCIF)
    {
        switch (CurrentMode)
        {
            case Running:
                HandleTelegramByte();
                break;
                
            case Config:
                HandleConfigByte();
                break;               
        }

    }
    
    if (INTCONbits.RAIF)
    {      
        // Disabling External Interrupt
        INTCONbits.RAIE = 0;
        unsigned char dummy = PORTA;
        INTCONbits.RAIF = 0;    

        // Entering ConfigMode
        CurrentMode = Config;
        
        // Setting RA5 as Output
        TRISAbits.TRISA5 = 0;
        // Pulling RA5 Low to enter ConfigMode
        PORTAbits.RA5 = 0;       

    }

    if (PIR1bits.TMR1IF)
    {
        PIR1bits.TMR1IF = 0;
        Timer_Interrupt();

    }
}
/*PUBLIC FUNCTIONS*/

// ***************************************** Pit42 2014 © ******************************************