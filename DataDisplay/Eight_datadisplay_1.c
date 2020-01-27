#include<reg52.h>                                   //包含头文件，头文件包含特殊功能寄存器的定义

#define DataPort P0                                 //定义数据端口 程序中遇到DataPort 则用P0替换
sbit LATCH1 = P2^2;                                 //段锁存
sbit LATCH2 = P2^3;                                 //位锁存
  
unsigned char code dofly_DuanMa[10] = {             //显示段码值0-9
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
	0x7f, 0x6f
};


unsigned char code dofly_WeiMa[] = {                //分别对应相应的数码管点亮，即位码
  	0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f
};


unsigned char TempData[8];                          //存储显示值的全局变量
void Display(unsigned char FirstBit, unsigned char Num);  

//主函数
void main(void){
	unsigned char num;
	unsigned int j = 0;
	while(1){
		j++;
		if(j==500) {                                    //检测当前数值显示了一小段时间后，
                                                    //需要显示的数值加1，实现数据显示的变化
	  	j=0;
			num++;
			if(num==10)                                   //用于显示0-9
				num=0;
		}
		TempData[0]=dofly_DuanMa[num%10];               //分解显示信息
		Display(0,1);
	}
}

void Delay(unsigned int t){                         //延时函数
	while(--t);
}


//显示函数
//FirstBit表示需要显示的第一位，如果输入0表示从第一个显示。
//Num表示需要显示的位数，如果需要显示99两位数值则该值输入2
void Display(unsigned char FirstBit, unsigned char Num) {
 	unsigned char i;
	for(i=0;i<Num;i++){
		DataPort = 0;
		LATCH1 = 1;
		LATCH1 = 0;
		
		DataPort = dofly_WeiMa[i+FirstBit];
		LATCH2 = 1;
		LATCH2 = 0;
	
		DataPort = TempData[i];
		LATCH1 = 1;
		LATCH1 = 0;
		Delay(200);
	}	
}
