#ifndef	_TM1638_H
#define	_TM1638_H

#include	<avr/io.h>
#include	<util/delay.h>
#include	<avr/pgmspace.h>

#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0

#define SET_BIT(PORT,BIT)	PORT|=(1<<BIT)
#define	CLR_BIT(PORT,BIT)	PORT&=~(1<<BIT)
#define	BIT_IN(DDR,BIT)		DDR&=~(1<<BIT)
#define	BIT_OUT(DDR,BIT)	DDR|=(1<<BIT)
#define	READ_BIT(PIN,BIT)	(PIN&(1<<BIT))

#define	DIO	PD7
#define	CLK	PD6
#define	STB	PD4

#define DIO_high		SET_BIT(PORTD,DIO)
#define DIO_low			CLR_BIT(PORTD,DIO)
#define CLK_high		SET_BIT(PORTD,CLK)
#define CLK_low			CLR_BIT(PORTD,CLK)
#define STB_high		SET_BIT(PORTD,STB)
#define STB_low			CLR_BIT(PORTD,STB)

#define DIO_IN		BIT_IN(DDRD,DIO)	//输入状态
#define DIO_OUT		BIT_OUT(DDRD,DIO)	//输出状态
#define	DIO_READ	READ_BIT(PIND,DIO)	//读引脚电平

/*
#define DIO_IN	DDRD&=~(1<<PD7)		//输入状态
#define DIO_OUT	DDRD|=(1<<PD7)		//输出状态
#define	DIO_READ	PIND&(1<<PD7)	//读引脚电平

//引脚定义
#define DIO_high		PORTD|=(1<<PD7)
#define DIO_low			PORTD&=~(1<<PD7)
#define CLK_high		PORTD|=(1<<PD6)
#define CLK_low			PORTD&=~(1<<PD6)
#define STB_high		PORTD|=(1<<PD4)
#define STB_low			PORTD&=~(1<<PD4)
*/
unsigned char tab[] PROGMEM={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
void TM1638_Write(unsigned char	DATA)			//写数据函数
{
	unsigned char i;
	DIO_OUT;
	for(i=0;i<8;i++)
	{
		CLK_low;
		if(DATA&0X01)
			DIO_high;
		else
			DIO_low;
		DATA>>=1;
		CLK_high;
		_delay_us(1);
	}
}
unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char temp=0;
	DIO_IN;
	_delay_us(1);
	for(i=0;i<8;i++)
	{
		temp>>=1;
		CLK_low;
		_delay_us(1);
		if(DIO_READ)
			temp|=0x80;
		CLK_high;
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//发送命令字
{
	STB_low;
	TM1638_Write(cmd);
	STB_high;
	_delay_us(1);
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	STB_low;
	TM1638_Write(0x42);
	_delay_us(1);
	for(i=0;i<4;i++)
		c[i]=TM1638_Read();
	STB_high;					//4个字节数据合成一个字节
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;
}
void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	Write_COM(0x44);
	STB_low;
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	STB_high;
}
void Write_ALLDATA(unsigned char add,unsigned char *p,unsigned char cnt)
{
	unsigned char i;
	Write_COM(0x44);
	for(i=0;i<cnt;i++)
		{
			Write_DATA(2*i,*(p+i));
		}
}
void Write_oneLED(unsigned char num,unsigned char flag)	//单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
	if(flag)
		Write_DATA(2*num+1,1);
	else
		Write_DATA(2*num+1,0);
}
void Write_allLED(unsigned char LED_flag)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			if(LED_flag&(1<<i))
				Write_DATA(2*i+1,3);
			else
				Write_DATA(2*i+1,0);
		}
}
void init_TM1638(void)
{
	unsigned char i;
	Write_COM(0x8b);//亮度
	Write_COM(0x40);
	STB_low;
	TM1638_Write(0xc0);
	for(i=0;i<16;i++)
		TM1638_Write(0x00);
	STB_high;
}
#endif
