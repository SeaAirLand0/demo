/*
 * param.c
 *
 *  Created on: 2020年3月8日
 *      Author: Administrator
 */

#include "common.h"
#include "ee_param.h"
#include "uart.h"

ee_param_t ee_param;
//ee_param_t ee_param;

extern void UART_Send_String(UINT8 UARTPort, UINT8 *ptr);
//extern void UART_Send_Buf(UINT8 UARTPort, UINT8 *ptr,UINT8 len);
extern void UART_Send_Data(UINT8 UARTPort, UINT8 c);

void ee_param_init(void)
{
  ee_param.sof=PARAM_SOF;

}

/***************************************************************************************
*@brief: 读函数
*@param: 无
*@ret:   读取成功返回：0,
         读取失败返回：1
****************************************************************************************/
//u8 read_param(void)
void read_param(void)
{
  u8 i;
  
  u8 buff[EEPROM_BYTES];
  ee_param_t *ee_param_ptr;
  
  for(i =0;i< EEPROM_BYTES;i++)
  {
    buff[i] = 0;
  }
  
  read_aprom_page(PAGE0,(UINT8*)&buff[0],EEPROM_BYTES);//34
  ee_param_ptr = (ee_param_t*)&buff[0];
  
  if(ee_param_ptr->sof == PARAM_SOF )   //0xA5
  {
    ee_param.beep_status = ee_param_ptr->beep_status;
  }
  else
  {
    //调试用
//     EA = 0;
      debug_printf_s("\r\n read err\n");
      debug_printf_de((u16)ee_param_ptr->sof );
//     EA = 1;
//    return 1;  //读取失败返回1
  }


}


void write_param(void)//(s8 val)
{
	erase_aprom_page(PAGE0);

  //调试用
  ee_param.sof = PARAM_SOF;
  
	write_aprom_page(PAGE0,(UINT8*)&ee_param,EEPROM_BYTES); //34
  
  debug_printf_s("\r\nee_param_ptr->sof = ");
  debug_printf_de((u16)buff[0] );

}
