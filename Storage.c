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
// * File Name:         Storage.c
// * Created By:        Bendixen
// * Date Created:      April 14, 2014, 4:50 PM
// * Descsription:      
// *************************************************************************************************

/*INCLUDE FILES*/
#include "Include.h"

/*LOCAL DEFINITIONS*/

/*PRIVATE FUNCTION PROTOTYPES*/

/*PRIVATE FUNCTIONS*/

/*PUBLIC FUNCTIONS*/

void Write_Memory (unsigned char Address, unsigned char Value)
{
    // Saves whether Global Interrupt is enabled or not
    unsigned GIE_Temp_State = INTCONbits.GIE;

    // Disable Interrupts
    INTCONbits.GIE = 0;

    // Initiating chosen address
    EEADR = Address;

    // Initiating chosen value
    EEDATA = Value;

    // Point to DATA memory
    EECON1bits.EEPGD = 0;

    // Enable writes
    EECON1bits.WREN = 1;

    // Write 55h
    EECON2 = 0x55;

    // Write AAh
    EECON2 = 0x0AA;

    // Set WR bit to begin write
    EECON1bits.WR = 1;

    // Wait for the write to complete
    while (EECON1bits.WR)
    {
        ;
    }

    // Disables writes
    EECON1bits.WREN = 0;

    // Set Global Interrupt to the initial state
    INTCONbits.GIE = GIE_Temp_State;
}

unsigned char Read_Memory (unsigned char Address)
{
    // Saves whether Global Interrupt is enabled or not
    unsigned GIE_Temp_State = INTCONbits.GIE;

    // Disable Interrupts
    INTCONbits.GIE = 0;

    // Initiating chosen address
    EEADR = Address;

    // Point to data memory
    EECON1bits.EEPGD = 0;

    // Read EEPROM
    EECON1bits.RD = 1;

    // Wait for the read to complete
    while (EECON1bits.RD)
    {
        ;
    }

    // Set Global Interrupt to the initial state
    INTCONbits.GIE = GIE_Temp_State;

    return EEDATA;
}

// ***************************************** Pit42 2014 © ******************************************