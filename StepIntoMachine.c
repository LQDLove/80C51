#include <reg52.h>

sbit A1 = P1 ^ 0;
sbit B1 = P1 ^ 1;
sbit C1 = P1 ^ 2;
sbit D1 = P1 ^ 3;

#define Coil_A1 \
    {           \
        A1 = 1; \
        B1 = 0; \
        C1 = 0; \
        D1 = 0; \
    }

#define Coil_B1 \
    {           \
        A1 = 0; \
        B1 = 1; \
        C1 = 0; \
        D1 = 0; \
    }

#define Coil_C1 \
    {           \
        A1 = 0; \
        B1 = 0; \
        C1 = 1; \
        D1 = 0; \
    }

#define Coil_D1 \
    {           \
        A1 = 0; \
        B1 = 0; \
        C1 = 0; \
        D1 = 1; \
    }

#define Coil_AB1 \
    {            \
        A1 = 1;  \
        B1 = 1;  \
        C1 = 0;  \
        D1 = 0;  \
    }

#define Coil_BC1 \
    {            \
        A1 = 0;  \
        B1 = 1;  \
        C1 = 1;  \
        D1 = 0;  \
    }

#define Coil_CD1 \
    {            \
        A1 = 0;  \
        B1 = 0;  \
        C1 = 1;  \
        D1 = 1;  \
    }

#define Coil_DA1 \
    {            \
        A1 = 1;  \
        B1 = 0;  \
        C1 = 0;  \
        D1 = 1;  \
    }

#define Coil_OFF \
    {            \
        A1 = 0;  \
        B1 = 0;  \
        C1 = 0;  \
        D1 = 0;  \
    }

unsigned char Speed;

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

void main()
{
    unsigned int i = 512;
    Speed = 8;
    Coil_OFF while (i--)
    {
        Coil_A1
            DelayMs(Speed);

        Coil_B1
            DelayMs(Speed);

        Coil_C1
            DelayMs(Speed);

        Coil_D1
            DelayMs(Speed);
    }
    Coil_OFF
        i = 512;
    while (i--)
    {
        Coil_D1
            DelayMs(Speed);

        Coil_C1
            DelayMs(Speed);

        Coil_B1
            DelayMs(Speed);

        Coil_A1
            DelayMs(Speed);
    }
}
