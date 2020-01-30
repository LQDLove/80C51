#include<reg52.h>

sbit OUT=P1^2;

void Init_Timer0(void)
{
    TMOD |= 0x01; //使用模式1，16位定时器，使用“|”符号可以在使用多个定时器是不受影响
    EA = 1;       //总中断打开
    ET0 = 1;      //定时器中断打开
    TR0 = 1;      //定时器开关打开
}

void main(){
    Init_Timer0();
    while(1);
}

void Timer0_isr(void) interrupt 1 using 1
{
    TH0 = (65536 - 2000) / 256; //重新赋值2ms
    TL0 = (65536 - 2000) % 256;

    OUT=~OUT;
}
