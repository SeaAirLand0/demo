/*
 * AIP1628.c
 *
 *  Created on: 2018��8��1��
 *      Author: Administrator
 */
#include "aip1628.h"

#include "common.h"
#include "7_segment.h"



/**
 * Ӳ���ӿ����Ӷ���
 */
#define TM_CS   (P13)
#define TM_DIO  (P11)
#define TM_CLK  (P12)


/**
 * CS:�͵�ƽ��Ч
 * CLK:��������Ч�������͵�ƽʱ����Ҫ�ȶ����ݡ�
 */
static void wt_byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		TM_CLK=0;
		TM_DIO = (dat &0x01);
		dat>>=1;
		TM_CLK=1;

	}
}




void aip1628_write_byte(u8 addr,u8 dat)
{
	TM_CS=0;
	wt_byte(ADDR_CMD|addr);
	wt_byte(dat);
//	wt_byte((u8)(dat>>8));
	TM_CS=1;

}


void aip1628_write_nbytes(u8 addr,u8 *p, u8 len)
{
	   TM_CS=0;
		wt_byte(ADDR_CMD|addr);
		for(;len>0;len--)
		{
			wt_byte(*p);
			p++;
		}
		TM_CS=1;
}


void set_aip1628_lighte_scale(u8 scale)
{
	//����ʾ
	TM_CS=0;
	wt_byte(DISP_CTR_CMD |DISP_ON |scale);
	TM_CS=1;
}



void aip1628_init(void)
{

	//���� ��ʾģʽ��5λ9��
	TM_CS=0;
	wt_byte(DISP_MODLE_CMD |B5S9);
	TM_CS=1;

	//����ģʽ
	TM_CS=0;
	wt_byte(DATA_CMD|WT_DISP_REG |ADDR_AUTO_INC);
	TM_CS=1;


	set_aip1628_lighte_scale(EXTINCTION_SCALE);
}





