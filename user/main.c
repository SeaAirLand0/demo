
/**********************************************************************************************
//2023.3.22 ：背光灯由TM1620驱动 在上电状态直接点亮。
//2023.3.29 ：1.增加蜂鸣器开关，
              2.上电传输默认设定好的的最大速度，最大电压，最小电压，kiv等参数。
              3.优化显示逻辑。

************************************************************************************************/
/*
 * main.c
 *
 *  Created on: 2019年11月28日
 *      Author: Administrator
 */

#include "common.h"
#include "aip1628.h"
#include "led_disp.h"
#include "indecate.h"
#include "keypad.h"
#include "keypad_fun.h"
#include "pwm_simple.h"
#include "beep.h"
#include "uart.h"
#include "uart_frame.h"
#include "treadmills.h"
#include "param.h"
#include "ee_param.h"


static bit poweron_send_flag = 0;

extern UINT8  ms10_flag;
extern UINT8  ms100_flag;
extern UINT8  ms500_flag;
extern UINT8  ms1000_flag;

//extern UINT8 ms50_flag;

extern void Timer0_Init(void);
extern void uart0_init(void);

extern u8 set_param_en;




void MODIFY_HIRC_166(void)
{
    unsigned char data hircmap0,hircmap1;
    unsigned int trimvalue16bit;
	/* Check if power on reset, modify HIRC */
	if (PCON&SET_BIT4)
	{

    CKDIV = 0x00;
	set_IAPEN;
	IAPAL = 0x30;
    IAPAH = 0x00;
    IAPCN = READ_UID;
    set_IAPGO;
	hircmap0 = IAPFD;
	IAPAL = 0x31;
    IAPAH = 0x00;
    set_IAPGO;
	hircmap1 = IAPFD;
	clr_IAPEN;
	trimvalue16bit = ((hircmap0<<1)+(hircmap1&0x01));
	trimvalue16bit = trimvalue16bit - 15;
	hircmap1 = trimvalue16bit&0x01;
	hircmap0 = trimvalue16bit>>1;
	TA=0XAA;
	TA=0X55;
	RCTRIM0 = hircmap0;
	TA=0XAA;
	TA=0X55;
	RCTRIM1 = hircmap1;


	//CKDIV = 0x50;					//HIRC devider 160
	//set_CLOEN;						//Check HIRC out wavefrom to confirm the HIRC modified
	/* Clear power on flag */
	 PCON &= CLR_BIT4;
	}

}


/*************************************************************************
@brief:  保存设置的参数值

@parameter: 无

@return: 无
**************************************************************************/
static void save_param(void)
{
  u8 i = 0;
        //**********2023.2.3保存设置的参数*********
           
  ee_param.beep_status = treadmills.param.beep_status;

  write_param();  //写保存上面的数据

}


/*************************************************************************
@brief:  读取设置的参数值

@parameter: 无

@return: 无
**************************************************************************/
static void read_beep_status(void)
{
  read_param(); 
  treadmills.param.beep_status = ee_param.beep_status;

}

//上电发送默认的参数值
static void send_default_param(void)
{
     u8 i=0;
  
     treadmills.param.speed_step = DEFAULT_SPEED_STEP;//50;
 
     treadmills.param.speed_max = DEFAULT_SPEED_MAX;//600;

     treadmills.param.voltage_max = DEFAULT_VOLTAGE_MAX;//90;

     treadmills.param.voltage_min = DEFAULT_VOLTAGE_MIN;//15;

     treadmills.param.over_current_max = DEFAULT_OVER_CURRENT_MAX;//120;
 
     treadmills.param.kiv[0] = DEFAULT_SET_KIV_1KM_VALUE;//20;
     treadmills.param.kiv[1] = DEFAULT_SET_KIV_2KM_VALUE;//20;
     treadmills.param.kiv[2] = DEFAULT_SET_KIV_3KM_VALUE;//15;
     treadmills.param.kiv[3] = DEFAULT_SET_KIV_4KM_VALUE;//10;
     treadmills.param.kiv[4] = DEFAULT_SET_KIV_5KM_VALUE;//8;
     treadmills.param.kiv[5] = DEFAULT_SET_KIV_6KM_VALUE;//8;
     treadmills.param.kiv[6] = DEFAULT_SET_KIV_7KM_VALUE;//6;
     treadmills.param.kiv[7] = DEFAULT_SET_KIV_8KM_VALUE;//6;
     treadmills.param.kiv[8] = DEFAULT_SET_KIV_9KM_VALUE;//6;
     treadmills.param.kiv[9] = DEFAULT_SET_KIV_10KM_VALUE;//6;
     treadmills.param.kiv[10] = DEFAULT_SET_KIV_11KM_VALUE;//6;
     treadmills.param.kiv[11] = DEFAULT_SET_KIV_12KM_VALUE;//6;

  
  //****************上电就发送*************
  
    for(i = 0; i< 100;i++)
  {
    NOP_15();
  }

  uart_frame_tx(CMD_TREADMILLS_SPEED_MAX,treadmills.param.speed_max/10);  
  uart_frame_tx(CMD_TREADMILLS_SPEED_MAX,treadmills.param.speed_max/10);  
  uart_frame_tx(CMD_VOLTAGE_MAX,treadmills.param.voltage_max);
  uart_frame_tx(CMD_VOLTAGE_MIN,treadmills.param.voltage_min); 
  uart_frame_tx(CMD_OVER_CURRENT_MAX,treadmills.param.over_current_max);  

  for( i = 16; i < 28; i++)        //16 = CMD_KIV_1KM， 27 = CMD_KIV_12KM;
  {
  uart_frame_tx(i ,treadmills.param.kiv[i-16]); 
  }
  
}



/************************************************************************************************************
*    Main function
************************************************************************************************************/
void main (void)
{
  u8 i = 0;
  static u8 w_cnt = 0;
  static u8 send_cnt = 0;

	NOP_15();
	MODIFY_HIRC_166();
	Set_All_GPIO_Quasi_Mode;

	/*
	 * 准双向:     PxM1=0 PxM2=0
	 * 推挽:       PxM1=0 PxM2=1
	 * 高阻抗输入:  PxM1=1 PxM2=0
	 * 开漏:       PxM1=1 PxM2=1
	 *
	 */
	P0M1 =(1<<0) |(1<<1) |(0<<2) |(1<<3) |(0<<4) |(0<<5) |(0<<6) |(0<<7);
	P0M2 =(0<<0) |(0<<1) |(0<<2) |(0<<3) |(0<<4) |(1<<5) |(0<<6) |(0<<7);

	P1M1 =(1<<0) |(0<<1) |(0<<2) |(0<<3) |(0<<4) |(0<<5) |(0<<6) |(0<<7);
	P1M2 =(0<<0) |(0<<1) |(0<<2) |(0<<3) |(1<<4) |(0<<5) |(0<<6) |(0<<7);

//	P2M1 =(1<<0) |(1<<1) |(1<<2) |(1<<3) |(1<<4) |(1<<5) |(1<<6) |(1<<7);
//	P2M2 =(1<<0) |(1<<1) |(1<<2) |(1<<3) |(1<<4) |(1<<5) |(1<<6) |(1<<7);
//
//
//	P3M1 =(1<<0) |(1<<1) |(1<<2) |(1<<3) |(1<<4) |(1<<5) |(1<<6) |(1<<7);
//	P3M2 =(1<<0) |(1<<1) |(1<<2) |(1<<3) |(1<<4) |(1<<5) |(1<<6) |(1<<7);

	key_mask=0x00;//禁止所有按键

	led_display_init();
	aip1628_init();
	aip1628_init();
	aip1628_init();
	aip1628_init();
	aip1628_init();
	pwm_init();
	uart0_init();
	Timer0_Init();
  uart_frame_init();

  treadmills_init();
  param_init();

  treadmills.status=STATUS_POWERON;
  
  
  for(i = 0;i<50;i++)
  {
   NOP_15();
  }
  read_beep_status();
  
  poweron_send_flag = 1;
    
    
	set_EA;                            //开总中断

	BEEP_SWITCH_ON_1_CNT(); //2023.3.27

    while(1)
    {
    	/**
    	 * 10ms 周期处理
    	 */
    	uart_frame_loop();
    	if(ms10_flag)
    	{
    		ms10_flag=0;
    		KeyPad_Scan();
    		beep_sound_proc();
    		boot_time();
    	}
      
    	/**
    	 * 100ms 周期处理程序
    	 */
        if(ms100_flag)
        {
        	ms100_flag=0;
        	seg_disp_proc();
        	treadmills_disp_loop();
        	treadmills_proc_loop();
        }

        /**
         * 500ms 周期处理程序
         */
        if(ms500_flag)
        {
        	ms500_flag=0;
//        LED1=!LED1;//指示灯闪烁
          communication_checkout();
          
          if(poweron_send_flag == 1)
          {
            send_cnt++;
            if( send_cnt >= 2)
            {
              send_default_param();
              send_cnt = 0;
              poweron_send_flag = 0;              
            }          
          } 
          
        }

        /**
         * 1秒周期 处理程序
         */
        if(ms1000_flag)
        {
        	ms1000_flag=0;
        	treadmills_param_calc();

          if(write_flag)
          {
            if(++w_cnt >=3)
            {
              save_param();
              w_cnt = 0;
              write_flag = 0;
            }    
          }
          else
          {
            w_cnt = 0;
          }
          
        }

        /**
         * led update
         */
    	disp_update();
    }
}
