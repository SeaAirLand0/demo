/*
 * uart_frame.h
 *
 *  Created on: 2019��12��11��
 *      Author: Administrator
 */

#ifndef USER_UART_FRAME_H_
#define USER_UART_FRAME_H_
#include "common.h"
#include "queue.h"

#define RX_BUFF_SIZE (300)
#define START_OF_FRAME  (0X7F)
#define END_OF_FRAME    (0X7E)

#define STATUS_MOTOR_RUN (1)
#define STATUS_MOTOR_STOP (0)


typedef enum _cmd_e
{
	CMD_STATUS=1,
	CMD_SPEED,
	CMD_ACK,
	CMD_ERROR,
	CMD_RESET,
	CMD_STOP_OVER,
	CMD_WRITE_PARAM,
	CMD_READ_PARAM,
	CMD_TEST,
	CMD_STATUS_INQUIRE,
	CMD_STATUS_URGENT_STOP,
  
  CMD_TREADMILLS_SPEED_MAX,   // 12 �ܲ�������ٶ� 0x0C
  CMD_VOLTAGE_MAX,            //13 ����ѹ  0x0D
  CMD_VOLTAGE_MIN,            //14 ��С��ѹ   0x0E
  CMD_OVER_CURRENT_MAX,       //15 ������ֵ  0x0F
  CMD_KIV_1KM,                    //16  KIV����   0x10 
  CMD_KIV_2KM,                    //17  KIV����   0x11 
  CMD_KIV_3KM,                    //18  KIV����   0x12 
  CMD_KIV_4KM,                    //19  KIV����   0x13 
  CMD_KIV_5KM,                    //20  KIV����   0x14 
  CMD_KIV_6KM,                    //21  KIV����   0x15 
  CMD_KIV_7KM,                    //22  KIV����   0x16 
  CMD_KIV_8KM,                    //23  KIV����   0x17 
  CMD_KIV_9KM,                    //24  KIV����   0x18 
  CMD_KIV_10KM,                   //25  KIV����   0x19 
  CMD_KIV_11KM,                   //26  KIV����   0x1A 
  CMD_KIV_12KM,                   //27  KIV����   0x1B 
	
	CMD_STATUS_HEART               //28
  
}cmd_e;



#define DATA_BUF_SIZE  (2)
typedef struct _t_uart_frame
{
	u8 sof;
	u8 cmd;
	u8 len;
	u8 buf[DATA_BUF_SIZE];
	u8 crch;
	u8 crcl;
	u8 eof;
	u8 is_ok;
}uart_frame_t;


extern T_QUEUE rx_queue;
extern uart_frame_t uart_frame;
extern uart_frame_t uart_tx_frame;
extern void uart_frame_init(void);
extern void uart_frame_loop(void);
extern void uart_frame_tx(u8 cmd,u8 dat);
extern void uart_frame_tx_2(u8 cmd,u8 dat0,u8 dat1);
extern void uart0_init(void);


#endif /* USER_UART_FRAME_H_ */
