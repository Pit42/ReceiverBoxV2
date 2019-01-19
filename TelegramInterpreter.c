#include "Include.h"

void HandleTelegramByte (void)
{
    unsigned char Byte;
    static unsigned char ByteCounter = Idle;
    static unsigned char CommandCounter = 0;
    static unsigned char Lenght;

    Byte = Read_Byte();

    if (Byte == ':')
    {
        Telegram.Start = Byte;
        ByteCounter = Address;       
    }
    else if (ByteCounter > Start)
    {
        switch (ByteCounter)
        {
            case Address:
                Telegram.Address = Byte;
                ByteCounter = Commandlength;
                break;

            case Commandlength:
                Telegram.Commandlength = Byte - 48;
                ByteCounter = Command;
                CommandCounter = 0;
                break;

            case Command:
                Telegram.Command[CommandCounter] = Byte;
                CommandCounter ++;
                if (CommandCounter >= Telegram.Commandlength)
                {
                    ByteCounter = End1;
                }          
                break;

            case End1:
                Telegram.End[0] = Byte;
                ByteCounter = End2;
                break;

            case End2:
                Telegram.End[1] = Byte;
                ByteCounter = Start;
                //TelegramQueue[0]=Telegram;
                TelegramReadyFlag = 1;
                break;
        }
    }
}

void SendTelegram(TelegramType MyTelegram)
{         

    
    unsigned char MacCounter;
    unsigned char CommandCounter;
    
    AddByteToBuffer(MyTelegram.Start);
    
    AddByteToBuffer(MyTelegram.Address);
    AddByteToBuffer(MyTelegram.Commandlength + 48);
    //Write_Byte(MyTelegram.Command[0]);
    
    for (CommandCounter = 0; CommandCounter <  MyTelegram.Commandlength; CommandCounter ++)
    {
        AddByteToBuffer(MyTelegram.Command[CommandCounter]);
    }

    AddByteToBuffer(MyTelegram.End[0]);
    AddByteToBuffer(MyTelegram.End[1]); 
       
}
