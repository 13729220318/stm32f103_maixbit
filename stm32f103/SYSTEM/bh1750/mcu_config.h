#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H

#include "stm32f10x.h"
#include <stdio.h>

/*---------------------------- �� �� ��	----------------------------------------*/
#define BUF_MAX   128                                      // ����2���ջ��泤��
#define LED_OFF    GPIO_SetBits(GPIOB,GPIO_Pin_1)             // Ϩ��LED      
#define LED_ON     GPIO_ResetBits(GPIOB,GPIO_Pin_1)           // ����LED 

/*--------------------------- �� �� �� �� -------------------------------------*/
void usart1_init_config(void);                                // ����1��ʼ�� MCU��PCͨ��
void usart2_init_config(void);                                // ����2��ʼ�� MCU��GPRSģ��ͨ��
void gprs_send_string(char* s);                               // MCU �����ַ�����GPRS
void nvic_init_config(void);                                  // �жϴ�����
void led_gpio_config(void);                                   // LED�� GPIO ��ʼ������
void clear_gprs_buf(void);                                    // ������յ�GPRS�Ļ���
void timer_init_config(void);                                 // ��ʱ����ʼ��
u8 find_string(char* p);                                      // �����ַ���
u8 gprs_send_cmd(char *b,char *a,u8 times,u8 wait_time);      // MCU����AT�����
char *str_delim(uint8_t num,char *temp,char *delim);          // �ַ����ָ��
void send_end(void);                                          // �������ݽ�����
#endif
