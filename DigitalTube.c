#include <reg52.h>

unsigned char code dofly_table[10] = { //显示段码值0-9
    0xc0, 0xf9, 0xa4, 0xb0, 0x99,
    0x92, 0x82, 0xf8, 0x80, 0x90};

void main(void)
{
    unsigned char i;
    while (1)
    {
        for (i = 0; i < 10; i++)
        {
            P1 = dofly_table[i];
            Delay(60000);
        }
    }
}

void Delay(unsigned char t)
{ //延时函数
    while (--t)
        ;
}
