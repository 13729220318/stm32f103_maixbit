#ifndef __SYN_H
#define __SYN_H
#include "sys.h"


void SYN_FrameInfo(u8 *HZdata);
void YS_SYN_Set(u8 *Info_data);

//������ģ������
#define	TRIG_PORT      GPIOA		       
#define	ECHO_PORT      GPIOA		
#define	TRIG_PIN       GPIO_Pin_0   	//TRIG-��������PC1       
#define	ECHO_PIN       GPIO_Pin_1		//ECHO-��������PC2    

//������ģ���ʼ��
void sr04_init(void);

//������ģ����
int32_t sr04_get_distance(void);
#endif

