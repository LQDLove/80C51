/**
*名称：遥控器红外解码数码管显示
*内容：8位数码管显示 用户码 用户码 数据码 数据反码，测得同一个案件的值不变，数据码+数据反码=EF表示解码正确， 使用12M晶振，应避免见光，日光灯干扰，因为接收头没有使用滤光片。
*/

#include<reg52.h>

sbit IR = P3^2;																				//红外接口标志	
#define DataPort P0
sbit LATCH1 = P2^2;
sbit LATCH2 = P2^3;

unsigned char code dofly_DuanMa[16] = {								//显示段码值0-F
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
	0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71
};


unsigned char code dofly_WeiMa[] = {									//分别对应相应的数码管点亮，即位码
  	0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f
};


unsigned char TempData[8];														//存储显示值的全局变量
unsigned char irtime;																	//红外用全局变量

bit irpro_ok,irok;

unsigned char IRcord[4];
unsigned char irdata[33];

void Ir_work(void);
void Ircordpro(void);

//显示函数
void Display(unsigned char FirstBit, unsigned char Num) 
{
 	static unsigned char i = 0;
	DataPort = 0;
	LATCH1 = 1;
	LATCH1 = 0;
	
	DataPort = dofly_WeiMa[i+FirstBit];
	LATCH2 = 1;
	LATCH2 = 0;

	DataPort = TempData[i];
	LATCH1 = 1;
	LATCH1 = 0;

	i++;
	if(i==Num)
		i=0;
}

//定时器0中断处理
void tim0_isr(void) interrupt 1 using 1 
{
 	irtime++;																								//用于计数2个下降沿之间的时间，时间长度是256us@12M晶振
}

//定时器中断子程序
void Timer1_isr(void) interrupt 3 
{
	TH1 = (65536-2000)/256;																	//重新赋值2ms
	TL1 = (65536-2000)%256;

	Display(0,8);																						//调用数码管扫描
}

//外部中断0中断处理
void EX0_ISR(void) interrupt 0														//外部中断0服务函数 
{
 	static unsigned char i;																	//接受红外信号处理
	static char startflag = 0;															//是否开始处理标志位
	if(startflag) {
		if(irtime<63 && irtime>=33)														//引导码TC9012的头码，256*33=8448us,256*63=16128us
			i=0;																								//9ms+4.5ms或者4.5ms+4.5ms
		irdata[i]=irtime;																			//存储每个电平的持续时间，用于以后判断0还是1
		irtime=0;
		i++;
		if(i==33) {
			irok=1;
			i=0;
			startflag=0;
		}
	} else {																								//第一个下降沿进入中断执行此处
		irtime = 0;
		startflag = 1;
	}
}

//定时器0初始化
void TIM0init(void) {
	TMOD = 0x02;//定时器0工作方式2，TH0是重载值，TL0是初值
	TH0 = 0x00;//重载值
	TL0 = 0x00;//初始化值
	ET0 = 1;//开中断	
	TR0 = 1;
}

//定时器初始化子程序
void Init_Timer1(void) {
	TMOD |= 0x01;//使用模式1，16位定时器，使用“|”符号可以在使用多个定时器是不受影响
	EA = 1;//总中断打开
	ET1 = 1;//定时器中断打开
	TR1 = 1;//定时器开关打开
}

//外部中断0初始化
void EX0init(void) {
	IT0 =	1;//指定外部中断0下降沿触发，INTO（P3.2）
	EX0 =	1;//是能外部中断
	EA	=	1;//开总中断
}

//键值处理
void Ir_work(void) {
	TempData[0] = dofly_DuanMa[IRcord[0]/16];
	TempData[1] = dofly_DuanMa[IRcord[0]%16];
	TempData[2] = dofly_DuanMa[IRcord[1]/16];
	TempData[3] = dofly_DuanMa[IRcord[1]%16];
	TempData[4] = dofly_DuanMa[IRcord[2]/16];
	TempData[5] = dofly_DuanMa[IRcord[2]%16];
	TempData[6] = dofly_DuanMa[IRcord[3]/16];
	TempData[7] = dofly_DuanMa[IRcord[3]%16];
	irpro_ok = 0;
}



void Ircordpro(void) {
 	unsigned char i, j, k;
	unsigned char cord, value;

	k = 1;
	for(i=0;i<4;i++) {
		for(j=1;j<=8;j++) {
			cord=irdata[k];
			if(cord>7)
				value |= 0x80;
			if(j<8){
				value>>=1;
			}
			k++;
		}
		IRcord[i] = value;
		value = 0;
	}			  
	irpro_ok = 1;
}



void main(void){
	EX0init();
	TIM0init();
	Init_Timer1();
	while(1){
		if(irok){
			Ircordpro();
			irok=0;
		}
		if(irpro_ok)
			Ir_work();
	}
}
