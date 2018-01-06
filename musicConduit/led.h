#ifndef _LED_

#define _LED_
#include "CH452CMD.H"
#include <intrins.h>

//数码管显示数字
int SEG[] = {0xbd, 0x18, 0xd5, 0xd9, 0x78, 0xe9, 0xed, 0x98, 0xfd, 0xf9};

//数码管选择
int DIG[] = {0x0800, 0x0900, 0x0a00, 0x0b00, 0x0c00, 0x0d00, 0x0e00, 0x0f00};
//键盘数字十六进制表示数组
int KEY[] = {0x00, 0x04, 0x05, 0x06, 0x07, 0x0C, 0x0D, 0x0E, 0x0F};
//ch452 管脚定义
sbit CH452_DCLK = P2 ^ 4;
sbit CH452_DIN  = P2 ^ 5;
sbit CH452_LOAD = P2 ^ 6;
sbit CH452_DOUT = P2 ^ 7;
//串口位操作
#define CH452_DCLK_SET {CH452_DCLK=1;}
#define CH452_DCLK_CLR {CH452_DCLK=0;}
#define CH452_DIN_SET  {CH452_DIN=1;}
#define CH452_DIN_CLR  {CH452_DIN=0;}
#define CH452_LOAD_SET {CH452_LOAD=1;}
#define CH452_LOAD_CLR {CH452_LOAD=0;}
#define DELAY_1US {_nop_();_nop_();} // MCS51<=30MHz 

/**
 * 数码管显示数据
 * @param  [unsigned short char] cmd [写入十六进制数]
 * @return [type]            [description]
 */
void CH452_Write(unsigned short cmd)
{
	unsigned char i;
	CH452_LOAD_CLR;
	for (i = 0; i != 12; i++) {
		CH452_DCLK_CLR;
		CH452_DIN = cmd & 1;
		CH452_DCLK_SET;
		cmd >>= 1;
	}
	CH452_LOAD_SET;
	DELAY_1US;
	DELAY_1US;
	DELAY_1US;
}
/**
 * 键盘数据读取
 * @return [unsigned char] [返回键盘数据1~8]
 */
unsigned char CH452_Read(void)
{
	unsigned char i, keyw = 0, cmd = 0x07;

	if (CH452_DOUT == 0) {
		EX0 = 0;
		CH452_LOAD_CLR;
		for (i = 0; i < 4; i++) {
			CH452_DIN = cmd & 1;
			CH452_DCLK_CLR;
			cmd >>= 1;
			CH452_DCLK_SET;
		}
		CH452_LOAD_SET;
		for (i = 0; i < 7; i++) {
			keyw <<= 1;
			keyw |= CH452_DOUT;
			CH452_DCLK_CLR;
			CH452_DCLK_SET;
		}
		//flag=1;
		EX0 = 1;
		IE0 = 0;
		switch (keyw & 0x3f) {
			case 0X04:
				return 1;
			case 0X05:
				return 2;
			case 0X06:
				return 3;
			case 0X07:
				return 4;
			case 0X0C:
				return 5;
			case 0X0D:
				return 6;
			case 0X0E:
				return 7;
			case 0X0F:
				return 8;
		}
	}
}
/**
 * 数码管显示数据
 * @param  [int] X [数码管选择]
 * @param  [int] Y [数码管显示数字]
 */
void display(int X, int Y)
{
	CH452_Write(DIG[X] | SEG[Y]);
}
void initLed()
{
	CH452_Write(CH452_RESET);
	CH452_Write(CH452_NO_BCD);
	CH452_Write(CH452_SYSON2);
}
#endif