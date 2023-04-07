/*
 * treadmills.c
 *
 *  Created on: 2019年12月12日
 *      Author: Administrator
 */
#include "common.h"
#include "treadmills.h"
#include "keypad.h"
#include "led_disp.h"
#include "keypad_fun.h"
#include "uart_frame.h"
#include "indecate.h"
#include "7_segment.h"
#include "beep.h"
#include "uart.h"
#include "param.h"
static void treadmills_display (index);

treadmills_t treadmills;

extern u8 set_param_en;
extern s8 param_speed;


void treadmills_init(void)
{
	treadmills.ack=0;
	treadmills.param.second=0;
	treadmills.param.speed=100;
  
  treadmills.param.beep_status = BEEP_SWITCH_ON;  //0表示蜂鸣器开，1表示关
}

u8 map_speed(u8 speed)
{
	float voltage;
	u8 tar_speed;
	voltage=GET_SPEED_VOLTAGE(speed);

	tar_speed=MAP_VOLTAGE_2_SPEED(voltage)/10;
	return tar_speed;
}

static const u8 boot_disp_tab[]=
{
#if(0)
		SEG_UP_,
		SEG_DWN_,
		SEG__
#else
		NUM_0,
		NUM_0,
		NUM_0,
#endif
};


static const u8 run_disp_tab[]=
{
		NUM_3,
		NUM_2,
		NUM_1
};


u8 booktime_en=0;
u8 booktime_cnt=0;
//时基 10ms
void boot_time(void)
{
	if(booktime_en==0){booktime_cnt=0;return;}
	booktime_cnt++;
	if(booktime_cnt>STOP_DISPLAY_INTERVAL)//18
	{
		booktime_cnt=0;
		if(treadmills.param.speed>=10)
		{
			treadmills.param.speed-=10;
		}
		else
		{
			treadmills.param.speed=0;
		}
		treadmills.display.index=0;//速度档
		treadmills_display(treadmills.display.index);

		if(treadmills.param.speed==0)
		{
			booktime_cnt=0;
			booktime_en=0;
		}
	}

}

void treadmills_param_calc(void)
{
    static float distance=0;
	if(treadmills.status !=STATUS_RUNNING){distance=0;return;}


	if(param.index==PARAM_SET_TIME)
	{
		if(treadmills.param.second){treadmills.param.second--;}
		else
		{
			treadmills.status=STATUS_STOP;
			key_mask =0x00;
      if(treadmills.param.beep_status == BEEP_SWITCH_ON)
      {
        	beep_set(STOP_BEEP_CNT,STOP_BEEP_ON_TICKS,STOP_BEEP_OFF_TICKS);
      }
      else
      {}

		}
	}
	else
	{
		treadmills.param.second++;
		//增加定时功能---必须和键盘统一
		if(treadmills.param.second>=3600)//
		{
			treadmills.status=STATUS_STOP;
			key_mask =0x00;
      
      if(treadmills.param.beep_status == BEEP_SWITCH_ON)
      {
        	beep_set(STOP_BEEP_CNT,STOP_BEEP_ON_TICKS,STOP_BEEP_OFF_TICKS);
      }
      else
      {}
		}
	}


	if(param.index==PARAM_SET_DISTANCE)
	{
		distance+=((float)(treadmills.param.speed))/360.0;//米/秒  单位：米

		treadmills.param.distance-=((float)(treadmills.param.speed))/360.0;//米/秒  单位：米

		if(treadmills.param.distance<10)
		{
			treadmills.status=STATUS_STOP;
			key_mask =0x00;
      if(treadmills.param.beep_status == BEEP_SWITCH_ON)
      {
        	beep_set(STOP_BEEP_CNT,STOP_BEEP_ON_TICKS,STOP_BEEP_OFF_TICKS);
      }
      else
      {}
		}
	}
	else
	{
		/**
		* distance=V*t
		*         =(((float)treadmills.param.speed/100.0)*1000.0)/(3600.0)
		*/
		treadmills.param.distance+=((float)(treadmills.param.speed))/360.0;//米/秒  单位：米
		distance=treadmills.param.distance;
	}


	if(param.index==PARAM_SET_CALORIE)
	{
		if(treadmills.param.calorie>=(((float)(treadmills.param.speed))/360.0)*80)//80千卡/千米 =0.080K/M  单位:卡
		{
			treadmills.param.calorie-=(((float)(treadmills.param.speed))/360.0)*80;
		}
		else
		{
			treadmills.param.calorie=0;
			treadmills.status=STATUS_STOP;
			key_mask =0x00;
      
      if(treadmills.param.beep_status == BEEP_SWITCH_ON)
      {
        	beep_set(STOP_BEEP_CNT,STOP_BEEP_ON_TICKS,STOP_BEEP_OFF_TICKS);
      }
      else
      {}
		}
	}
	else
	{
		treadmills.param.calorie=distance*80;//80千卡/千米 =0.080K/M  单位:卡
	}

	treadmills_display(treadmills.display.index);
}

//时基 100ms
void treadmills_proc_loop(void)
{
	u8 tar_speed;
	static u8 cnt=0,timeout=0;
    static u8 val=0;
	switch(treadmills.status)
	{
    case STATUS_POWERON:
        cnt++;
        if(cnt>1)//10
        {
        	cnt=0;
          indecate.led_speed=LED_OFF;
          indecate.led_distance=LED_OFF;
          indecate.led_calorie=LED_OFF;
          indecate.led_auto=LED_ON;//LED_OFF;
          indecate.led_time=LED_ON;
          indecate.led_back_light = LED_ON;//2023.3.22
            
          indecate.led_updp = LED_OFF;//LED_ON;
          indecate.led_downdp = LED_OFF;//LED_ON;

          set_bit_seg(0,boot_disp_tab[val]);
          set_bit_seg(1,boot_disp_tab[val]);
          set_bit_seg(2,boot_disp_tab[val]);
          set_bit_seg(3,boot_disp_tab[val]);

          treadmills.display.index=1;//时间档位
          treadmills.param.second=0;
          //treadmills_display(treadmills.display.index);
          treadmills.status=STATUS_WAIT;

          uart_tx_frame.cmd=CMD_READ_PARAM;
          uart_tx_frame.len=1;
          uart_tx_frame.buf[0]=0x00;
          UART_Send_Buf(UART0,(UINT8*)&uart_tx_frame,sizeof(uart_tx_frame));

        	//key_mask= BTN_ONOFF |BTN_MODE;
        	key_mask=0xff;
        }
		break;
	case STATUS_WAIT:
		break;
	case STATUS_RUN:		
		if(cnt==0)
		{
			 BEEP_SWITCH_ON_1_CNT(); //2023.3.27

				set_bit_seg(0,run_disp_tab[val]);
				set_bit_seg(1,run_disp_tab[val]);
				set_bit_seg(2,run_disp_tab[val]);
				set_bit_seg(3,run_disp_tab[val]);
				cnt=1;
				val=1;
      
          indecate.led_auto=LED_OFF;   //~~~~~~~~
          indecate.led_updp = LED_OFF;
          indecate.led_downdp = LED_OFF;
      
		}
		else
		{
			cnt++;
			 if(cnt>10)
			{
				 cnt=1;
				BEEP_SWITCH_ON_1_CNT(); //2023.3.27
        if(val<=2)
        {
					set_bit_seg(0,run_disp_tab[val]);
					set_bit_seg(1,run_disp_tab[val]);
					set_bit_seg(2,run_disp_tab[val]);
					set_bit_seg(3,run_disp_tab[val]);
					val++;
          
          indecate.led_auto=LED_OFF;   //~~~~~~~~
          indecate.led_updp = LED_OFF;
          indecate.led_downdp = LED_OFF;

        }
				else
				{
					val=0;
					treadmills.param.speed=100;

					tar_speed=map_speed(treadmills.param.speed/10);
					uart_frame_tx_2(CMD_STATUS,STATUS_MOTOR_RUN,tar_speed);
					uart_frame_tx_2(CMD_STATUS,STATUS_MOTOR_RUN,tar_speed);

					treadmills.status=STATUS_RUNNING;
					treadmills.display.index=0;//速度档
					treadmills_display(treadmills.display.index);
				}
			}
		}
		break;
	case STATUS_RUNNING:
		break;
	case STATUS_STOP:
		indecate.led_auto=LED_OFF;
		treadmills.display.mode=IDEL;
		treadmills.param.second=0;
		treadmills.param.second=0;
		treadmills.param.distance=0;
		treadmills.param.calorie=0;

		uart_frame_tx(CMD_STATUS,STATUS_MOTOR_STOP);
		uart_frame_tx(CMD_STATUS,STATUS_MOTOR_STOP);

    	booktime_cnt=0;
    	booktime_en=1;
    	param.book_en=0;
    	param.index=0;
    	cnt=0;
    	timeout=0;
    	treadmills.status=STATUS_STOP_WAIT;

		break;
	case STATUS_STOP_WAIT:
		timeout++;
		if(timeout>STOP_WAIT_TIMEOUT)//超时退出 5*5=25   //100
		{
			timeout=0;
			treadmills.status=STATUS_STOP_OVER;
		}
		break;
	case STATUS_STOP_OVER:
		cnt++;
		if(cnt>3)//5=约1秒
		{
			indecate.led_auto= LED_ON;//LED_OFF;  //~~~~~2023.3.16
			treadmills.display.mode=IDEL;
			treadmills.param.second=0;
			treadmills.param.second=0;
			treadmills.param.distance=0;
			treadmills.param.calorie=0;

			treadmills.display.index= 2;//1时间档 //现在2是时间档2023.3.27
			treadmills_display(treadmills.display.index);
			key_mask =BTN_ONOFF|BTN_MODE;
			booktime_en=0;
			param.book_en=0;
			param.index=0;
			treadmills.status=STATUS_WAIT;			
			cnt=0;
			val=0;
			beep_force_stop();
		}
		break;
	case STATUS_ERROR:
      key_mask=0x00;//关闭所有的按键功能
      set_bit_seg(0,SEG_E);
      set_bit_seg(1,SEG_R);
      set_bit_seg(2,seg_num[0]);
      set_bit_seg(3,seg_num[treadmills.error_code]);

      indecate.led_speed=LED_OFF;
      indecate.led_distance=LED_OFF;
      indecate.led_calorie=LED_OFF;
      indecate.led_auto=LED_OFF;
      indecate.led_time=LED_OFF;
      indecate.led_updp=LED_OFF;
      indecate.led_downdp=LED_OFF;
	    treadmills.status=IDEL;
	    treadmills.display.mode=IDEL;
	    treadmills.status=IDEL;

      if(treadmills.param.beep_status == BEEP_SWITCH_ON)
      {
        	 beep_set(1,ERROR_ON_TICKS,ERROR_OFF_TICKS);
      }
      else
      {}
	   
		 break;
	default:
		cnt=0;
		break;
	}
}


void treadmills_display (index)
{
	u8 high,low;
  u8 beep_status;
	float val;
	if(index == PARAM_SET_SPEED_ADC)//显示速度 km/hour
		{
		indecate.led_speed=LED_ON;
		indecate.led_distance=LED_OFF;
		indecate.led_calorie=LED_OFF;
		//indecate.led_auto=LED_OFF;
		indecate.led_time=LED_OFF;
		indecate.led_updp=LED_OFF;
		indecate.led_downdp=LED_ON;

		high=treadmills.param.speed/100;
		low=treadmills.param.speed%100;
		set_seg_two(high,low);
		}
    else if( index == PARAM_SET_BEEP)  //2023.3.27
    {
        indecate.led_speed=LED_OFF;
        indecate.led_distance=LED_OFF;
        indecate.led_calorie=LED_OFF;
        indecate.led_auto=LED_OFF;
        indecate.led_time=LED_OFF;
        indecate.led_updp=LED_OFF;
        indecate.led_downdp=LED_OFF;
        beep_status = treadmills.param.beep_status;

        if( beep_status == BEEP_SWITCH_ON)
        {
          set_bit_seg(0,SEG_O);
          set_bit_seg(1,SEG_N);
          set_bit_seg(2,SEG_ALL_OFF);
          set_bit_seg(3,SEG_ALL_OFF);

        }
        else if( beep_status == BEEP_SWITCH_OFF)
        {
          set_bit_seg(0,SEG_O);
          set_bit_seg(1,SEG_F);
          set_bit_seg(2,SEG_F);
          set_bit_seg(3,SEG_ALL_OFF);  
        }

    } 
		else if(index == PARAM_SET_TIME)//时间:mm:ss
		{
			indecate.led_speed=LED_OFF;
			indecate.led_distance=LED_OFF;
			indecate.led_calorie=LED_OFF;
			//indecate.led_auto=LED_OFF;
			indecate.led_time=LED_ON;
			indecate.led_updp=LED_ON;
			indecate.led_downdp=LED_ON;

			high=((treadmills.param.second/60)%100);
			low=treadmills.param.second%60;
			set_seg_two(high,low);
		}
		else if(index == PARAM_SET_DISTANCE )//路程 km
		{
			indecate.led_speed=LED_OFF;
			indecate.led_distance=LED_ON;
			indecate.led_calorie=LED_OFF;
			//indecate.led_auto=LED_OFF;
			indecate.led_time=LED_OFF;
			indecate.led_updp=LED_OFF;
			indecate.led_downdp=LED_ON;
			if(treadmills.param.distance>99990)
			{
				treadmills.param.distance=99990;
			}
			val=(treadmills.param.distance);
			high=((u32)val/1000);
			low=((u32)val/10)%100;

			set_seg_two(high,low);
		}
		else if(index == PARAM_SET_CALORIE )//卡洛里 kcal
		{
			indecate.led_speed=LED_OFF;
			indecate.led_distance=LED_OFF;
			indecate.led_calorie=LED_ON;
			//indecate.led_auto=LED_OFF;
			indecate.led_time=LED_OFF;
			indecate.led_updp=LED_OFF;
			indecate.led_downdp=LED_OFF;

			//treadmills.param.calorie//单位：卡
			if(treadmills.param.calorie<=99000)//=99.99千卡
			{
				indecate.led_downdp=LED_ON;

				high=(treadmills.param.calorie/1000);
				low=(treadmills.param.calorie%1000)/10;
			}
			else
			{
//				indecate.led_downdp=LED_OFF;
         if(treadmills.param.calorie<=999000.00)//999.00千卡
         {
            high=(treadmills.param.calorie/1000)/10;
            low=((treadmills.param.calorie/100)%100);
         }
			}
			set_seg_two(high,low);
			set_bit_seg(3,SEG_ALL_OFF);//强制关闭显示
		}
		else{}
}



//时基 100ms
void treadmills_disp_loop(void)
{
	static u8 cnt=0;
	switch(treadmills.display.mode)
	{
    case DISP_MODE_AUTO:
                      cnt=0;
                      //显示时间
                      treadmills.display.index=0;  //2023.3.16
                      treadmills_display(treadmills.display.index);
                      treadmills.display.mode=DISP_MODE_TO_AUTO;
                      indecate.led_auto=LED_ON;
                      break;
    case DISP_MODE_TO_AUTO:
                      cnt++;
                      if(cnt>50)  //50
                      {
                        cnt=0;
                        treadmills.display.index++;  
                        if( treadmills.display.index == 1)  //运行状态跳过蜂鸣器的显示
                        {
                          treadmills.display.index++;
                        }  
                        if(treadmills.display.index>=5){treadmills.display.index=0;}  //4
                        
                        treadmills_display(treadmills.display.index);

                        indecate.led_auto=LED_ON;
                      }
                      break;
    case DISP_MODE_SINGLE:
                      treadmills_display(treadmills.display.index);
                      indecate.led_auto=LED_OFF;
                      treadmills.display.mode=IDEL; //  
                      break;
    default:
                      break;
	}

}





#if(1)
void communication_checkout(void)
{
	static u8 timeout=0;
	static u8 sm=0;
	static u8 error_cnt=0;
	switch(sm)
	{
	case 0:
		treadmills.ack=0;
		uart_frame_tx(CMD_ACK,treadmills.ack);
		//LED1=0;//点亮
		debug_printf("\r\n send ack");
		timeout=0;
		sm=1;
		break;
	case 1:
		timeout++;
		if(treadmills.ack==0)
		{
			uart_frame_tx(CMD_ACK,treadmills.ack);
			if(timeout>1)
			{
				timeout=0;
				error_cnt++;
				if(error_cnt>10)
				{
          treadmills.error_code=ERROR_COMM;
          treadmills.status=STATUS_ERROR;
          sm=2;
          debug_printf("\r\n --->wait ack  timeout");
          if(treadmills.param.beep_status == BEEP_SWITCH_ON)
          {
           beep_set(3,ERROR_ON_TICKS,ERROR_OFF_TICKS);
          }
          else
          {}
					
				}
				else
				{
					sm=0;
				}
			}
		}
		else
		{
			error_cnt=0;
			sm=0;
		}
		break;
	case 2:
		break;
	default:
		break;
	}

}

#endif
