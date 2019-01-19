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
// * File Name:         Timer.c
// * Created By:        Bendixen
// * Date Created:      April 14, 2014, 5:58 PM
// * Descsription:      
// *************************************************************************************************

/*INCLUDE FILES*/
#include "Include.h"

/*LOCAL DEFINITIONS*/

/*PRIVATE FUNCTION PROTOTYPES*/
void Timer_Interrupt(void);

/*PRIVATE FUNCTIONS*/

/*PUBLIC FUNCTIONS*/
void Init_Timer ()
{
    // Setting prescaler rate to 1:8
    T1CONbits.T1CKPS = 3;
    
    // Disabling LP oscillaotr for Timer1
    T1CONbits.T1OSCEN = 0;

    // Setting clock source to internal
    T1CONbits.TMR1CS = 0;

    // Enabling global interrupt
    INTCONbits.GIE = 1;

    // Enabling peripheral interrupts
    INTCONbits.PEIE = 1;

    // Enabling Timer1 interrupts
    PIE1bits.TMR1IE = 1;

}

void Start_Timer (void)
{
    // Clearing Timer1 interrupt flag
    PIR1bits.TMR1IF = 0;

    // Clearing 100ms counter flag
    Counter_100ms = 0;
    
    TMR1H = 160;             // preset for timer1 MSB register
    TMR1L = 138;

    // Enables Timer1
    T1CONbits.TMR1ON = 1;
}

void Stop_Timer (void)
{
    // Disables Timer1
    T1CONbits.TMR1ON = 0;
}

void Timer_Interrupt (void)
{ 
    TMR1H = 160;             // preset for timer1 MSB register
    TMR1L = 138;
    Counter_100ms++;


    
    if (Counter_100ms == 10)
    {
        
        Counter_100ms = 0;
        Counter_1sec ++;
        
        if (Counter_1sec == 10)
        {
            
            Counter_1sec = 0;
            Counter_10sec ++;
            
            if (Counter_10sec == 10)
            {

                Counter_10sec = 0;
            }
            
        }
                  
    }
}

// ***************************************** Pit42 2014 © ******************************************