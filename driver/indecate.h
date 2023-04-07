/*
 * indecate.h
 *
 *  Created on: 2018Äê8ÔÂ1ÈÕ
 *      Author: Administrator
 */

#ifndef __indecate__HH
#define __indecate__HH
#include "common.h"
#include "aip1628.h"



typedef struct _indecate_t
{
	u8 seg[4];

//	u8 led9:1;
//	u8 led10:1;
//	u8 led21:1;
//	u8 led31:1;
//	u8 led30:1;
//	u8 led40:1;
//	u8 led41:1;
//	u8 :1;


	u8 led_time:1;
	u8 led_distance:1;
	u8 led_calorie:1;
	u8 led_speed:1;
	u8 led_auto:1;
	u8 led_updp:1;
	u8 led_downdp:1;
	u8 led_back_light:1;  //2023.3.22

}indecate_t;


extern  indecate_t   indecate;

extern u8  indecate_update_flag;

#define set_light_scale(scale)  {set_aip1628_lighte_scale(scale);}
extern void disp_update(void);





#endif /* SOFTWARE_BSP_INDECATE_H_ */
