#include <reg52.h>

sbit LED = P1 ^ 0;
void DelayUs2x(unsigned char t);
void DelayMs(unsigned char t);
void main()
{
    LED = 0; //LED灯点亮
    EA = 1;  //全局中断开
    EX1 = 1; //外部中断1开
    IT1 = 1; //IT1=1表示边缘触发
    while (1)
    {
        //在此处添加其他程序
    }
}

//外部中断程序
void ISR_INT1(void) interrupt 2
{
    if (!INT1)
    {
        DelayMs(10);
        if (!INT1)
        //while(!INT1);//等待按键释放
        {
            LED = !LED;
        }
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
