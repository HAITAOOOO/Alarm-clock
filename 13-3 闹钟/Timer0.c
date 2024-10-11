#include <REGX52.H>

void Timer0Init(void)
{
	TMOD &= 0xF0;		//设置定时器模式 低八位清零
	TMOD |= 0x01;		//设置定时器模式 高八位清零
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
	PT0=0;
}
