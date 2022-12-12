#include	<avr/io.h>
//#include	<util/delay.h>
#include	<tm1638.h>


unsigned char num[48];		//各个数码管显示的值，最多级联6个，6*8=48
void port_init(void)
{
	DDRD|=0xff;
	PORTD|=0xff;
}

unsigned char presence(void)		//检测存在的模块数量
{
	unsigned char c,i;
	for(i=6;i>0;i--)
	{
		PORTD&=~(0x20>>i);
		TM1638_Write(0x42);
		_delay_us(1);
		c=TM1638_Read();
		PORTD|=(0x20>>i);	//STB=1		
		if((c&0x04)==0)
			break;
	}
	return i+1;
}
int main(void)
{
	unsigned char i,j,STB_num,key_num;
	port_init();
	STB_num=presence();
	for(j=0;j<STB_num;j++)	//初始化显示内容为检测到的模块数量
		{
			init_TM1638(j);
			for(i=0;i<8;i++)
			Write_DATA(i<<1,pgm_read_byte(tab+STB_num),j);
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
							Write_DATA(key_num<<1,pgm_read_byte(tab+num[8*j+key_num]),j);
							Write_allLED(1<<key_num,j);
						}
				}
		}
}

