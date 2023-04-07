#include "common.h"
#include "uart.h"




unsigned char Receive_Data(unsigned char UARTPort)
{
    UINT8 c;
    switch (UARTPort)
    {
      case UART0:
        while (!RI);
        c = SBUF;
        RI = 0;
      break;
      case UART1:
        while (!RI_1);
        c = SBUF_1;
        RI_1 = 0;
      break;
    }
    return (c);
}

void UART_Send_Data(UINT8 UARTPort, UINT8 c)
{
    switch (UARTPort)
    {
      case UART0:
        TI = 0;
        SBUF = c;
        while(TI==0);
      break;
      case UART1:
        TI_1 = 0;
        SBUF_1 = c;
        while(TI_1==0);
      break;
    }
}


void UART_Send_Value(UINT8 UARTPort, UINT16 val)
{
	UART_Send_Data(UARTPort,(val/10000)+0x30);
	UART_Send_Data(UARTPort,((val%10000)/1000)+0x30);
	UART_Send_Data(UARTPort,((val%1000)/100)+0x30);
	UART_Send_Data(UARTPort,((val%100)/10)+0x30);
	UART_Send_Data(UARTPort,(val%10)+0x30);
}






void UART_Send_String(UINT8 UARTPort, UINT8 *ptr)
{
	while(*ptr)
	{
		UART_Send_Data(UARTPort,*ptr);
		ptr++;
	}
}


void UART_Send_Buf(UINT8 UARTPort, UINT8 *ptr,UINT8 len)
{
	for(;len>0;len--)
	{
		UART_Send_Data(UARTPort,*ptr);
		ptr++;
	}
}



