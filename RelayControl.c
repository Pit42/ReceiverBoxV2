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
// * Project Name:      OGC Receiver
// * File Name:         RelayControl.c
// * Created By:        Bendixen
// * Date Created:      June 5, 2014, 3:23 PM
// * Descsription:      
// *************************************************************************************************

/*INCLUDE FILES*/
#include "Include.h"

/*LOCAL DEFINITIONS*/
#define Relay1_Tris TRISAbits.TRISA0
#define Relay1_Port PORTAbits.RA0

#define Relay2_Tris TRISAbits.TRISA1
#define Relay2_Port PORTAbits.RA1

#define Relay3_Tris TRISAbits.TRISA2
#define Relay3_Port PORTAbits.RA2

#define Relay4_Tris TRISCbits.TRISC0
#define Relay4_Port PORTCbits.RC0

#define Relay5_Tris TRISCbits.TRISC1
#define Relay5_Port PORTCbits.RC1

#define Relay6_Tris TRISCbits.TRISC2
#define Relay6_Port PORTCbits.RC2

/*PRIVATE FUNCTION PROTOTYPES*/

/*PRIVATE FUNCTIONS*/

/*PUBLIC FUNCTIONS*/
void Init_Relays (void)
{
    Relay1_Tris = 0;
    Relay1_Port = 0;
    Relay2_Tris = 0;
    Relay2_Port = 0;
    Relay3_Tris = 0;
    Relay3_Port = 0;
    Relay4_Tris = 0;
    Relay4_Port = 0;
    Relay5_Tris = 0;
    Relay5_Port = 0;
    Relay6_Tris = 0;
    Relay6_Port = 0;

}

void Set_Relay (unsigned char Relay, unsigned char State)
{
    switch (Relay)
    {
        case Relay1:
        if (State == On)
        {
            Relay1_Port = 1;
        }
        else if (State == Off)
        {
            Relay1_Port = 0;
        }
        else
        {
            // Error
        }
        break;

        case Relay2:
        if (State == On)
        {
            Relay2_Port = 1;
        }
        else if (State == Off)
        {
            Relay2_Port = 0;
        }
        else
        {
            // Error
        }
        break;

        case Relay3:
        if (State == On)
        {
            Relay3_Port = 1;
        }
        else if (State == Off)
        {
            Relay3_Port = 0;
        }
        else
        {
            // Error
        }
        break;

        case Relay4:
        if (State == On)
        {
            Relay4_Port = 1;
        }
        else if (State == Off)
        {
            Relay4_Port = 0;
        }
        else
        {
            // Error
        }
        break;

        case Relay5:
        if (State == On)
        {
            Relay5_Port = 1;
        }
        else if (State == Off)
        {
            Relay5_Port = 0;
        }
        else
        {
            // Error
        }
        break;

        case Relay6:
        if (State == On)
        {
            Relay6_Port = 1;
        }
        else if (State == Off)
        {
            Relay6_Port = 0;
        }
        else
        {
            // Error
        }
        break;
    }

}

unsigned char Read_Relay (unsigned char Relay)
    {
        switch (Relay)
        {
            case Relay1:
                return Relay1_Port;

            case Relay2:
                return Relay2_Port;

            case Relay3:
                return Relay3_Port;

            case Relay4:
                return Relay4_Port;

            case Relay5:
                return Relay5_Port;

            case Relay6:
                return Relay6_Port;
        }
        return(255);
    }

// ***************************************** Pit42 2014 © ******************************************
