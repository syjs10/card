#include <reg51.h>
#include "CH452CMD.H"
//#include "STC15F2K60S2.H"
#include <intrins.h>

//数码管显示数字
int SEG[]={0xbd,0x18,0xd5,0xd9,0x78,0xe9,0xed,0x98,0xfd,0xf9};

//数码管选择
int DIG[]={0x0800,0x0900,0x0a00,0x0b00,0x0c00,0x0d00,0x0e00,0x0f00};
//键盘数字十六进制表示数组
int KEY[]={0x00,0x04,0x05, 0x06, 0x07, 0x0C, 0x0D, 0x0E, 0x0F};
//ch452 管脚定义
sbit CH452_DCLK = P2^4;
sbit CH452_DIN  = P2^5;
sbit CH452_LOAD = P2^6;
sbit CH452_DOUT = P2^7;

//串口位操作
#define CH452_DCLK_SET {CH452_DCLK=1;} 
#define CH452_DCLK_CLR {CH452_DCLK=0;} 
#define CH452_DIN_SET  {CH452_DIN=1;} 
#define CH452_DIN_CLR  {CH452_DIN=0;} 
#define CH452_LOAD_SET {CH452_LOAD=1;} 
#define CH452_LOAD_CLR {CH452_LOAD=0;} 

//延时
//#define DELAY_1US {} // MCS51<=10MHz 
//#define DELAY_1US {_nop_();} // MCS51<=20MHz 
#define DELAY_1US {_nop_();_nop_();} // MCS51<=30MHz 


// instruction compensate count
#define COMP_COUNT        8

// define music note timer count

#define G3 half_cycle_time/196  - COMP_COUNT 
#define A3 half_cycle_time/220  - COMP_COUNT
#define B3 half_cycle_time/247  - COMP_COUNT
#define C4 half_cycle_time/262  - COMP_COUNT    // middle c
#define D4 half_cycle_time/294  - COMP_COUNT
#define E4 half_cycle_time/330  - COMP_COUNT
#define F4 half_cycle_time/365  - COMP_COUNT
#define G4 half_cycle_time/392  - COMP_COUNT
#define A4 half_cycle_time/440  - COMP_COUNT
#define B4 half_cycle_time/494  - COMP_COUNT
#define C5 half_cycle_time/523  - COMP_COUNT
#define D5 half_cycle_time/587  - COMP_COUNT
#define E5 half_cycle_time/659  - COMP_COUNT
#define F5 half_cycle_time/750  - COMP_COUNT
#define G5 half_cycle_time/784  - COMP_COUNT
#define A5 half_cycle_time/880  - COMP_COUNT
        

unsigned int ToneCountTable[16]={
     G3    ,A3    ,B3    ,C4    
    ,D4    ,E4    ,F4    ,G4    
    ,A4    ,B4    ,C5    ,D5
    ,E5    ,F5    ,G5    ,A5
};

// hardware configuration
sbit sound = P4^5;

#define  DEBOUNCE_COUNT            5


// timer 0
unsigned int timer0_count;
unsigned char count_high_byte = 0;
unsigned char count_low_byte = 0;

// crystal=11.0592MHz
// smod=0
#define UART_TIMER1_9600_BAUD      0xFD  

// toggle sound 0->1, 1->0    
void T0_int(void) interrupt 1    
{
      TH0=count_high_byte;
      TL0=count_low_byte;
    
      sound =~ sound;
    
} /* T0_int */

void init_uart()
{
    SCON = 0x50;  // UART mode 1
    TMOD = 0x20;  // time1 mode2 , 9600 baud
    TH1  = UART_TIMER1_9600_BAUD;
    TR1  = 1; // start
    TI   = 1; 
}


/**
 * 数码管显示数据
 * @param  [unsigned short char] cmd [写入十六进制数]
 * @return [type]            [description]
 */
void CH452_Write(unsigned short cmd) 
{ 
	unsigned char i; 
	CH452_LOAD_CLR; 
	for(i=0;i!=12;i++){  
		CH452_DCLK_CLR; 
		CH452_DIN=cmd&1; 
		CH452_DCLK_SET; 
		cmd>>=1; 
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
	unsigned char i,keyw=0,cmd=0x07;
	EX0=0;	
	CH452_LOAD_CLR;
	for(i=0;i<4;i++) {
		CH452_DIN=cmd&1;
		CH452_DCLK_CLR; 
		cmd>>=1;
		CH452_DCLK_SET; 
	}
	CH452_LOAD_SET;
	for(i=0;i<7;i++) {
		keyw<<=1;
		keyw|=CH452_DOUT;
		CH452_DCLK_CLR;
		CH452_DCLK_SET;
	}
	//flag=1;
	EX0=1;
	IE0=0;
	switch (keyw){
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
/**
 * 数码管显示数据
 * @param  [int] X [数码管显示数字]
 * @param  [int] Y [数码管选择]
 */
void display(int X, int Y) {
	CH452_Write(DIG[X] | SEG[Y]);
}

void main() 
{ 
	//开启中断
	init_uart(); 
  	EA  = 1;
  	ET0 = 1;
  	TMOD |= 0x01;
  	TR0   = 0; 

	CH452_Write(CH452_RESET); 
	CH452_Write(CH452_NO_BCD);
	CH452_Write(CH452_SYSON2); 
	while (1) {
		char tmp = CH452_Read();
		display(4,tmp);
		display(5,tmp);
		display(6,tmp);
		display(7,tmp);
	}
}