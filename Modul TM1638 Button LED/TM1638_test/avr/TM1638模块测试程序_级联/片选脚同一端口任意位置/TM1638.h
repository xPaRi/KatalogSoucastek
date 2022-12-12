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
//#define STB_high		SET_BIT(PORTD,STB)
//#define STB_low			CLR_BIT(PORTD,STB)

#define DIO_IN		BIT_IN(DDRD,DIO)	//����״̬
#define DIO_OUT		BIT_OUT(DDRD,DIO)	//���״̬
#define	DIO_READ	READ_BIT(PIND,DIO)	//�����ŵ�ƽ
//Ƭѡ��STB����ͬһ��IO�ڣ�˳������
//STB0~PD4
//STB1~PD5
//STB2~PD2
//STB3~PD3
//STB4~PD0
//STB5~PD1


unsigned char tab[] PROGMEM={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
unsigned char STB_SEL[] PROGMEM={0xEF, 0xDF, 0XFB, 0XF7, 0XFE, 0XFD};
								// PD4=0 PD5=0 PD2=0 PD3=0 PD0=0 PD1=0
//STBƬѡ�ţ�Ҫ��Ƭѡ����ͬһ��PORT�ڣ����и���Ӳ���޸��������ݣ����STB����˳�����������ʹ�ø��򵥵ķ���
void TM1638_Write(unsigned char	DATA)			//д���ݺ���
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
unsigned char TM1638_Read(void)					//�����ݺ���
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
void Write_COM(unsigned char cmd,unsigned char STB_CS)		//����������
{
	PORTD&=pgm_read_byte(STB_SEL+STB_CS);//STB=0;
	TM1638_Write(cmd);
	PORTD|=~pgm_read_byte(STB_SEL+STB_CS);	//STB=1
	_delay_us(1);
}
unsigned char Read_key(unsigned char STB_CS)
{
	unsigned char c[4],i,key_value=0;
	PORTD&=pgm_read_byte(STB_SEL+STB_CS);
	TM1638_Write(0x42);
	_delay_us(1);
	for(i=0;i<4;i++)
		c[i]=TM1638_Read();		//4���ֽ����ݺϳ�һ���ֽ�
	PORTD|=~pgm_read_byte(STB_SEL+STB_CS);					//STB=1;
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;
}
void Write_DATA(unsigned char add,unsigned char DATA,unsigned char STB_CS)		//ָ����ַд������
{
	Write_COM(0x44,STB_CS);
	PORTD&=pgm_read_byte(STB_SEL+STB_CS);		//ѡ���Ӧ��ģ��
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	PORTD|=~pgm_read_byte(STB_SEL+STB_CS);			//STB=1;
}
void Write_ALLDATA(unsigned char add,unsigned char *p,unsigned char cnt,unsigned char STB_CS)
{
	unsigned char i;
	Write_COM(0x44,STB_CS);
	for(i=0;i<cnt;i++)
		{
			Write_DATA(2*i,*(p+i),STB_CS);
		}
}
void Write_oneLED(unsigned char num,unsigned char flag,unsigned char STB_CS)	//��������һ��LED������numΪ��Ҫ���Ƶ�led��ţ�flagΪ0ʱϨ�𣬲�Ϊ0ʱ����
{
	if(flag)
		Write_DATA(2*num+1,1,STB_CS);
	else
		Write_DATA(2*num+1,0,STB_CS);
}
void Write_allLED(unsigned char LED_flag,unsigned char STB_CS)					//����ȫ��LED������LED_flag��ʾ����LED״̬
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			if(LED_flag&(1<<i))
				Write_DATA(2*i+1,3,STB_CS);
			else
				Write_DATA(2*i+1,0,STB_CS);
		}
}
void init_TM1638(unsigned char STB_CS)
{
	unsigned char i;
	Write_COM(0x8b,STB_CS);//����
	Write_COM(0x40,STB_CS);
	PORTD&=pgm_read_byte(STB_SEL+STB_CS);;
	TM1638_Write(0xc0);
	for(i=0;i<16;i++)
		TM1638_Write(0x00);
	PORTD|=~pgm_read_byte(STB_SEL+STB_CS);
}
#endif
