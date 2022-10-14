#ifndef __SYN_H
#define __SYN_H
#include "sys.h"


void SYN_FrameInfo(u8 *HZdata);
void YS_SYN_Set(u8 *Info_data);

//超声波模块引脚
#define	TRIG_PORT      GPIOA		       
#define	ECHO_PORT      GPIOA		
#define	TRIG_PIN       GPIO_Pin_0   	//TRIG-发送引脚PC1       
#define	ECHO_PIN       GPIO_Pin_1		//ECHO-接收引脚PC2    

//超声波模块初始化
void sr04_init(void);

//超声波模块测距
int32_t sr04_get_distance(void);
#endif

