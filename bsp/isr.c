/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2019 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Jan/21/2019
//***********************************************************************************************************

//***********************************************************************************************************
//  File Function: MS51 ADC / Bandgap input demo code
//***********************************************************************************************************

#include "common.h"
#include "uart_frame.h"
#include "queue.h"
#include "treadmills.h"

#if(0)
/*all interrupt subroutine list */
//-----------------------------------------------------------------------------------------------------------
void INT0_ISR(void) interrupt 0          // Vector @  0x03
{
    clr_TCON_IE0;
}
//-----------------------------------------------------------------------------------------------------------
void Timer0_ISR(void) interrupt 1        // Vector @  0x0B
{
    clr_TCON_TF0;
		TH0 = (TIMER_DIV12_VALUE_1ms)/256;
	 TL0 = (TIMER_DIV12_VALUE_1ms)%256;
}
//-----------------------------------------------------------------------------------------------------------
void INT1_ISR(void) interrupt 2          // Vector @  0x13
{
    clr_TCON_IE1;
}
//-----------------------------------------------------------------------------------------------------------
void Timer1_ISR(void) interrupt 3        // Vector @  0x1B
{
    clr_TCON_TF1;
}
//-----------------------------------------------------------------------------------------------------------
void UART0_ISR(void) interrupt 4         // Vector @  0x23
{
    clr_SCON_RI;
		clr_SCON_TI;
}
//-----------------------------------------------------------------------------------------------------------
void Timer2_ISR(void) interrupt 5        // Vector @  0x2B
{
    clr_T2CON_TF2;
}
//-----------------------------------------------------------------------------------------------------------
void I2C_ISR(void) interrupt 6           // Vector @  0x33
{
    clr_I2CON_SI;
}
//-----------------------------------------------------------------------------------------------------------
void Pin_INT_ISR(void) interrupt 7       // Vector @  0x3B
{
    PIF = 0;
}
//-----------------------------------------------------------------------------------------------------------
void BOD_ISR(void) interrupt 8           // Vector @  0x43
{
    clr_BODCON0_BOF;
}
//-----------------------------------------------------------------------------------------------------------
void SPI_ISR(void) interrupt 9           // Vector @  0x4B
{
    clr_SPSR_SPIF;
}
//-----------------------------------------------------------------------------------------------------------
void WDT_ISR(void) interrupt 10          // Vector @  0x53
{
    clr_WDCON_WDTF;
}
//-----------------------------------------------------------------------------------------------------------
void ADC_ISR(void) interrupt 11          // Vector @  0x5B
{
    clr_ADCCON0_ADCF;
}
//-----------------------------------------------------------------------------------------------------------
void Capture_ISR(void) interrupt 12      // Vector @  0x63
{
    clr_CAPCON0_CAPF0;
		clr_CAPCON0_CAPF1;
		clr_CAPCON0_CAPF2;
}
//-----------------------------------------------------------------------------------------------------------
void PWM_Brake_ISR(void) interrupt 14    // Vector @  0x73
{
    clr_FBD_FBF;
}
//-----------------------------------------------------------------------------------------------------------
void UART1_ISR(void) interrupt 15    			// Vector @  0x7B
{
    clr_SCON_1_RI_1;
		clr_SCON_1_TI_1;
}
//-----------------------------------------------------------------------------------------------------------
void Timer3_ISR(void) interrupt 16    		// Vector @  0x83
{
    clr_T3CON_TF3;
}
//-----------------------------------------------------------------------------------------------------------
void WKT_ISR(void) interrupt 17    				// Vector @  0x8B
{
    clr_WKCON_WKTF;
}


//-----------------------------------------------------------------------------------------------------------
void main(void)
{
		while(1);
}

#else 

UINT8  ms10_flag=0;
UINT8  ms50_flag = 0;
UINT8  ms100_flag=0;
UINT8  ms500_flag=0;
UINT8  ms1000_flag=0;

void Timer0_ISR(void) interrupt 1        // Vector @  0x0B
{
	static UINT16 cnt1=0;
	static UINT16 cnt2=0;
	static UINT16 cnt3=0;
  static UINT16 cnt4=0;  //~~~~3.29
	
    //clr_TCON_TF0;
	TH0 = (TIMER_DIV12_VALUE_10ms>>8);
	TL0 = (TIMER_DIV12_VALUE_10ms);
	
	  ms10_flag=1;
	cnt1++;
	if(cnt1>10)//100ms=10
	{
			cnt1=0;
		  ms100_flag=1;
	}
	
	
  
  
	
		cnt2++;
	if(cnt2>50)//250ms 25
	{
		cnt2=0;
		  ms500_flag=1;
	}
	
	
	cnt3++;
	if(cnt3>99)//1000ms//99:1Ð¡Ê±Âý30Ãë
	{
			cnt3=0;
		  ms1000_flag=1;
	}
  
  cnt4++;
  if(cnt4 > 5) //50ms  
  {
    cnt4 = 0;
    ms50_flag = 1;

  }
  
  
}


void UART0_ISR(void) interrupt 4         // Vector @  0x23
{
	u8 dat;

	//	   if (TI)
	//	    {
	//	        clr_SCON_TI;                                       // Clear TI (Transmit Interrupt).
	//	    }
	//

    if (RI)
    {
     //treadmills.ack=1;
      dat = SBUF;
      Enter_queue(&rx_queue,dat);
      clr_RI;                                         // Clear RI (Receive Interrupt).
    }
}

#endif
