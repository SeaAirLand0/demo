/* Ctrl + h ��QUEUE �滻 ��Ҫ�� �ַ���*/
//QUEUE

#ifndef  QUEUE_HH_
#define	 QUEUE_HH_ 



/* common  Includes ------------------------------------------------------------------*/
#include "common.h"


/* User  Includes ------------------------------------------------------------------*/



/* Micro  define  ------------------------------------------------------------------*/


/* Type  define  ------------------------------------------------------------------*/

// ����ָ�붨��
/*
  void (*pfunc) (void);
  u8   (*pfunc) (u8,u8);
*/


typedef struct _T_QUEUE 
{
	u8 *pBase;
	u32 front;    //ָ����е�һ��Ԫ��
	u32 rear;    //ָ��������һ��Ԫ�ص���һ��Ԫ��
	u32 maxsize; //ѭ�����е����洢�ռ�
}T_QUEUE;



/*Eextern  variable  And function  Declaration------------------------------------------*/

extern  void Create_Queue(T_QUEUE *Q,u8 *pbuff,u32 maxsize);
extern  u8 Enter_queue(T_QUEUE *Q, u8 val);
extern  u8 Denter_queue(T_QUEUE *Q, u8 *val);
extern  u8 Is_Full_Queue(T_QUEUE *Q);



#endif
