/*
 * param.h
 *
 *  Created on: 2020��3��8��
 *      Author: Administrator
 */

#ifndef BSP_PARAM_H_
#define BSP_PARAM_H_
#include "common.h"
#include "iap_eeprom.h"



#define PARAM_SOF (0xA5)

#define EEPROM_BYTES   2 //31   //2023.3.24
/*
typedef struct _ee_param_t
{
	u8 sof;
	s8 val;
	s8 not_val;

}ee_param_t;
*/
typedef struct _ee_param_t
{
  u8 sof;                      //��ʼ�ֽ�
  u8 beep_status;              //2023.3.24
//  u8 treadmills_speed_step;    //�ܲ�������     //1�ֽ�
//  u16 treadmills_speed_max;    //�ܲ�������ٶ� //2�ֽ�
//  u8 voltage_max;             //����ѹ       //2�ֽ�  //u16
//  u8 voltage_min;             //��С��ѹ       //2�ֽ�  //u16
//  u8 over_current_max;        //������ֵ �Ŵ���    // 2�ֽ�  //
//  volatile u16 kiv[KIV_NUM];            //KIV����     //u16->u8 2023.2.17

}ee_param_t;




extern ee_param_t ee_param;
extern void read_param(void);//u8 read_param(void);
extern void write_param(void);//(s8 val);
extern void ee_param_init(void);

#endif /* BSP_PARAM_H_ */
