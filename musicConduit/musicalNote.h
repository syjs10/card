#ifndef _MUSICAL_NODE_
#define _MUSICAL_NODE_
#include <STC15F2K60S2.H>
#include <intrins.h>
#include "show.h"
#include "led.h"
long beat = 1.5e4;
sbit sound = P4 ^ 5;

/**
 * 延迟函数
 *
 * @param[in]  x     { parameter_description }
 */
void delay2(unsigned long x)
{
	for (; x > 0; x--) {
		_nop_();
		_nop_();
	}
}
/**
 * 音符发声函数
 *
 * @param[in]  note  音符
 * @param[in]  beat  节拍
 */
void tune(int note, long beat) //音符
{
	long i, n, end;

	float f;

	if(P27==0){
		return;
	}
	if (note == 0) delay2(beat / 10 * 9); //休止符
	else
	{
		/**节拍**/
		if (note > 1000 && note < 1100) {
			i = beat;
		} //1
		else if (note > 500 && note < 600) {
			i = beat / 2;
		} //1/2
		else if (note > 200 && note < 300) {
			i = beat / 4;
		} //1/4
		else if (note > 700 && note < 800) {
			i = beat / 4 * 3;
		} //3/4
		else if (note > 1200 && note < 1300) {
			i = beat / 4 * 5;
		} //4/5
		else if (note > 1500 && note < 1600) {
			i = beat / 2 * 3;
		} //3/2
		else if (note > 2000 && note < 2100) {
			i = beat * 2;
		} //2
		switch (note % 100) {
			case 11: f = 261.6; break;
			case 12: f = 293.7; break;
			case 13: f = 329.6; break;
			case 14: f = 349.2; break;
			case 15: f = 392.6; break;
			case 16: f = 440.0; break;
			case 17: f = 493.9; break;

			case 21: f = 523.3; break;
			case 22: f = 587.3; break;
			case 23: f = 659.3; break;
			case 24: f = 698.5; break;
			case 25: f = 784.0; break;
			case 26: f = 880.0; break;
			case 27: f = 987.8; break;

			case 31: f = 1045.5; break;
			case 32: f = 1174.7; break;
			case 33: f = 1318.5; break;
			case 34: f = 1396.9; break;
			case 35: f = 1568.0; break;
			case 36: f = 1760.0; break;
			case 37: f = 1975.5; break;

			default: break;
		}
		n = 1e5 / f / 2; //根据频率算出当前的半周期(us)
		end = i;

		for (i = 2 * i / n; i > 0; i--) {//重复震动次数计算
			sound = ~sound;
			delay2(n); //延迟半周期
		}
		// delay(end / 10); //当前周期结束暂停时间
		show(note % 10, note % 100 / 10);
		delShow(note % 10, note % 100 / 10, end / 10);
	}
}
/**
 * 音乐播放函数
 *
 * @param      b     节拍数组
 * @param[in]  beat  一个节拍长度
 * @param[in]  len   乐谱长度
 */
void play(int* b, long beat, int len) //播放
{
	int p;
	for (p = 0; p < len; p = p + 1)
	{
		tune(b[p], beat);   //单个音符传递给tune函数
		display(4, b[p] / 1000);
		display(5, b[p] % 1000 / 100);
		display(6, b[p] % 100 / 10);
		display(7, b[p] % 10);
	}
}
#endif
