
/*INCLUDE GUARD*/
#ifndef TELEGRAMINTERPRETER_H
#define	TELEGRAMINTERPRETER_H

/*INCLUDE FILES*/

/*PUBLIC DEFINITIONS*/
unsigned TelegramReadyFlag = 0;

typedef struct
{
    unsigned char Start; 
    unsigned char Address; // The address of the Input/Output that should be controlled (a-zA-Z0-9)
    unsigned char Commandlength; // Length in Bytes of the following command (128 - 255)
    unsigned char Command[16]; // The Command (a-zA-Z0-9)
    unsigned char End[2]; // CR LF / 0D 0A
} TelegramType;

TelegramType Telegram;

enum TelegramBytes
{
    Idle,
    Start,
    Address,
    Commandlength,
    Command,
    End1,
    End2,
};

/*PUBLIC FUNCTION PROTOTYPES*/
void HandleTelegramByte (void);
void SendTelegram(TelegramType MyTelegram);

#endif	/* TELEGRAMINTERPRETER_H */

