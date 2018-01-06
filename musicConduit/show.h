#include <STC15F2k60S2.h>
#include "12864g-86-pc.h"
#include <intrins.h>

/**
 * 延迟函数
 *
 * @param[in]  x     { parameter_description }
 */
void delay1(unsigned long x)
{
	for (; x > 0; x--) {
		_nop_();
		_nop_();
	}
}
void delShow(int note, int height, unsigned long x)
{
	int i, j;

	for (j = (3 - height) * 3; j < 9 ; j++) {
		delay1(x);
		for (i = (note - 1) * 16; i < note * 16; ++i) {
			lcd_address(j + 1, i);
			transfer_data_lcd(0x00);
		}

	}
}
void show(int note, int height)
{
	int i, j;
	for (j = height * 3 - 1; j > 0 ; j--) {
		for (i = (note - 1) * 16; i < note * 16; ++i) {
			lcd_address(9 - j, i);
			transfer_data_lcd(0xff);
		}
	}

	// delShow(note, height);
}
