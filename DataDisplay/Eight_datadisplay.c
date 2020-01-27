#include<reg52.h>						//包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义

#define DataPort P0 						//DataPort替换命名									
sbit LATCH1 = P2^6;						//单片机端口P2.6
sbit LATCH2 = P2^7;

unsigned char code dofly_DuanMa[10] = {                     	//数码管显示数字0-9
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
	0x7f, 0x6f
};


unsigned char code dofly_WeiMa[] = {				//数码管的位，表示在哪个位置数字
  	0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f
};

void Delay(unsigned int t);
unsigned int i = 0;



void main(void)
{
	while(1)
	{
		LATCH1   = 1;				//位码
		DataPort = 0xfe;
		LATCH1   = 0;
		if(i==10)
		     i=0;
		LATCH2   = 1;				//段码
		DataPort = dofly_DuanMa[i++];
		LATCH2   = 0;
		
		Delay(500000);
	}
}

void Delay(unsigned int t){
	while(--t)
	     ;
}
