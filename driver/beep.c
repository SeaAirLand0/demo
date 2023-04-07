/*
 * beep.c
 *
 *  Created on: 2019��7��3��
 *      Author: Administrator
 */

#ifndef DRIVER_BEEP_C_
#define DRIVER_BEEP_C_
#include "common.h"
#include "beep.h"
#include "pwm_simple.h"


#define BEEP_DISABLE()  {PWM1DutyCycle(0);}
#define BEEP_ENABLE()   {PWM1DutyCycle(500);}




static u8 cnt=0;
static u16 on=0,off=0;
static u16 ticks=0;

void beep_force_stop(void)
{
	cnt=0+1;
	BEEP_DISABLE();
}


void beep_set(u8 count,u16 on_ticks,u16 off_ticks)
{
#if(0)
	if(count==0){cnt=0;BEEP_DISABLE();return;}
	on=on_ticks;
	off=off_ticks;
	BEEP_ENABLE();
	cnt=count+1;
	ticks=0;
#else
	if(count==0){cnt=0;BEEP_DISABLE();return;}
	if(cnt){return;}
	on=on_ticks;
	off=off_ticks;
	BEEP_ENABLE();
	cnt=count+1;
	ticks=0;
#endif
}





void beep_sound_proc(void)
{
	if(cnt==1)
	{
		BEEP_DISABLE();
		cnt=0;
	}
	else if(cnt>1)
	{
		if(ticks==on)
		{
			BEEP_DISABLE();
			cnt--;
		}
		else if(ticks>(off+on))
		{
			if(cnt>1){BEEP_ENABLE();}
			ticks=0xffff;
		}
		ticks++;
	}
	else{}

}





#endif /* DRIVER_BEEP_C_ */
