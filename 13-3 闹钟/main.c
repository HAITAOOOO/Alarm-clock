#include <REGX52.H>
#include "Delay.h"
#include "Timer0.h"
#include "Nixie.h"
#include "key.h"
#include "Buzzer.h"
#include "Timer1.h"

unsigned char TimeSelect[3][3]={{23,59,55},{0,0,10},{0,0,0}};//时间，闹钟时间，秒表初始化
unsigned char Date[]={22,12,31};//日期
unsigned char Mode=0,KeyNum,Flag=0,FlashFlag=0,Select=0,DateSelect=0,Tflag=0;
unsigned char a,b,c,d,e,f;

void main()
{
	Timer0Init();
	Timer1Init();
	while(1)
	{
		KeyNum=Key();
		//闹钟判断
		if(TimeSelect[0][0] == TimeSelect[1][0] && TimeSelect[0][1] == TimeSelect[1][1] &&TimeSelect[0][2] ==TimeSelect[1][2])
		{
			Buzzer_Time(300);
		}
		
		if(KeyNum==1)//模式选择
		{
			Mode++;
			Mode%=6;
		}
		if(Mode==0)//时间显示
		{
			Nixie(1,TimeSelect[0][0]/10);
			Nixie(2,TimeSelect[0][0]%10);
			Nixie(3,TimeSelect[0][1]/10);
			Nixie(4,TimeSelect[0][1]%10);
			Nixie(5,TimeSelect[0][2]/10);
			Nixie(6,TimeSelect[0][2]%10);
			Nixie(7,10);
			Nixie(8,10);	
		}
		if(Tflag==1){Date[2]++;Tflag=0;}//满一天加一
		if(Mode==1 || Mode==2)//调整显示的时间和闹钟时间
		{
			if(KeyNum==3)
			{
				TimeSelect[Mode-1][Select]++;
				TimeSelect[Mode-1][0]%=24;
				TimeSelect[Mode-1][1]%=60;
				TimeSelect[Mode-1][2]%=60;
			}
			if(KeyNum==4)
			{
				TimeSelect[Mode-1][Select]--;
				if(TimeSelect[Mode-1][0]==255){TimeSelect[Mode-1][0]=23;}
				if(TimeSelect[Mode-1][1]==255){TimeSelect[Mode-1][1]=59;}
				if(TimeSelect[Mode-1][2]==255){TimeSelect[Mode-1][2]=59;}
			}
			if(FlashFlag==1)//闪烁控制
			{
				Nixie(Select*2+1,12);
				Nixie(Select*2+2,12);
			}else{
				Nixie(Select*2+1,TimeSelect[Mode-1][Select]/10);
				Nixie(Select*2+2,TimeSelect[Mode-1][Select]%10);
			}
			if(KeyNum==2)
			{
				Select++;
				Select%=3;
			}
			//根据Selsct的值协调其他位的显示
			if(Select==0){e=1,f=2,a=3,b=4,c=5,d=6;}
			else if(Select==1){e=2,f=0,a=5,b=6,c=1,d=2;}
			else if(Select==2){e=0,f=1,a=1,b=2,c=3,d=4;}
			
			Nixie(a,TimeSelect[Mode-1][e]/10);
			Nixie(b,TimeSelect[Mode-1][e]%10);
			Nixie(c,TimeSelect[Mode-1][f]/10);
			Nixie(d,TimeSelect[Mode-1][f]%10);
			Nixie(7,10);
			Nixie(8,10);
		}
		if(Mode == 3)//秒表显示
		{
			Nixie(7,11);
			Nixie(8,11);
			Nixie(1,TimeSelect[Mode-1][0]/10);
			Nixie(2,TimeSelect[Mode-1][0]%10);
			Nixie(3,TimeSelect[Mode-1][1]/10);
			Nixie(4,TimeSelect[Mode-1][1]%10);
			Nixie(5,TimeSelect[Mode-1][2]/10);
			Nixie(6,TimeSelect[Mode-1][2]%10);
			if(KeyNum==3 && Flag==0)
			{
				TR1=1;
			}
			if(KeyNum==4)
			{
				TR1=0;	
				Flag=1;
			}
			if(KeyNum==3 && Flag== 1)
			{
				TimeSelect[Mode-1][0]=TimeSelect[Mode-1][1]=TimeSelect[Mode-1][2]=0;
				Flag=0;
			}
		}
		if(Mode==4)
		{
			Nixie(1, Date[0]/10);
			Nixie(2, Date[0]%10);
			Nixie(3, Date[1]/10);
			Nixie(4, Date[1]%10);
			Nixie(5, Date[2]/10);
			Nixie(6, Date[2]%10);
			Nixie(7,10);
			Nixie(8,10);	
		}
		if(Mode==5)//日期显示与修改
		{
			if(KeyNum==2)
			{
				DateSelect++;
				DateSelect%=3;
			}
			if(FlashFlag==1)//闪烁控制
			{
				Nixie(DateSelect*2+1,12);
				Nixie(DateSelect*2+2,12);
			}else{
				Nixie(DateSelect*2+1,Date[DateSelect]/10);
				Nixie(DateSelect*2+2,Date[DateSelect]%10);
			}
			if(DateSelect==0){e=1,f=2,a=3,b=4,c=5,d=6;}
			else if(DateSelect==1){e=2,f=0,a=5,b=6,c=1,d=2;}
			else if(DateSelect==2){e=0,f=1,a=1,b=2,c=3,d=4;}
			
			Nixie(a,Date[e]/10);
			Nixie(b,Date[e]%10);
			Nixie(c,Date[f]/10);
			Nixie(d,Date[f]%10);
			Nixie(7,10);
			Nixie(8,10);
			if(KeyNum==3)//按键3按下
			{
				Date[DateSelect]++;//时间设置位数值加1
			}
			if(KeyNum==4)//按键3按下
			{
				Date[DateSelect]--;//时间设置位数值减1				
			}
		}
		
		//日期判断越界
		if(Date[0]<0){Date[0]=99;}//年越界判断
		if(Date[1]<1){Date[1]=12;}//月越界判断
		if(Date[1]>12){Date[1]=1;Date[0]++;}//月越界判断
		if( Date[1]==1 || Date[1]==3 || Date[1]==5 || Date[1]==7 || 
			Date[1]==8 || Date[1]==10 || Date[1]==12)//日越界判断
		{
			if(Date[2]<1){Date[2]=31;}//大月
			if(Date[2]>31){Date[2]=1;Date[1]++;}
		}
		else if(Date[1]==4 || Date[1]==6 || Date[1]==9 || Date[1]==11)
		{
			if(Date[2]<1){Date[2]=30;}//小月
			if(Date[2]>30){Date[2]=1;Date[1]++;}
		}
		else if(Date[1]==2)
		{
			if(Date[0]%4==0)
			{
				if(Date[2]<1){Date[2]=29;}//闰年2月
				if(Date[2]>29){Date[2]=1;Date[1]++;}
			}
			else
			{
				if(Date[2]<1){Date[2]=28;}//平年2月
				if(Date[2]>28){Date[2]=1;Date[1]++;}
			}
		}	
	}
}

//时分秒运行
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count,FlashCount;
	TL0 = 0x18;
	TH0 = 0xfc; //1ms
	T0Count++;
	FlashCount++;
	if(FlashCount>=500)
	{
		FlashCount=0;
		FlashFlag=!FlashFlag;//闪烁取反
	}
	if(T0Count>=1000)
	{
		T0Count=0;
		TimeSelect[0][2]++;
		if(TimeSelect[0][2]>=60)
		{
			TimeSelect[0][2]=0;
			TimeSelect[0][1]++;
			if(TimeSelect[0][1]>=60)
			{
				TimeSelect[0][1]=0;
				TimeSelect[0][0]++;
				if(TimeSelect[0][0]>=24)
				{
					TimeSelect[0][0]=0;
					if(Mode != 5){Tflag=1;}	
				}
			}
		}
	}
}

//秒表
void Timer1_Routine() interrupt 3
{
	static unsigned int T1Count;//FlashCount
	TH1 = 0xd8; 
	TL1 = 0xef;//10ms
	T1Count++;
	if(T1Count>=1)
	{
		T1Count=0;
		TimeSelect[2][2]++;
		if(TimeSelect[2][2]>=100)
		{
			TimeSelect[2][2]=0;
			TimeSelect[2][1]++;
			if(TimeSelect[2][1]>=60)
			{
				TimeSelect[2][1]=0;
				TimeSelect[2][0]++;
				TimeSelect[2][0]%=60;
			}
		}
	}
}
