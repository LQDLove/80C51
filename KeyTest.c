#include <reg52.h>

sbit KEY_ADD = P3 ^ 3;
sbit KEY_DEC = P3 ^ 4;

#define DataPort P0
sbit LATCH1 = P2 ^ 2;
sbit LATCH2 = P2 ^ 3;

unsigned char code dofly_DuanMa[16] = { //显示段码值0-F
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

unsigned char code dofly_WeiMa[8] = { //分别对应相应的数码管点亮，即位码
    0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

unsigned char TempData[8];
void DelayUs2x(unsigned char t);
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit, unsigned char Num);
void Init_Timer0(void);

void main()
{
    unsigned char num = 0, key_press_num;
    KEY_ADD = 1;
    KEY_DEC = 1;
    Init_Timer0();
    while (1)
    {
        if (!KEY_ADD)
        {
            DelayMs(10);
            if (!KEY_ADD)
            {
                while (!KEY_ADD)
                {
                    key_press_num++;
                    DelayMs(10);
                    if (key_press_num == 200)
                    {
                        key_press_numb = 0;
                        while (!KEY_ADD)
                        {
                            if (num < 99)
                                num++;
                            TempData[0] = dofly_DuanMa[num / 10];
                            TempData[1] = dofly_DuanMa[num % 10];
                            DelayMs(50);
                        }
                    }
                }
                key_press_num = 0;
                if (num < 99)
                    num++;
            }
        }

        if (!KEY_DEC)
        {
            DelayMs(10);
            if (!KEY_DEC)
            {
                while (!KEY_DEC)
                {
                    key_press_num++;
                    DelayMs(10);
                    if (key_press_num == 200)
                    {
                        key_press_num = 0;
                        while (!KEY_DEC)
                        {
                            if (num > 0)
                            {
                                num--;
                            }
                            TempData[0] = dofly_DuanMa[num / 10];
                            TempData[1] = dofly_DuanMa[num % 10];
                            DelayMs(50);
                        }
                    }
                }
                key_press_num = 0;
                if (num > 0)
                    num--;
            }
        }
        TempData[0] = dofly_DuanMa[num / 10];
        TempData[1] = dofly_DuanMa[num % 10];
    }
}

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

void Display(unsigned char FirstBit, unsigned char Num)
{
    static unsigned char i = 0;
    DataPort = 0;
    LATCH1 = 1;
    LATCH1 = 0;

    DataPort = dofly_WeiMa[i + FirstBit];
    LATCH2 = 1;
    LATCH2 = 0;

    DataPort = TempData[i];
    LATCH1 = 1;
    LATCH1 = 0;

    i++;
    if (i == Num)
        i = 0;
}

void Init_Timer0(void)
{
    TMOD |= 0x01; //使用模式1，16位定时器，使用“|”符号可以在使用多个定时器是不受影响
    EA = 1;       //总中断打开
    ET0 = 1;      //定时器中断打开
    TR0 = 1;      //定时器开关打开
}

void Timer0_isr(void) interrupt 1
{
    TH0 = (65536 - 2000) / 256; //重新赋值2ms
    TL0 = (65536 - 2000) % 256;

    Display(0, 8); //调用数码管扫描
}
