#include <REGX52.H>

void Timer1Init(void)
{
	TMOD &= 0x0F;		//设置定时器模式 高八位清零
	TMOD |= 0x10;		//设置定时器模式 低八位清零
	TL1 = 0xef;		//设置定时初值
	TH1 = 0xd8;		//设置定时初值
	TF1 = 0;		//清除TF0标志
	TR1 = 0;		//定时器0开始计时
	ET1=1;
	EA=1;
	PT1=0;
}