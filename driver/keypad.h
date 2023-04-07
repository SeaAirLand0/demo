/*
 * keypad.h
 *
 *  Created on: 2018��2��4��
 *      Author: Administrator
 */

#ifndef SOFTWARE_USER_KEYPAD_H_
#define SOFTWARE_USER_KEYPAD_H_
#include "common.h"


#define KEY_PRESS (1)
#define KEY_RELEASE (2)


#define KEY_MSG_SHORTPRESS    (0X30)
#define KEY_MSG_LONGPRESS     (0X31)
#define KEY_MSG_CONTINUEPRESS (0x32)

#define KEY_MSG_ALL_REALSE    (0x33)




/**
 * �������������ã���λ10ms
 */
#define    LONG_TICKS_MAX     (40) //����ʶ��ʱ��
#define    CONTINUE_TICKS_MAX (80)//����ʶ��ʱ�� LONG_TICKS_MAX+(150)
#define    CONTINUE_TICKS     (20)//�������ʱ�䣬���ٶȿ���






extern u8 key_cnt;
extern u8 key;
extern u8 key_mask;

extern u8 get_key_status(u8 btn);
extern void KeyPad_Scan(void);


#endif /* SOFTWARE_USER_KEYPAD_H_ */
