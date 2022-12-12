#include <REGX51.H>
#include	<tm1638.h>


unsigned char num[48];		//各个数码管显示的值，最多级联6个，6*8=48

unsigned char presence(void)		//检测存在的模块数量
{
	unsigned char c,i;
	for(i=6;i>0;i--)
	{
		P0&=STB_SEL[i];
		TM1638_Write(0x42);
		c=TM1638_Read();
		P0|=~STB_SEL[i];	//STB=1		
		if((c&0x04)==0)
			break;
	}
	return i+1;
}
int main(void)
{
	unsigned char i,j,STB_num,key_num;
	STB_num=presence();
	for(j=0;j<STB_num;j++)	//初始化显示内容为检测到的模块数量
		{
			init_TM1638(j);
			for(i=0;i<8;i++)
			Write_DATA(i<<1,tab[STB_num],j);
		}
	while(1)
		{
			for(j=0;j<STB_num;j++)
				{
					key_num=Read_key(j);
					if(key_num<8)
						{
							num[8*j+key_num]++;
							while(Read_key(j)==key_num);		//等待按键释放
							if(num[8*j+key_num]>15)
								num[8*j+key_num]=0;
							Write_DATA(key_num<<1,tab[num[8*j+key_num]],j);
							Write_allLED(1<<key_num,j);
						}
				}
		}
}

