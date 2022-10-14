#include "syn.h"
#include "usart.h"
#include "string.h"
#include "delay.h"

//Music:选择背景音乐。0:无背景音乐，1~15：选择背景音乐
void SYN_FrameInfo(u8 *HZdata)
{
  /****************需要发送的文本**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//定义校验字节
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度

  /*****************帧固定配置信息**************************************/
  Frame_Info[0] = 0xFD ; 			//构造帧头FD
  Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
  Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
  Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
  Frame_Info[4] = 0x01 | 0 << 4 ; //构造命令参数：背景音乐设定

  /*******************校验码计算***************************************/
  for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
  {
    ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
  }

  for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
  {
    ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
  }
  /*******************发送帧信息***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  USART3_SendString(Frame_Info, 5 + HZ_Length + 1);
}

//默认波特率9600bps。
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  USART3_SendString(Info_data, Com_Len);
}
////////////////////////////////////////////////////////////////////////
u8 d;      //计算出的距离

//超声波初始化函数
void sr04_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
		 	       
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟使能
    
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;		//PC8接TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      //速度
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	         //初始化GPIO 

	GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				 //PC9接ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(ECHO_PORT,&GPIO_InitStructure);		//初始化GPIO 

	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//PC8初始状态为低电平，看时序图	
}

/*获取距离*/
int32_t sr04_get_distance(void)
{
	uint32_t t;	
		
	//PC8高电平
	GPIO_SetBits(TRIG_PORT,TRIG_PIN);
	delay_us(20);	//持续10us以上
	GPIO_ResetBits(TRIG_PORT,TRIG_PIN);//PC8低电平
	
	while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	//等待高电平
	
	//等待PC9出现高电平
	t=0;
	while(ECHO_PIN==0)
	{	
		//超时处理
		t++;
		delay_us(1);
		
		//如果超时，就返回一个错误码
		if(t >= 10000)
			return -1;
	
	}

	//测量高电平的时间
	t=0;
	while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN))
	{	
		t++;
		delay_us(9);	//9us == 3mm
		
		//如果超时，就返回一个错误码
		if(t >= 10000)
			return -2;
	}
		
	//由于测量的时间，就是超声波从发射到返回的时间	
	d = t*0.3;
	
	return d;
	
}
////////////////////////////////////////////////////////////////////////
