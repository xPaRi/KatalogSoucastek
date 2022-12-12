#ifndef	_TM1638_H
#define	_TM1638_H

#include <REGX52.H>

#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0


sbit	DIO=P0^7;
sbit	CLK=P0^6;

//Ƭѡ��STB����ͬһ��IO�ڣ�˳������
//STB0~P05
//STB1~P04
//STB2~P03
//STB3~P02
//STB4~P01
//STB5~P00


unsigned char code tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
void TM1638_Write(unsigned char	DATA)			//д���ݺ���
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		CLK=0;
		if(DATA&0X01)
			DIO=1;
		else
			DIO=0;
		DATA>>=1;
		CLK=1;
	}
}
unsigned char TM1638_Read(void)					//�����ݺ���
{
	unsigned char i;
	unsigned char temp=0;
	DIO=1;
	for(i=0;i<8;i++)
	{
		temp>>=1;
		CLK=0;
		if(DIO)
			temp|=0x80;
		CLK=1;
	}
	return temp;
}
void Write_COM(unsigned char cmd,unsigned char STB_CS)		//����������
{
	P0&=~(0x20>>STB_CS);//STB=0;
	TM1638_Write(cmd);
	P0|=(0x20>>STB_CS);	//STB=1
}
unsigned char Read_key(unsigned char STB_CS)
{
	unsigned char c[4],i,key_value=0;
	P0&=~(0x20>>STB_CS);
	TM1638_Write(0x42);
	for(i=0;i<4;i++)
		c[i]=TM1638_Read();		//4���ֽ����ݺϳ�һ���ֽ�
	P0|=(0x20>>STB_CS);					//STB=1;
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
	P0&=~(0x20>>STB_CS);		//ѡ���Ӧ��ģ��
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	P0|=(0x20>>STB_CS);			//STB=1;
}
/*void Write_ALLDATA(unsigned char add,unsigned char *p,unsigned char cnt,unsigned char STB_CS)
{
	unsigned char i;
	Write_COM(0x44,STB_CS);
	for(i=0;i<cnt;i++)
		{
			Write_DATA(2*i,*(p+i),STB_CS);
		}
}	*/
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
	P0&=~(0x20>>STB_CS);
	TM1638_Write(0xc0);
	for(i=0;i<16;i++)
		TM1638_Write(0x00);
	P0|=(0x20>>STB_CS);
}
#endif
