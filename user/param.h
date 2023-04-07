/*
 * param.h
 *
 *  Created on: 2020��3��9��
 *      Author: Administrator
 */

#ifndef USER_PARAM_H_
#define USER_PARAM_H_
#include "common.h"

#define SPEED_ADC_MAX  (5)
#define SPEED_ADC_MIN  (-5)

#define TIME_MAX       (3600)
#define TIME_MIN       (0)

#define DISTANCE_MAX  (99000)
#define DISTANCE_MIN  (0)

#define CALORIE_MAX  (99000)
#define CALORIE_MIN  (0)




#define PARAM_SET_SPEED_ADC  (0)

//#define PARAM_SET_TIME       (1)
//#define PARAM_SET_DISTANCE   (2)
//#define PARAM_SET_CALORIE    (3)

#define PARAM_SET_BEEP       (1) //�ٶ�ģʽ   //2023.3.27
#define PARAM_SET_TIME       (2) 
#define PARAM_SET_DISTANCE   (3) 
#define PARAM_SET_CALORIE    (4) 


//���ò����ĳ�ʼֵ
#define DEFAULT_SPEED_STEP             (10)    //����50 = 0.5km/h 
#define DEFAULT_SPEED_MAX              (600)   //600 = 6km //�˴��޸ļǵú�treadmills.h�������ٶ�һͬ�޸�
#define DEFAULT_VOLTAGE_MAX            (90)                //�˴��޸ļǵú�treadmills.h��ĵ�ѹһͬ�޸�
#define DEFAULT_VOLTAGE_MIN            (17)    //15        //�˴��޸ļǵú�treadmills.h��ĵ�ѹһͬ�޸�
#define DEFAULT_OVER_CURRENT_MAX       (120)


#define DEFAULT_SET_KIV_1KM_VALUE    (20) // (20)
#define DEFAULT_SET_KIV_2KM_VALUE    (50) // (20)
#define DEFAULT_SET_KIV_3KM_VALUE    (18) // (15) 
#define DEFAULT_SET_KIV_4KM_VALUE    (50) // (10)
#define DEFAULT_SET_KIV_5KM_VALUE    (11) // (8) 
#define DEFAULT_SET_KIV_6KM_VALUE    (11) // (8)
#define DEFAULT_SET_KIV_7KM_VALUE    (16) // (6)
#define DEFAULT_SET_KIV_8KM_VALUE    (16) // (6)
#define DEFAULT_SET_KIV_9KM_VALUE    (16) // (6)
#define DEFAULT_SET_KIV_10KM_VALUE   (16) // (6)
#define DEFAULT_SET_KIV_11KM_VALUE   (16) // (6)
#define DEFAULT_SET_KIV_12KM_VALUE   (16) // (6)



typedef struct _param_t
{
	u8 en;
	u8 index;
	s8 speed_adc;//�û���Ҫ��׼��λ �ٶȲ���
	u8 book_en;
}param_t;

extern param_t param;
extern void param_init(void);

#endif /* USER_PARAM_H_ */
