/*
 * keypad.c
 *
 *  Created on: 2018年2月4日
 *      Author: Administrator
 */
#include "keypad.h"
#include "keypad_fun.h"



/*
 * keyboard.c
 *
 *  Created on: 2017年4月28日
 *      Author: Administrator
 */

/**
 * 本键盘程序实现以下功能
 * 功能：
 * 1.将底层驱动和APP分离(keypad_func.c中添加具体实现的功能)，减少耦合性，方便移植。
 * 2.能够识别单键或组合键，键功能自行定义。
 * 3.能够检测当前按下键的数量。
 * 4.实现 短/长/连击功能，以消息循环自行响应按键功能。
 * 5.长按时间长短，连击速度快慢可以通过宏定义配置，短按立即触发快慢只和系统响应有关。
 * 6.具有软件设置按键的屏蔽功能，以及连击(可软件屏蔽)屏蔽功能。
 * 7.自动实现防抖动功能，硬件输入接口简单。
 * 功能配置：
 * #define  LONG_PRESS_FUNC_EN        (0)//实现长功能
 * #define  CONTINUE_PRESS_FUNC_EN    (0)//实现连击功能，但是长按宏定义必须打开
 *
 */



#define KEY1 (P01)
#define KEY2 (P00)
#define KEY3 (P10)
#define KEY4 (P03)


#define IS_PRESS_KEY1()  (KEY1==0)
#define IS_PRESS_KEY2()  (KEY2==0)
#define IS_PRESS_KEY3()  (KEY3==0)
#define IS_PRESS_KEY4()  (KEY4==0)




extern  void keypad_short_proc(void);
extern  void keypad_long_proc(void);
extern  void keypad_continue_proc(void);
extern  void keypad_all_realse_proc(void);



static u8 trig=0;
static u8 cont=0;

static u8 cnt=0,state=0;
static u8 new ,old;

static u8 cont_cnt=0;
static u8 flag=0;

u8 key=0;
u8 key_cnt=0x00;
u8 key_mask=0x00;

u8 get_key_status(u8 btn)
{
	u8 status=0;

	if(trig & btn)
	{
		if(cont & btn){status= KEY_PRESS;}
		else{status= KEY_RELEASE;}

	}
	return status;
}

void KeyPad_Scan(void)
{

	    /**
		 * 按键输入入口,和硬件有关系
		 */
		//=================================

	new=0;
	if(IS_PRESS_KEY1()){new |=(1<<0);}
	if(IS_PRESS_KEY2()){new |=(1<<1);}
	if(IS_PRESS_KEY3()){new |=(1<<2);}
	if(IS_PRESS_KEY4()){new |=(1<<3);}

    switch(state)
    {
       case 0://去除抖动
		if(new !=old)//有可能是抖动
		{
			cnt=0;
		}
		else//可能是稳定值
		{
			cnt++;
			if(cnt>2)//确定是稳定值
			{
				state=1;
				key=new;
				cnt=0;
			}
		}
		old=new;
		break;

       case 1:
    		trig=(key ^ cont);
    	    cont=key;

    	    key_cnt=0;
    	    if(key &(1<<0)){key_cnt++;}
    	    if(key &(1<<1)){key_cnt++;}
    	    if(key &(1<<2)){key_cnt++;}
    	    if(key &(1<<3)){key_cnt++;}

    	    if(trig)//表示有按键的触发(press/realse)
    	    {
    	    	cont_cnt=0;
    	    	flag=0;
    	    	if(key_cnt)
    	    	{
    	    		keypad_short_proc();
    	    	}
    	    	else
    	    	{
    	    		keypad_all_realse_proc();
    	    	}
    	    }
    	    else//表示连续按键有可能是 连续释放
    	    {
    	    	cont_cnt++;

    	    	if(flag==0)//表示长按
    	    	{
					if(cont_cnt>50)
					{
						flag=1;
						cont_cnt=0;
						keypad_long_proc();
					}
    	    	}
    	    	else//表示连击
    	    	{
    	    		if(cont_cnt>20)
					{
						cont_cnt=0;
						keypad_continue_proc();
					}
    	    	}
    	    }

     	   //2.判断是否有新的键值
     	   if(new!=key){old=new;state=0;}
    	   break;
       default:
    	   break;
    }

}


