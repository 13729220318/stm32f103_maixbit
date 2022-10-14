/*************************************************************************
 * 文件名称 ：mcu_config.c
 * 描    述 ：串口通信驱动函数         
 * 实验平台 ：STM32F103vet6开发板
 * 硬件连接 ：串口1用于与电脑通讯及程序下载，串口2用于与评估板通讯
 *    （开发板串口2）
 *         Usart2 ---------GPRS模块
 *        PA2  (TXD)----------->  RXD   
 *			  PA3  (RXD) ---------->  TXD   
 *        GND  ---------------> GND
 *    (开发板串口1)
 *         Usart1 : -------  电 脑
 *			  PA9 （TXD） ------->   TXD  
 *		    PA10 （RXD）------->   RXD  
 *        GND   -------------->  GND
 * 库版本  ：STM32F10x_StdPeriph_Lib_V3.5.0
*****************************************************************************/
#include "mcu_config.h"
#include "string.h"
#include "sys.h"

/*---------------------------- 函 数 声 明	---------------------------------*/
void usart1_gpio_config(void);
void usart2_gpio_config(void);
void usart1_parameter_config(void);
void usart2_parameter_config(void);
void timer_parameter_config(void);

/*---------------------------- 变 量 声 明	---------------------------------*/
u8   buf_address = 0;                             // 缓存首地址初始值为0
char gprs_buf[BUF_MAX];                           // 串口2接收缓存

/*************************************************************************
* 函数名称: usart1_init_config
* 描    述: 串口1初始化配置
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 无
*************************************************************************/
/*void usart1_init_config()
{
	usart1_gpio_config();                            // 端口1工作模式设置	
    usart1_parameter_config();                       // 串口1工作参数设置		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);   // 使能串口1接收中断
	USART_Cmd(USART1, ENABLE);                       // 使能串口1 
	USART_ClearFlag(USART1, USART_FLAG_TC);		       // 清除串口1的发送完成标志
}*/

/*************************************************************************
* 函数名称: usart2_init_config
* 描    述: 串口2初始化设置
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 无
*************************************************************************/
void usart2_init_config(void)
{	
	usart2_gpio_config();                                 // 端口2工作模式设置	
	usart2_parameter_config();                            // 串口2工作参数设置	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);        // 使能串口2接收中断 
	USART_Cmd(USART2, ENABLE);                            // 使能串口  
	USART_ClearFlag(USART2, USART_FLAG_TC);		            // 清除发送完成标志
}


/*************************************************************************
* 函数名称: timer_init_config
* 描    述: Timer2初始化配置
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 利用定时器2  延时1秒
*************************************************************************/
void timer_init_config(void)
{ 	 		
	timer_parameter_config();                             // 定时器2参数设置		
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE );           // 开启定时器2中断
	TIM_Cmd(TIM2, ENABLE);  									            // 开启定时器2
}	

/*************************************************************************
* 函数名称: usart1_gpio_config
* 描    述: 串口1的GPIO端口配置
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 串口1时钟是挂载在APB2上
*************************************************************************/
/*void usart1_gpio_config(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;	
	//使能USART1和GPIOA外设时钟
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	  
	//复位串口1
 	 USART_DeInit(USART1);	
  //USART1_GPIO初始化设置 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			        // USART1_TXD(PA.9)     
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    // 设置引脚输出最大速率为50MHz
   GPIO_Init(GPIOA, &GPIO_InitStructure);				          // 调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.9) 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				      // USART1_RXD(PA.10)
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// 浮空输入
   GPIO_Init(GPIOA, &GPIO_InitStructure);				          // 调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.10)
}*/

/*************************************************************************
* 函数名称: usart2_gpio_config
* 描    述: 串口2的GPIO端口配置
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 串口2时钟是挂载在APB1
*************************************************************************/
void usart2_gpio_config()
{
   GPIO_InitTypeDef  GPIO_InitStructure;	
	/*使能USART2和GPIOA外设时钟*/ 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	/*复位串口2*/
 	 USART_DeInit(USART2);
  /*USART1_GPIO初始化设置*/ 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			            // USART2_TXD(PA.2)     
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	         // 复用推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	        // 设置引脚输出最大速率为50MHz
   GPIO_Init(GPIOA, &GPIO_InitStructure);				              // 调用库函数中的GPIO初始化函数，初始化USART1_TXD(PA.2) 

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				          // USART2_RXD(PA.3)
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	    // 浮空输入
   GPIO_Init(GPIOA, &GPIO_InitStructure);					            // 调用库函数中的GPIO初始化函数，初始化USART1_RXD(PA.3)
}

/*************************************************************************
* 函数名称: usart1_parameter_config
* 描    述: 串口1参数配置
* 输    入: Bound1：波特率(常用：2400、4800、9600、19200、38400、115200等)
* 输    出: 无
* 返    回: 无 
* 说    明: 工作模式：8-n-1
*************************************************************************/
/*void usart1_parameter_config(void)
{
	USART_InitTypeDef   USART_InitStructure;
	
	//USART1 参数设置
	USART_InitStructure.USART_BaudRate =115200;	                           // 设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		         // 8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		             // 1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			               // 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	 // 无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				 // 工作模式设置为收发模式
  USART_Init(USART1, &USART_InitStructure);			                         // 初始化串口1
}*/

/*************************************************************************
* 函数名称: usart2_parameter_config
* 描    述: 串口2参数配置
* 输    入: Bound2：波特率(常用：2400、4800、9600、19200、38400、115200等)
* 输    出: 无
* 返    回: 无 
* 说    明: 工作模式：8-n-1
*************************************************************************/ 
void usart2_parameter_config(void)
{
	USART_InitTypeDef   USART_InitStructure;

	/*USART1 初始化设置*/
	USART_InitStructure.USART_BaudRate =115200;	                          // 设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		        // 8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		            // 1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			              // 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				// 工作模式设置为收发模式
  USART_Init(USART2, &USART_InitStructure);			                        // 初始化串口2
}

/*************************************************************************
* 函数名称: timer_parameter_config
* 描    述: 定时器2参数设置
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 利用定时器2  延时1秒
*************************************************************************/
void timer_parameter_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);              // 使能定时器2时钟
	
	/*定时器参数设置*/
	TIM_TimeBaseStructure.TIM_Period = 9999;					                // 设置在下一个更新事件装入活动的自动重装载寄存器周期的值(计数到10000为1s)
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					              // 设置用来作为TIMx时钟频率除数的预分频值(10KHz的计数频率)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		        // 设置时钟分割:不分割，选择默认TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	      // TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				
}	

/*************************************************************************
*函数名称: nvic_init_config
* 描    述: 中断向量初始化配置
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 设置中断优先级
*************************************************************************/
void nvic_init_config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;                        // 定义NVIC初始化结构体
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);               // 将向量表基地选址在0x08000000
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);             // 设置中断优先级组为2，先占优先级和从优先级各两位(可设0～3)

	/*定时器2中断向量配置*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				      // TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  	// 设置抢占优先级3级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    // 设置响应优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				      // 使能IRQ通道
	NVIC_Init(&NVIC_InitStructure); 

	/*//串口1中断向量配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;				    // 串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	  // 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			    // 设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				      // 使能NVIC
	NVIC_Init(&NVIC_InitStructure);	*/

	/*串口2中断向量配置*/
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;           // 串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	    // 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		      // 设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			        // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
}


/*************************************************************************
* 函数名称: led_gpio_config 
* 描    述: 配置LED引脚端口 
* 输入参数: 无
* 输    出: 无
* 返    回: 无
* 说    明: 1）LED1的IO口是:PB0 
*           2）被函数led_init_config（）调用
*************************************************************************/
void led_gpio_config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;		

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);	// 使能GPIOB的外设时钟	

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1 ;	              // 选择要初始化的GPIOB引脚PB1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	      // 设置引脚工作模式为通用推挽输出 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      // 设置引脚输出最大速率为50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);		
	LED_OFF;
}


/*************************************************************************
* 函数名称: fputc
* 描    述: 重定向c库函数printf到串口，重定向后可使用printf函数 
* 输入参数: 无
* 输    出: 无
* 返    回: 无
* 说    明:
*************************************************************************/
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);	                         // 发送一个字节数据到串口
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	 // 等待发送完毕
		return (ch);
}

/*************************************************************************
* 函数名称: clear_gprs_buf
* 描    述: 清除串口2缓存数据
* 输    入: 无
* 输    出: 无
* 返    回: 无 
* 说    明: 无
*************************************************************************/
void clear_gprs_buf(void)
{
	 memset(gprs_buf,0,BUF_MAX);
   buf_address = 0;                                        // 接收字符串的起始存储位置
}


/*******************************************************************************
* 函数名称: find_string
* 描    述: 判断缓存中是否含有指定的字符串
* 输入参数: 字符串 p
* 输    出: 无
* 返    回: unsigned char:1 找到指定字符，0 未找到指定字符
* 说    明: 调用extern char *strstr(char *str1, char *str2)该函数，
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
* 函数名称: gprs_send_cmd 
* 描    述: 发送AT指令函数
* 输入参数: b--- 发送数据的指针(命令)
*           a--- 希望接收到的应答数据指针（应答）
*           times---如果应答数据有误，循环发送AT命令的次数
*						wait_time ----发送等待时间(单位：20ms)
* 输    出: 无
* 返    回: 正确---1   错误---0
* 说    明: 无
*******************************************************************************/
u8 gprs_send_cmd(char *b,char *a,u8 times,u8 wait_time)         
{
	u8 i;
	i = 0;
	clear_gprs_buf();                    // 清除串口2缓存数据
	while(i < times)                    
	{
    gprs_send_string(b);
		gprs_send_string("\r\n");	         // 回车换行
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
* 函数名  : gprs_send_string
* 描述    : USART2发送字符串
* 输入    : *s 字符串指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void gprs_send_string(char* s)
{
	while(*s)                                                   // 检测字符串结束符
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);                             // 发送当前字符
	}
}

/******************************************************************************
* 函数名称:str_delim
* 描    述:字符串分割函数
* 输入参数: num   0---取出分隔字符串（delim）前面的字符串  1---取出分隔字符串（delim）后面的字符串
*           temp  要分割的字符串
*           delim 分隔符字符串
* 输    出: 无
* 返    回: 分割后的字符串
* 说    明:          
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
* 函数名称: send_end
* 描    述: 发送内容结束符
* 输入参数: 无
* 输    出: 无
* 返    回: 无
* 说    明:当输入完内容后，以16进制发送内容结束符0x1A
*******************************************************************************/
void send_end(void)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
	USART_SendData(USART2 ,0X1A);   
}

