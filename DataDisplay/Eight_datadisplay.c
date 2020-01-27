#include<reg52.h>

#define DataPort P0
sbit LATCH1 = P2^6;
sbit LATCH2 = P2^7;

unsigned char code dofly_DuanMa[10] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 
	0x7f, 0x6f
};


unsigned char code dofly_WeiMa[] = {
  	0xfe, 0xfd, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f
};


unsigned char TempData[8];
void Display(unsigned char FirstBit, unsigned char Num);
void Delay(unsigned int t);
unsigned int i = 0;



void main(void){
	//unsigned char num;
	//unsigned int j = 0;
	//while(1){
	//	j++;
	//	if(j==500) {
	//	 	j=0;
	//		num++;
	//		if(num==10)
	//			num=0;
	//	}
	//	TempData[0]=dofly_DuanMa[num%10];
	//	Display(0,1);
	//}
	while(1){
		LATCH1 = 1;
		P0=0xfe;
		LATCH1 = 0;
		if(i==10)
			i=0;
		LATCH2 = 1;
		P0=dofly_DuanMa[i++];
		LATCH2 = 0;
		
		Delay(500000);
	}
}

void Delay(unsigned int t){
	while(--t);
}

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
		Delay(500000);
	}
}
