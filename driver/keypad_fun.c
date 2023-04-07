/*
 * keypad_fun.c
 *
 *  Created on: 2018年8月2日
 *      Author: Administrator
 */
#include "common.h"
#include "keypad_fun.h"
#include "beep.h"
#include "led_disp.h"
#include "uart_frame.h"
#include "uart.h"
#include "treadmills.h"
#include "7_segment.h"
#include "indecate.h"
#include "treadmills.h"
#include "param.h"
#include "ee_param.h"

bit write_flag = 0;
//bit manual_to_auto = 0;
//bit last_auto_flag =0;

void keypad_short_proc(void);
void keypad_long_proc(void);
void keypad_continue_proc(void);
void keypad_all_realse_proc(void);


void keypad_all_realse_proc(void)
{


}


static void disp_speed_adc_param(void)
{
	u8 temp=0;
	indecate.led_speed=LED_OFF;
	indecate.led_distance=LED_OFF;
	indecate.led_calorie=LED_OFF;
	indecate.led_auto=LED_OFF;
	indecate.led_time=LED_OFF;
	indecate.led_updp=LED_OFF;
	indecate.led_downdp=LED_OFF;

	set_bit_seg(0,SEG_ALL_OFF);
	if(param.speed_adc<0)
	{
		set_bit_seg(1,SEG__);
		temp=-param.speed_adc;
	}
	else
	{
		set_bit_seg(1,SEG_ALL_OFF);
		temp=param.speed_adc;
	}

	set_bit_seg(2,seg_num[temp/10]);
	set_bit_seg(3,seg_num[temp%10]);

	set_seg_mode(SEG_MODE_FLASH_SELF);
}





void keypad_short_proc(void)
{
	u8 bitmap = key;
    u8 tar_speed;
		if (1 == key_cnt) //单键
		{

			if(param.en==1)//进入参数设置模式
			{

				if((bitmap & BTN_MODE ) &&(key_mask & BTN_MODE))
				{

					BEEP_SWITCH_ON_1_CNT(); //2023.3.27

					if(treadmills.status==STATUS_WAIT)
					{
						param.index++;

						if(param.index>4) //2023.3.27
						{
							param.index=1;
						}

						param.en=1;
						treadmills.display.index=param.index;
						key_mask=0XFF;//打开所有键盘

						treadmills.param.second=0;
						treadmills.param.distance=0;
						treadmills.param.calorie=0;

						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);
					}
				}

				if((bitmap & BTN_ADD) &&(key_mask & BTN_ADD))
				{

					if(param.index==PARAM_SET_SPEED_ADC)
					{
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27

						param.speed_adc+=1;
						if(param.speed_adc>SPEED_ADC_MAX){param.speed_adc=SPEED_ADC_MAX;}
						disp_speed_adc_param();
					}
          else if( param.index == PARAM_SET_BEEP)      //2023.3.27
          {
            
            BEEP_SWITCH_ON_1_CNT();  //2023.3.27
 
            treadmills.param.beep_status = BEEP_SWITCH_ON;
            
            treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);

          }
					else if(param.index==PARAM_SET_TIME)
					{
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						if((treadmills.param.second+60)<=(TIME_MAX))
						{
							treadmills.param.second+=60;
						}
						else
						{
							treadmills.param.second=TIME_MAX;
						}

						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);

					}
					else if(param.index==PARAM_SET_DISTANCE)
					{
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						treadmills.param.distance+=1000;

						if(treadmills.param.distance>DISTANCE_MAX)
						{
							treadmills.param.distance=DISTANCE_MAX;
						}
						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);
					}
					else if(param.index==PARAM_SET_CALORIE)
					{
            BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						//单位: 卡
						treadmills.param.calorie+=1000;
						if(treadmills.param.calorie>CALORIE_MAX)
						{treadmills.param.calorie=CALORIE_MAX;}
						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);
					}
					else{}
				}
        
				if((bitmap & BTN_SUB) &&(key_mask & BTN_SUB))
				{
					if(param.index==PARAM_SET_SPEED_ADC)
					{
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						param.speed_adc-=1;
						if(param.speed_adc<SPEED_ADC_MIN){param.speed_adc=SPEED_ADC_MIN;}
						disp_speed_adc_param();
					}
          else if( param.index == PARAM_SET_BEEP)      //2023.3.27
          {
            BEEP_SWITCH_ON_1_CNT();  //2023.3.27
            
            treadmills.param.beep_status = BEEP_SWITCH_OFF;
            
            treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);

          }
					else if(param.index==PARAM_SET_TIME)
					{
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						if(treadmills.param.second>=60)
						{
							treadmills.param.second-=60;
						}
						else
						{
							treadmills.param.second=0;
						}
						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);
					}
					else if(param.index==PARAM_SET_DISTANCE)
					{
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						if(treadmills.param.distance>=1000)
						{
							treadmills.param.distance-=1000;
						}
						else
						{
							treadmills.param.distance=DISTANCE_MIN;
						}

						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);
					}
					else if(param.index==PARAM_SET_CALORIE)
					{
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						//单位:卡
						if(treadmills.param.calorie>=1000)
						{
							treadmills.param.calorie-=1000;
						}
						else
						{
							treadmills.param.calorie=CALORIE_MIN;
						}
						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);
					}
					else{}
				}

				//------ 设置退出处理 ----------------------------
				if((bitmap & BTN_ONOFF) &&(key_mask & BTN_ONOFF))
				{
					BEEP_SWITCH_ON_1_CNT(); //2023.3.27
					param.en=0;
					set_seg_mode(SEG_MODE_NORMAL);
          
          write_flag = 1;

					if( (treadmills.param.second) \
					    ||(treadmills.param.distance) \
					    ||treadmills.param.calorie)
					{
						param.book_en=1;
					}
					else
					{
						param.book_en=0;
					}

					if(param.index==PARAM_SET_SPEED_ADC)
					{
						//给主控发送参数指令，让其保存在FLASH中.
						uart_tx_frame.cmd=CMD_WRITE_PARAM;
						uart_tx_frame.len=2;
						uart_tx_frame.buf[0]=(u8)param.speed_adc;
						uart_tx_frame.buf[1]=(~(u8)param.speed_adc);
						UART_Send_Buf(UART0,(UINT8*)&uart_tx_frame,sizeof(uart_tx_frame));
						param.index=0;
						treadmills.status=STATUS_STOP_OVER;
					}
					else
					{
						if(param.book_en)
						{
							treadmills.status=STATUS_RUN;
							key_mask =0xff;//打开所有按键
						}
						else
						{
							param.index=0;
							treadmills.status=STATUS_STOP_OVER;
						}
					}
				}
			}
			else//正常模式
			{
			   // ON/OFF 按键
				if((bitmap & BTN_ONOFF) &&(key_mask & BTN_ONOFF))
				{
          
					if(treadmills.status==STATUS_WAIT)
					{
						debug_printf("\r\n run");
						treadmills.status=STATUS_RUN;
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
						key_mask =0xff;//打开所有按键
					}
					else if(treadmills.status==STATUS_RUN)//进入运行到计时间，但未真正的开始运行的时候。
					{
						debug_printf("\r\n STATUS_STOP_OVER");
						treadmills.status=STATUS_STOP_OVER;
						key_mask=0x00;//禁止所有按键操作
						BEEP_SWITCH_ON_1_CNT(); //2023.3.27
					}
					else if(treadmills.status==STATUS_RUNNING)//电机正在运行中
					{
						debug_printf("\r\n stop");
						treadmills.status=STATUS_STOP;
						key_mask=0x00;//禁止所有按键操作
            
            if(treadmills.param.beep_status == BEEP_SWITCH_ON)
            {
             	beep_set(2,DEFAULT_ON_TICKS,DEFAULT_OFF_TICKS);
            }
            else
            {}            

					
					}
					else{}
          
				}

				// MODE 按键
				if((bitmap & BTN_MODE ) &&(key_mask & BTN_MODE))
				{

					BEEP_SWITCH_ON_1_CNT(); //2023.3.27
					if(treadmills.status==STATUS_WAIT)  
					{
						param.index++;
						if(param.index>4) //3
						{
							param.index=1;
						}

            treadmills.param.beep_status = ee_param.beep_status; //读取上次保存的beep_status,2023.3.27
            
						treadmills.param.second=0;
						treadmills.param.distance=0;
						treadmills.param.calorie=0;

						param.en=1;
						treadmills.display.index=param.index;
						key_mask=0XFF;//打开所有键盘
						treadmills_display(param.index);
						set_seg_mode(SEG_MODE_FLASH_SELF);
						return;
					}

					if(treadmills.status==STATUS_RUNNING)
					{
						if((treadmills.display.mode==DISP_MODE_AUTO) || ( treadmills.display.mode ==DISP_MODE_TO_AUTO ))
						{							
							treadmills.display.mode=DISP_MODE_SINGLE;
              treadmills.display.index=0;
						}
						else 
						{
                 treadmills.display.index++;  
                 if( treadmills.display.index == 1)
                 {
                   treadmills.display.index++;
                 }

                treadmills.display.mode=DISP_MODE_SINGLE;

                if(treadmills.display.index>4) //  if(treadmills.display.index==4)
                {
                  treadmills.display.mode=DISP_MODE_AUTO;
                  treadmills.display.index=0;                 
                }
						}
					}

				}

				if(treadmills.status!=STATUS_RUNNING){return;}//做一个条件限制
				// + 按键
				if((bitmap & BTN_ADD) &&(key_mask & BTN_ADD))
				{
					BEEP_SWITCH_ON_1_CNT(); //2023.3.27
					if(treadmills.param.speed < DEFAULT_SPEED_MAX)  //600
					{
						treadmills.param.speed += DEFAULT_SPEED_STEP; //50
						tar_speed=map_speed(treadmills.param.speed/10);
						uart_frame_tx(CMD_SPEED,tar_speed);

						debug_printf("\r\n speed(+)=");
						debug_printf_d(treadmills.param.speed);
					}

					treadmills.display.index=0;
//					treadmills.display.mode=DISP_MODE_SINGLE; 
					treadmills.display.mode=DISP_MODE_AUTO; //~~~~~~~~~~~~

				}

				// - 按键
				if((bitmap & BTN_SUB) &&(key_mask & BTN_SUB))
				{
					BEEP_SWITCH_ON_1_CNT(); //2023.3.27
					if(treadmills.param.speed>100)
					{
						treadmills.param.speed -= DEFAULT_SPEED_STEP;  //50
						tar_speed=map_speed(treadmills.param.speed/10);
						uart_frame_tx(CMD_SPEED,tar_speed);

						debug_printf("\r\n speed(-)=");
						debug_printf_d(treadmills.param.speed);
					}
					treadmills.display.index=0;
//					treadmills.display.mode=DISP_MODE_SINGLE;
          treadmills.display.mode=DISP_MODE_AUTO; //~~~~~~~~~~~~

				}
        
			}
		}
		else if(2 == key_cnt)//双键
		{
			//对码成功
			if(((bitmap & BTN_ADD) &&(key_mask & BTN_ADD))  && ((bitmap & BTN_MODE ) &&(key_mask & BTN_MODE)))
			{
        if(treadmills.param.beep_status == BEEP_SWITCH_ON)
        {
          beep_set(2,DEFAULT_ON_TICKS,DEFAULT_OFF_TICKS);
        }
        else
        {} 

			}

			if(treadmills.status !=STATUS_WAIT){return;}//表示当前不在WAIT状态 需要退出
#if(PARAME_SET_EN)
			//进入参数设置模式
			if(((bitmap & BTN_ONOFF) &&(key_mask & BTN_ONOFF))  && ((bitmap & BTN_MODE ) &&(key_mask & BTN_MODE)))
			{
        if(treadmills.param.beep_status == BEEP_SWITCH_ON)
        {
          beep_set(2,DEFAULT_ON_TICKS,DEFAULT_OFF_TICKS);
        }
        else
        {} 
				param.en=1;
				param.index=PARAM_SET_SPEED_ADC;
				disp_speed_adc_param();
				key_mask=0XFF;//打开所有键盘
			}
#endif
		}
		else
		{}
}


void keypad_long_proc(void)
{

}



void keypad_continue_proc(void)
{

}








