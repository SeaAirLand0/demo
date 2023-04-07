/*
 * tm1668.h
 *
 *  Created on: 2018��8��1��
 *      Author: Administrator
 */

#ifndef __aip1628__HH
#define __aip1628__HH
#include "common.h"




/**
 * ��ʼ��ָ��
 */
#define DISP_MODLE_CMD (0X00)
	#define B4S13 (0X00)/*4λ13��*/
	#define B5S12 (0X01)/*5λ12��*/
	#define B6S11 (0X02)/*6λ11��*/
	#define B7S10 (0X03)/*7λ10��*/


#define B5S9 (0X01)/*5λ9��*/

#define DATA_CMD       (0X40)
	#define WT_DISP_REG	    (0X00)
	#define RD_KEY_REG      (0X02)
	#define ADDR_AUTO_INC	(0X00)
	#define ADDR_FIX	    (0X04)

#define DISP_CTR_CMD   (0X80)
	#define EXTINCTION_SCALE (7) /*0~7*/
	#define DISP_ON  (0X08)
	#define DISP_OFF (0X00)

#define ADDR_CMD       (0XC0)






/**
 * ���� ��ʾ�Ĵ�����ַ��
�üĴ����洢ͨ�����нӿڽ��մ��ⲿ�������͵�TM1668������,�����Ч��ַ��C0H-CDH��14�ֽڵ�Ԫ,
�ֱ���оƬSGE��GRID�ܽŶ�Ӧ,���������ͼ(2)��
дLED��ʾ���ݵ�ʱ��,������ʾ��ַ�ӵ�λ����λ,�����ֽڴӵ�λ����λ������
SEG1
SEG2
SEG3
SEG4
SEG5
SEG6
SEG7
SEG8
SEG9
SEG10
X
SEG12
SEG13
SEG14
X X
xxHL������λ�� xxHU(����λ) xxHL(����λ) xxHU����λ��
B0 B1 B2 B3 B4 B5 B6 B7 B0 B1 B2 B3 B4 B5 B6 B7
C0HL C0HU C1HL C1HU GRID1
C2HL C2HU C3HL C3HU GRID2
C4HL C4HU C5HL C5HU GRID3
C6HL C6HU C7HL C7HU GRID4
C8HL C8HU C9HL C9HU GRID5
CAHL CAHU CBHL CBHU GRID6
CCHL CCHU CDHL CDHU GRID7

 оƬ��ʾ�Ĵ������ϵ�˲�����ڲ������ֵ�����������ȷ����,��ʱ�ͻ�ֱ�ӷ��Ϳ�������,
���п��ܳ�����ʾ���롣������˾����ͻ�����ʾ�Ĵ�������һ���ϵ��������,���ϵ����14λ�Դ��ַ
��C0H-CDH����ȫ��д������0x00��
 */


extern void set_aip1628_lighte_scale(u8 scale);
extern void aip1628_write_byte(u8 addr,u8 dat);
extern void aip1628_init(void);
extern void aip1628_write_nbytes(u8 addr,u8 *p, u8 len);

#endif /* SOFTWARE_DRIVER_TM1688_H_ */
