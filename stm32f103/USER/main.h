#include "stdio.h"//��׼���������
#include "string.h"//�ַ�����
#include "stdlib.h"//���õ�ϵͳ������
#include "sys.h"//ϵͳ�жϷ����
#include "delay.h"//��ʱ������
#include "usart.h"//�������ÿ�
#include "led.h"//LED������

#include "mpu6050.h"//MPU6050������
#include "inv_mpu.h"//������������
#include "inv_mpu_dmp_motion_driver.h" //DMP��̬�����
#include "syn.h"

#include "mcu_config.h"
#include "bh1750.h"
#include "esp8266.h"

char tmp_buf[33];			//�ַ�������
float pitch,roll,yaw; 		//ŷ����:�����ǣ�ƫ���ǣ���ת��
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����  angular acceleration
short gyrox,gyroy,gyroz;	//������ԭʼ����  gyroscope
short temp;					//�¶�


struct MPU6050				//MPU6050�ṹ��
{
	u8 flag;				//�ɼ��ɹ���־λ
	u8 speed;				//�ϱ��ٶ�
}mpu6050;					//Ψһ�ṹ�����


int main(void);				//������
void SYS_Init(void);		//ϵͳ��ʼ���ܺ���
void MPU_Read(void);		//MPU6050���ݶ�ȡ����	
void DATA_Report(void);		//MPU6050�����ϱ�			
void buffer_Init(void);
void light_ctrl(float light);
void mpu6050_ctrl(void);
void sr04_ctrl(u8 ds);
void lightpub(int x,int y,int z);
void dspub(int x,int y);
void esp8266_ctrl();
void tipsub();

