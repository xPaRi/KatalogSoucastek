#include	<avr/io.h>
//#include	<util/delay.h>
#include	<tm1638.h>

//#define led_off	PORTD|=(1<<PD3)
//#define	led_on	PORTD&=~(1<<PD3)

unsigned char num[8];		//各个数码管显示的值
void port_init(void)
{
	DDRD|=(1<<PD3)|(1<<DIO)|(1<<CLK)|(1<<STB);
	PORTD|=(1<<PD3)|(1<<DIO)|(1<<CLK)|(1<<STB);
}

unsigned char presence_flag(void)
{
	unsigned char c;
	STB_low;
	TM1638_Write(0x42);
	_delay_us(1);
	c=TM1638_Read();
	STB_high;			
	if((c&0x04))
		return	0;
	else
		return	1;
}
int main(void)
{
	unsigned char i;
	port_init();
	init_TM1638();
	for(i=0;i<8;i++)Write_DATA(i<<1,pgm_read_byte(tab));
//	led_on;
	while(1)
		{
/*			if(presence_flag()==0)
				{
					led_off;
					while(presence_flag()==0);
					_delay_ms(1000);
					while(presence_flag()==0);
					led_on;				
					init_TM1638();
					for(j=0;j<8;j++)
						{
							for(i=0;i<8;i++)
								Write_DATA(i<<1,0x01<<j);
							Write_allLED(0x01<<j);
							_delay_ms(400);
						}
					for(i=0;i<8;i++)
					{
						num[i]=0;
						Write_DATA(i<<1,pgm_read_byte(tab+num[i]));
					}
				}
			else
				led_on;*/
			i=Read_key();
			if(i<8)
				{
					num[i]++;
					while(Read_key()==i);		//等待按键释放
					if(num[i]>15)
						num[i]=0;
					Write_DATA(i*2,pgm_read_byte(tab+num[i]));
					Write_allLED(1<<i);
				}
		}
}

