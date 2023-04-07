/*
 * keypad.c
 *
 *  Created on: 2018��2��4��
 *      Author: Administrator
 */
#include "keypad.h"
#include "keypad_fun.h"



/*
 * keyboard.c
 *
 *  Created on: 2017��4��28��
 *      Author: Administrator
 */

/**
 * �����̳���ʵ�����¹���
 * ���ܣ�
 * 1.���ײ�������APP����(keypad_func.c����Ӿ���ʵ�ֵĹ���)����������ԣ�������ֲ��
 * 2.�ܹ�ʶ�𵥼�����ϼ������������ж��塣
 * 3.�ܹ���⵱ǰ���¼���������
 * 4.ʵ�� ��/��/�������ܣ�����Ϣѭ��������Ӧ�������ܡ�
 * 5.����ʱ�䳤�̣������ٶȿ�������ͨ���궨�����ã��̰�������������ֻ��ϵͳ��Ӧ�йء�
 * 6.����������ð��������ι��ܣ��Լ�����(���������)���ι��ܡ�
 * 7.�Զ�ʵ�ַ��������ܣ�Ӳ������ӿڼ򵥡�
 * �������ã�
 * #define  LONG_PRESS_FUNC_EN        (0)//ʵ�ֳ�����
 * #define  CONTINUE_PRESS_FUNC_EN    (0)//ʵ���������ܣ����ǳ����궨������
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
		 * �����������,��Ӳ���й�ϵ
		 */
		//=================================

	new=0;
	if(IS_PRESS_KEY1()){new |=(1<<0);}
	if(IS_PRESS_KEY2()){new |=(1<<1);}
	if(IS_PRESS_KEY3()){new |=(1<<2);}
	if(IS_PRESS_KEY4()){new |=(1<<3);}

    switch(state)
    {
       case 0://ȥ������
		if(new !=old)//�п����Ƕ���
		{
			cnt=0;
		}
		else//�������ȶ�ֵ
		{
			cnt++;
			if(cnt>2)//ȷ�����ȶ�ֵ
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

    	    if(trig)//��ʾ�а����Ĵ���(press/realse)
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
    	    else//��ʾ���������п����� �����ͷ�
    	    {
    	    	cont_cnt++;

    	    	if(flag==0)//��ʾ����
    	    	{
					if(cont_cnt>50)
					{
						flag=1;
						cont_cnt=0;
						keypad_long_proc();
					}
    	    	}
    	    	else//��ʾ����
    	    	{
    	    		if(cont_cnt>20)
					{
						cont_cnt=0;
						keypad_continue_proc();
					}
    	    	}
    	    }

     	   //2.�ж��Ƿ����µļ�ֵ
     	   if(new!=key){old=new;state=0;}
    	   break;
       default:
    	   break;
    }

}


