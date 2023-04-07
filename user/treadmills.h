/*
 * treadmills.h
 *
 *  Created on: 2019��12��12��
 *      Author: Administrator
 */

#ifndef USER_TREADMILLS_H_
#define USER_TREADMILLS_H_
#include "common.h"
#define LED1  (P05)


#define VOLTAGE_MAX  (90.0)//(180.0) //����DEFAULT_VOLTAGE_MAX  90
#define VOLTAGE_MIN  (17.0)//(30.0)  //����DEFAULT_VOLTAGE_MIN 17
#define SPEED_SIZE   (60.0)          //����(DEFAULT_SPEED_MAX/10), DEFAULT_SPEED_MAX 600
#define GET_SPEED_VOLTAGE(speed) (float)((((VOLTAGE_MAX-VOLTAGE_MIN)/(SPEED_SIZE-10))*(speed-10))+VOLTAGE_MIN)
#define BASE_VOLTAGE   (17.0)//(30) /*��ֹ�޸�,����¿ز���*/ //����DEFAULT_VOLTAGE_MIN 17
#define BASE_VOLTAGE_K  (float)((VOLTAGE_MAX-VOLTAGE_MIN)/(DEFAULT_SPEED_MAX - 100) ) //(0.136) /*��ֹ�޸�,������¿ز���*/
#define BASE_SPEED     (100) /*��ֹ�޸�,������¿ز���*/
#define MAP_VOLTAGE_2_SPEED(voltage) (((voltage-BASE_VOLTAGE)/BASE_VOLTAGE_K)+BASE_SPEED)

#define STOP_DISPLAY_INTERVAL (40)//base time:10ms ֹͣ��ʱ�򵹼�ʱ�ٶȣ����ִ󵹼�ʱ��
#define STOP_WAIT_TIMEOUT     (250) //BASE TIME:100ms

#define KIV_NUM        12


/*
һ�������ź�˵����
er01 û��ͨѶ�����°�֮��û�н���ͨѶ
er02 ���ʹܶ�·��������ߵ�
er03 �����ת�������һ��ʱ����ֹͣת����
er04 ���̵���ճ��
er05 ������������ֵ��������ع���
er06 �ٶȳ����趨ֵ���ɳ���
er07 �¿������ź��߿�·������Ϊ�¿������ź��ߺ㶨Ϊ�ߵ�ƽ
����er02---er06�����°����������źŴ�����er01��er07�����ϰ��Լ���ⴥ��
ͨѶЭ�飺�ӿ��������°�֮����벻��ϵ�ͨѶ��ȷ��ͨѶ��������һ����ʱ������ͨѶ��������ô�°�ͣ�����ϰ屨er01���������ʱ�����ó�5�����ϣ��Լ�ǿ�ݴ�
*/
typedef enum _error_code_enum
{
	ERROR_COMM=1,//er01 û��ͨѶ�����°�֮��û�н���ͨѶ
	ERROR_MOS_SHORT,//er02 ���ʹܶ�·��������ߵ�
	ERROR_MT_STALL,//er03 �����ת�������һ��ʱ����ֹͣת����
	ERROR_RELAY,//er04 ���̵���ճ��
	ERROR_OVER_CURRENT,//er05 ������������ֵ��������ع���
	ERROR_OVER_SPEED,//er06 �ٶȳ����趨ֵ���ɳ���
}error_code_enum;

typedef enum _status_e
{
	STATUS_POWERON=1,
	STATUS_WAIT,

	STATUS_RUN,
	STATUS_RUNNING,

	STATUS_STOP,
	STATUS_STOP_WAIT,
	STATUS_STOP_OVER,


	STATUS_ERROR,

	DISP_MODE_AUTO,
	DISP_MODE_TO_AUTO,
	DISP_MODE_SINGLE,
}status_e;



typedef struct _t_treadmills
{
	u8 ack;
	u8 error_code;
	u8 status;
	u8 basetime;
	struct
	{
    u8 beep_status; //2023.3.27
		u16 speed;
		u32 second;
		float distance;
		u32 calorie;
    
    u8 speed_step;    
    u16 speed_max;
    u8 voltage_max;
    u8 voltage_min;
    u8 over_current_max;
    volatile u16 kiv[KIV_NUM];                
	}param;

	struct
	{
		u8 index;
	  u8 mode;
	}display;

}treadmills_t;


extern treadmills_t treadmills;
extern void treadmills_init(void);
extern void treadmills_proc_loop(void);
extern void treadmills_disp_loop(void);
extern void communication_checkout(void);
extern void treadmills_param_calc(void);
extern void boot_time(void);
extern void treadmills_display (index);
extern u8 map_speed(u8 speed);
#endif /* USER_TREADMILLS_H_ */
