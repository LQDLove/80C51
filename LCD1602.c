#include <reg52.h>
#include <intrins.h>

sbit RS = P2 ^ 4;
sbit RW = P2 ^ 5;
sbit EN = P2 ^ 6;

#define RS_CLR RS=0
#define RS_SET RS=1
#define RW_CLR RW=0
#define RW_SET RW=1
#define EN_CLR EN=0
#define EN_SET EN=1
#define DataPort P0
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

bit LCD_Chek_Busy(void)
{
    DataPort = 0xff;
    RS_CLR;
    RW_SET;
    EN_CLR;
    _nop_();
    EN_SET;
    return (bit)(DataPort & 0x80);
}

void LCD_Write_Com(unsigned char com)
{
    //while(LCD_Check_Busy());//忙则等待
    DelayMs(5);
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataPort = com;
    _nop_();
    EN_CLR;
}

void LCD_Write_Data(unsigned char Data){
    //while(LCD_Check_Busy());//忙则等待
    DelayMs(5);
    RS_SET;
    RW_CLR;
    EN_SET;
    DataPort = Data;
    _nop_();
    EN_CLR;
}

void LCD_Clear(void)
{
    LCD_Write_Com(0x01);
    DelayMs(5);
}

void LCD_Write_String(unsigned char x, unsigned char y, unsigned char *s)
{
    if (y == 0)
    {
        LCD_Write_Com(0x80 + x);
    }
    else
    {
        LCD_Write_Com(0xC0 + x);
    }
    while (*s)
    {
        LCD_Write_Data(*s);
        s++;
    }
}

void LCD_Write_Char(unsigned char x, unsigned char y, unsigned char Data)
{
    if (y == 0)
    {
        LCD_Write_Com(0x80 + x);
    }
    else
    {
        LCD_Write_Com(0xC0 + x);
    }
    LCD_Write_Data(Data);
}

void LCD_Init(void)
{
    LCD_Write_Com(0x38);
    DelayMs(5);
    LCD_Write_Com(0x38);
    DelayMs(5);
    LCD_Write_Com(0x38);
    DelayMs(5);
    LCD_Write_Com(0x38);
    LCD_Write_Com(0x08);
    LCD_Write_Com(0x01);
    LCD_Write_Com(0x06);
    DelayMs(5);
    LCD_Write_Com(0x0C);
}

void main(void)
{
    LCD_Init();
    LCD_Clear();
    while (1)
    {
        LCD_Write_Char(7, 0, 'o');
        LCD_Write_Char(8, 0, 'k');
        LCD_Write_String(1, 1, "www.dofly.net");
        while (1)
            ;
    }
}
