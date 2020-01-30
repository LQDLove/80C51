#include <reg52.h>
#include <intrins.h>

unsigned char segout[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned char code tab[] = {
    0x00, 0x6c, 0x92, 0x82, 0x44, 0x28, 0x10, 0x00};

sbit LATCH = P1 ^ 0;
sbit SRCLK = P1 ^ 1;
sbit SER = P1 ^ 2;

sbit LATCH_B = P2 ^ 2;
sbit SRCLK_B = P2 ^ 1;
sbit SER_B = P2 ^ 0;

void DelayUs2x(unsigned char t)
{ //延时函数
    while (--t)
        ;
}

void DelayMs(unsigned char t)
{
    while (t--)
    {
        DelayUs2x(245);
        DelayUs2x(245);
    }
}

void SendByte(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        SRCLK = 0;
        SER = dat & 0x80;
        dat <<= 1;
        SRCLK = 1;
    }
}

void Send2Byte(unsigned char dat1, unsigned char dat2)
{
    SendByte(dat1);
    SendByte(dat2);
}

void Out595(void)
{
    _nop_();
    LATCH = 1;
    LATCH = 0;
}

void SendSeg(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        SRCLK_B = 0;
        SER_B = dat & 0x80;
        dat <<= 1;
        SRCLK_B = 1;
    }
    LATCH_B = 1;
    _nop_();
    LATCH_B = 0;
}

void main()
{
    while (1)
    {
        for (j = 0; j < 60; j++)
        {
            for (i = 0; i < 8; i++)
            {
                SendSeg(segout[i]);
                Send2Byte(~tab[i], 0xff);
                Out595();
                DelayMs(1);
                Send2Byte(0xff, oxff);
                Out595();
            }
        }

        for (j = 0; j < 60; j++)
        {
            for (i = 0; i < 8; i++)
            {
                SendSeg(segout[7 - i]);
                Send2Byte(0xff, ~tab[i]);
                Out595();
                DelayMs(1);
                Send2Byte(0xff, 0xff);
                Out595();
            }
        }
    }
}
