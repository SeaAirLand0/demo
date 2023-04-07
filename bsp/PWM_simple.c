
#include "common.h"

/************************************************************************************************************
*    Main function 
************************************************************************************************************/
void pwm_init(void)
{

    PWM1_P14_OUTPUT_ENABLE;
  
    PWM_IMDEPENDENT_MODE;

/**********************************************************************
  PWM frequency = Fpwm/((PWMPH,PWMPL) + 1) <Fpwm = Fsys/PWM_CLOCK_DIV> 
                = (16MHz/8)/(999 + 1)
                = 2KHz
***********************************************************************/
    PWM_CLOCK_DIV_8;
    PWMPH = (900>>8);//600
    PWMPL = (900);


    PWM1H = (0>>8);
    PWM1L = 0;
    

/* PWM output inversly enable */
    PWM1_OUTPUT_NORMAL;

/*-------- PWM start run--------------*/ 
    set_LOAD;
    set_PWMRUN;

}


void PWM1DutyCycle(UINT16 pwm)
{
	PWM1H = (pwm>>8);
	PWM1L = pwm;
	set_LOAD;// 装载周期和占空比数据

}
