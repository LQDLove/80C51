#include <reg52.h>

#define DataPort P0
#define KeyPort P1
sbit LATCH1 = P2 ^ 2;
sbit LATCH2 = P2 ^ 3;

unsigned char code dofly_DuanMa[16] = { //显示段码值0-F
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

unsigned char code dofly_WeiMa[8] = { //分别对应相应的数码管点亮，即位码
    0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};

unsigned char TempData[10];
void DelayUs2x(unsigned char t);
void DelayMs(unsigned char t);
void Display(unsigned char FirstBit, unsigned char Num);
unsigned char KeyScan(void);
unsigned char KeyPro(void);
void Init_Timer0(void);

void main()
{
    unsigned char num, i, j;
    unsigned char temp[8];
    Init_Timer0();
    while (1)
    {
        num = KeyPro();
        if (num != 0xff)
        {
            if (i < 8)
            {
                temp[i] = dofly_DuanMa[num];
                for (j = 0; j <= i; j++)
                {
                    TempData[7 - i + j] = temp[j];
                }
            }
            i++;
            if (i == 9)
            {
                i = 0;
                for (j = 0; j < 8; j++)
                {
                    TempData[j] = 0;
                }
            }
        }
        Display(0, 8);
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

unsigned char KeyScan(void)
{
    unsigned char Val;
    KeyPort = 0xf0;
    if (KeyPort != 0xf0)
    {
        DelayMs(10);
        if (KeyPort != 0xf0)
        {
            KeyPort = 0xfe;
            if (KeyPort != 0xfe)
            {
                Val = KeyPort & 0xf0;
                Val += 0x0e;
                while (KeyPort != 0xfe)
                    ;
                DelayMs(10);
                while (KeyPort != 0xfe)
                    ;
                return Val;
            }

            KeyPort = 0xfd;
            if (KeyPort != 0xfd)
            {
                Val = KeyPort & 0xf0;
                Val += 0x0d;
                while (KeyPort != 0xfd)
                    ;
                DelayMs(10);
                while (KeyPort != 0xfd)
                    ;
                return Val;
            }

            KeyPort = 0xfb;
            if (KeyPort != 0xfb)
            {
                Val = KeyPort & 0xf0;
                Val += 0x0b;
                while (KeyPort != 0xfb)
                    ;
                DelayMs(10);
                while (KeyPort != 0xfb)
                    ;
                return Val;
            }

            KeyPort = 0xf7;
            if (KeyPort != 0xf7)
            {
                Val = KeyPort & 0xf0;
                Val += 0x07;
                while (KeyPort != 0xf7)
                    ;
                DelayMs(10);
                while (KeyPort != 0xf7)
                    ;
                return Val;
            }
        }
    }
    return 0xff;
}

unsigned char KeyPro(void)
{
    switch (KeyScan())
    {
    case 0x7e:
        return 0;
        break;
    case 0x7d:
        return 1;
        break;
    case 0x7b:
        return 2;
        break;
    case 0x77:
        return 3;
        break;
    case 0xbe:
        return 4;
        break;
    case 0xbd:
        return 5;
        break;
    case 0xbb:
        return 6;
        break;
    case 0xb7:
        return 7;
        break;
    case 0xde:
        return 8;
        break;
    case 0xdd:
        return 9;
        break;
    case 0xdb:
        return 10;
        break;
    case 0xd7:
        return 11;
        break;
    case 0xee:
        return 12;
        break;
    case 0xed:
        return 13;
        break;
    case 0xeb:
        return 14;
        break;
    case 0xe7:
        return 15;
        break;
    default:
        return 0xff;
        break;
    }
}
