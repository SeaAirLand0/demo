/*
 * treadmills.h
 *
 *  Created on: 2019年12月12日
 *      Author: Administrator
 */

#ifndef USER_TREADMILLS_H_
#define USER_TREADMILLS_H_
#include "common.h"
#define LED1  (P05)


#define VOLTAGE_MAX  (90.0)//(180.0) //等于DEFAULT_VOLTAGE_MAX  90
#define VOLTAGE_MIN  (17.0)//(30.0)  //等于DEFAULT_VOLTAGE_MIN 17
#define SPEED_SIZE   (60.0)          //等于(DEFAULT_SPEED_MAX/10), DEFAULT_SPEED_MAX 600
#define GET_SPEED_VOLTAGE(speed) (float)((((VOLTAGE_MAX-VOLTAGE_MIN)/(SPEED_SIZE-10))*(speed-10))+VOLTAGE_MIN)
#define BASE_VOLTAGE   (17.0)//(30) /*禁止修改,这个下控参数*/ //等于DEFAULT_VOLTAGE_MIN 17
#define BASE_VOLTAGE_K  (float)((VOLTAGE_MAX-VOLTAGE_MIN)/(DEFAULT_SPEED_MAX - 100) ) //(0.136) /*禁止修改,这个是下控参数*/
#define BASE_SPEED     (100) /*禁止修改,这个是下控参数*/
#define MAP_VOLTAGE_2_SPEED(voltage) (((voltage-BASE_VOLTAGE)/BASE_VOLTAGE_K)+BASE_SPEED)

#define STOP_DISPLAY_INTERVAL (40)//base time:10ms 停止的时候倒计时速度，数字大倒计时慢
#define STOP_WAIT_TIMEOUT     (250) //BASE TIME:100ms

#define KIV_NUM        12


/*
一、错误信号说明：
er01 没有通讯，上下板之间没有建立通讯
er02 功率管短路，电机断线等
er03 电机堵转（电机在一段时间内停止转动）
er04 主继电器粘连
er05 电流超过保护值（电机负载过大）
er06 速度超过设定值，飞车等
er07 下控上送信号线开路，现象为下控上送信号线恒定为高电平
其中er02---er06是由下板送上来的信号触发，er01和er07是由上板自己检测触发
通讯协议：从开机起上下板之间必须不间断的通讯，确保通讯正常，在一定的时间里无通讯建立，那么下板停机，上板报er01。建议这个时间设置成5秒以上，以加强容错，
*/
typedef enum _error_code_enum
{
	ERROR_COMM=1,//er01 没有通讯，上下板之间没有建立通讯
	ERROR_MOS_SHORT,//er02 功率管短路，电机断线等
	ERROR_MT_STALL,//er03 电机堵转（电机在一段时间内停止转动）
	ERROR_RELAY,//er04 主继电器粘连
	ERROR_OVER_CURRENT,//er05 电流超过保护值（电机负载过大）
	ERROR_OVER_SPEED,//er06 速度超过设定值，飞车等
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
