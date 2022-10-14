/*************************************************************************
 * �ļ����� ��mcu_config.c
 * ��    �� ������ͨ����������         
 * ʵ��ƽ̨ ��STM32F103vet6������
 * Ӳ������ ������1���������ͨѶ���������أ�����2������������ͨѶ
 *    �������崮��2��
 *         Usart2 ---------GPRSģ��
 *        PA2  (TXD)----------->  RXD   
 *			  PA3  (RXD) ---------->  TXD   
 *        GND  ---------------> GND
 *    (�����崮��1)
 *         Usart1 : -------  �� ��
 *			  PA9 ��TXD�� ------->   TXD  
 *		    PA10 ��RXD��------->   RXD  
 *        GND   -------------->  GND
 * ��汾  ��STM32F10x_StdPeriph_Lib_V3.5.0
*****************************************************************************/
#include "mcu_config.h"
#include "string.h"
#include "sys.h"

/*---------------------------- �� �� �� ��	---------------------------------*/
void usart1_gpio_config(void);
void usart2_gpio_config(void);
void usart1_parameter_config(void);
void usart2_parameter_config(void);
void timer_parameter_config(void);

/*---------------------------- �� �� �� ��	---------------------------------*/
u8   buf_address = 0;                             // �����׵�ַ��ʼֵΪ0
char gprs_buf[BUF_MAX];                           // ����2���ջ���

/*************************************************************************
* ��������: usart1_init_config
* ��    ��: ����1��ʼ������
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ��
*************************************************************************/
/*void usart1_init_config()
{
	usart1_gpio_config();                            // �˿�1����ģʽ����	
    usart1_parameter_config();                       // ����1������������		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);   // ʹ�ܴ���1�����ж�
	USART_Cmd(USART1, ENABLE);                       // ʹ�ܴ���1 
	USART_ClearFlag(USART1, USART_FLAG_TC);		       // �������1�ķ�����ɱ�־
}*/

/*************************************************************************
* ��������: usart2_init_config
* ��    ��: ����2��ʼ������
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ��
*************************************************************************/
void usart2_init_config(void)
{	
	usart2_gpio_config();                                 // �˿�2����ģʽ����	
	usart2_parameter_config();                            // ����2������������	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);        // ʹ�ܴ���2�����ж� 
	USART_Cmd(USART2, ENABLE);                            // ʹ�ܴ���  
	USART_ClearFlag(USART2, USART_FLAG_TC);		            // ���������ɱ�־
}


/*************************************************************************
* ��������: timer_init_config
* ��    ��: Timer2��ʼ������
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ���ö�ʱ��2  ��ʱ1��
*************************************************************************/
void timer_init_config(void)
{ 	 		
	timer_parameter_config();                             // ��ʱ��2��������		
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );           // ������ʱ��2�ж�
	TIM_Cmd(TIM2, ENABLE);  									            // ������ʱ��2
}	

/*************************************************************************
* ��������: usart1_gpio_config
* ��    ��: ����1��GPIO�˿�����
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ����1ʱ���ǹ�����APB2��
*************************************************************************/
/*void usart1_gpio_config(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;	
	//ʹ��USART1��GPIOA����ʱ��
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	  
	//��λ����1
 	 USART_DeInit(USART1);	
  //USART1_GPIO��ʼ������ 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			        // USART1_TXD(PA.9)     
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    // ������������������Ϊ50MHz
   GPIO_Init(GPIOA, &GPIO_InitStructure);				          // ���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.9) 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				      // USART1_RXD(PA.10)
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// ��������
   GPIO_Init(GPIOA, &GPIO_InitStructure);				          // ���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.10)
}*/

/*************************************************************************
* ��������: usart2_gpio_config
* ��    ��: ����2��GPIO�˿�����
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ����2ʱ���ǹ�����APB1
*************************************************************************/
void usart2_gpio_config()
{
   GPIO_InitTypeDef  GPIO_InitStructure;	
	/*ʹ��USART2��GPIOA����ʱ��*/ 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	/*��λ����2*/
 	 USART_DeInit(USART2);
  /*USART1_GPIO��ʼ������*/ 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			            // USART2_TXD(PA.2)     
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	         // �����������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	        // ������������������Ϊ50MHz
   GPIO_Init(GPIOA, &GPIO_InitStructure);				              // ���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_TXD(PA.2) 

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				          // USART2_RXD(PA.3)
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	    // ��������
   GPIO_Init(GPIOA, &GPIO_InitStructure);					            // ���ÿ⺯���е�GPIO��ʼ����������ʼ��USART1_RXD(PA.3)
}

/*************************************************************************
* ��������: usart1_parameter_config
* ��    ��: ����1��������
* ��    ��: Bound1��������(���ã�2400��4800��9600��19200��38400��115200��)
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ����ģʽ��8-n-1
*************************************************************************/
/*void usart1_parameter_config(void)
{
	USART_InitTypeDef   USART_InitStructure;
	
	//USART1 ��������
	USART_InitStructure.USART_BaudRate =115200;	                           // ���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		         // 8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		             // 1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			               // ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 // ��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				 // ����ģʽ����Ϊ�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure);			                         // ��ʼ������1
}*/

/*************************************************************************
* ��������: usart2_parameter_config
* ��    ��: ����2��������
* ��    ��: Bound2��������(���ã�2400��4800��9600��19200��38400��115200��)
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ����ģʽ��8-n-1
*************************************************************************/ 
void usart2_parameter_config(void)
{
	USART_InitTypeDef   USART_InitStructure;

	/*USART1 ��ʼ������*/
	USART_InitStructure.USART_BaudRate =115200;	                          // ���ò�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		        // 8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		            // 1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			              // ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				// ����ģʽ����Ϊ�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);			                        // ��ʼ������2
}

/*************************************************************************
* ��������: timer_parameter_config
* ��    ��: ��ʱ��2��������
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ���ö�ʱ��2  ��ʱ1��
*************************************************************************/
void timer_parameter_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);              // ʹ�ܶ�ʱ��2ʱ��
	
	/*��ʱ����������*/
	TIM_TimeBaseStructure.TIM_Period = 9999;					                // ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ(������10000Ϊ1s)
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					              // ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ(10KHz�ļ���Ƶ��)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		        // ����ʱ�ӷָ�:���ָѡ��Ĭ��TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	      // TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				
}	

/*************************************************************************
*��������: nvic_init_config
* ��    ��: �ж�������ʼ������
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: �����ж����ȼ�
*************************************************************************/
void nvic_init_config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;                        // ����NVIC��ʼ���ṹ��
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);               // �����������ѡַ��0x08000000
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             // �����ж����ȼ���Ϊ2����ռ���ȼ��ʹ����ȼ�����λ(����0��3)

	/*��ʱ��2�ж���������*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				      // TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	// ������ռ���ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    // ������Ӧ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				      // ʹ��IRQͨ��
	NVIC_Init(&NVIC_InitStructure); 

	/*//����1�ж���������
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				    // ����1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	  // ������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    // ������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				      // ʹ��NVIC
	NVIC_Init(&NVIC_InitStructure);	*/

	/*����2�ж���������*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           // ����2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	    // ������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		      // ������Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
}


/*************************************************************************
* ��������: led_gpio_config 
* ��    ��: ����LED���Ŷ˿� 
* �������: ��
* ��    ��: ��
* ��    ��: ��
* ˵    ��: 1��LED1��IO����:PB0 
*           2��������led_init_config��������
*************************************************************************/
void led_gpio_config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;		

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	// ʹ��GPIOB������ʱ��	

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 ;	              // ѡ��Ҫ��ʼ����GPIOB����PB1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	      // �������Ź���ģʽΪͨ��������� 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      // ������������������Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);		
	LED_OFF;
}


/*************************************************************************
* ��������: fputc
* ��    ��: �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf���� 
* �������: ��
* ��    ��: ��
* ��    ��: ��
* ˵    ��:
*************************************************************************/
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);	                         // ����һ���ֽ����ݵ�����
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	 // �ȴ��������
		return (ch);
}

/*************************************************************************
* ��������: clear_gprs_buf
* ��    ��: �������2��������
* ��    ��: ��
* ��    ��: ��
* ��    ��: �� 
* ˵    ��: ��
*************************************************************************/
void clear_gprs_buf(void)
{
	 memset(gprs_buf,0,BUF_MAX);
   buf_address = 0;                                        // �����ַ�������ʼ�洢λ��
}


/*******************************************************************************
* ��������: find_string
* ��    ��: �жϻ������Ƿ���ָ�����ַ���
* �������: �ַ��� p
* ��    ��: ��
* ��    ��: unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ�
* ˵    ��: ����extern char *strstr(char *str1, char *str2)�ú�����
*******************************************************************************/

u8 find_string(char* p)
{ 
	if(strstr(gprs_buf,p)!=NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*******************************************************************************
* ��������: gprs_send_cmd 
* ��    ��: ����ATָ���
* �������: b--- �������ݵ�ָ��(����)
*           a--- ϣ�����յ���Ӧ������ָ�루Ӧ��
*           times---���Ӧ����������ѭ������AT����Ĵ���
*						wait_time ----���͵ȴ�ʱ��(��λ��20ms)
* ��    ��: ��
* ��    ��: ��ȷ---1   ����---0
* ˵    ��: ��
*******************************************************************************/
u8 gprs_send_cmd(char *b,char *a,u8 times,u8 wait_time)         
{
	u8 i;
	i = 0;
	clear_gprs_buf();                    // �������2��������
	while(i < times)                    
	{
    gprs_send_string(b);
		gprs_send_string("\r\n");	         // �س�����
	  delay_ms(wait_time);
    /*if(find_string(a))
	  {
	     return 1;
	  }*/
	  i++;
		return 1;
	}
	//return 0;
}

/*******************************************************************************
* ������  : gprs_send_string
* ����    : USART2�����ַ���
* ����    : *s �ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void gprs_send_string(char* s)
{
	while(*s)                                                   // ����ַ���������
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);                             // ���͵�ǰ�ַ�
	}
}

/******************************************************************************
* ��������:str_delim
* ��    ��:�ַ����ָ��
* �������: num   0---ȡ���ָ��ַ�����delim��ǰ����ַ���  1---ȡ���ָ��ַ�����delim��������ַ���
*           temp  Ҫ�ָ���ַ���
*           delim �ָ����ַ���
* ��    ��: ��
* ��    ��: �ָ����ַ���
* ˵    ��:          
*******************************************************************************/
char *str_delim(uint8_t num,char *temp,char *delim)
{
	int i;
	char *str[2]={0};
	char *tok=temp;
	char *restr;
	for(i=0;i<2;i++)
	{
		tok=strtok(tok,delim);
		str[i]=tok;
		tok = NULL;
	}
	restr=str[num];
  return restr;
}

/******************************************************************************
* ��������: send_end
* ��    ��: �������ݽ�����
* �������: ��
* ��    ��: ��
* ��    ��: ��
* ˵    ��:�����������ݺ���16���Ʒ������ݽ�����0x1A
*******************************************************************************/
void send_end(void)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
	USART_SendData(USART2 ,0X1A);   
}

