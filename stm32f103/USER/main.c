#include "stm32f10x.h"                  // Device header 
#include "main.h"//主函数头文件

//@brief  主函数

unsigned char buf1[200]="[v10][m0][t5]前方有行人 距离为0.00米";
unsigned char buf2[200]="[v10][m0][t5]前方有机动车 距离为0.00米";
unsigned char buf3[200]="[v10][m0][t5]前方有动物 距离为0.00米";
unsigned char buf4[200]="[v10][m0][t5]前方有座椅 距离为0.00米";
unsigned char buf6[200]="[v10][m0][t5]前方有障碍物 距离为0.00米";



char MQTTDATA1[100]="AT+MQTTPUB=0,\"ZDFPUB\",\"DSQQ=00=\",1,0\r\n";
char MQTTDATA2[100]="AT+MQTTPUB=0,\"ZDFPUB\",\"people down\",1,0\r\n";
char MQTTDATA3[100]="AT+MQTTPUB=0,\"ZDFPUB\",\"Light=000=\",1,0\r\n";
char MQTTDATA4[100]="AT+MQTTPUB=0,\"ZDFPUB\",\"normal\",1,0\r\n";


extern char esp8266_buf[128];


u8 ds=0;
int i;
int j;
int k;
float Light = 0;
int tip_flag = 0;
int down =0;
int main(void) 
{	

	SYS_Init();//系统初始化总函数
	
  timer_init_config(); 		                             // 定时器2初始化设置
  ESP8266_Init();                      

	while(1)   //主循环
	{
		Light = LIght_Intensity(); //获取光照值
		ds=sr04_get_distance();	//获取超声波测距值
		lightpub(Light/10/10,(int)Light/10%10,(int)Light%10);//mqtt上传光照值
		dspub((int)ds/10%10,ds%10);//mqtt超声波测距值
		mpu6050_ctrl();//检测姿态情况
		tipsub();//mqtt上传姿态情况
		sr04_ctrl(ds);   //k210识别物体，同时语音播报        
	  light_ctrl(Light);//根据光照值控制led灯
	}
}
	


  //* @brief  系统初始化总函数
void SYS_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组函数
	delay_init();	    							 //延时函数初始化	  
	uart_init(115200);	 	                         //串口1初始化为115200
	Usart2_Init(115200);//串口2初始化为115200
	USART3_Init(9600);//串口3初始化为9600
	LED_Init();		                               	 //初始化与LED连接的硬件接口
	MPU_Init();	                                     //初始化MPU6050 --姿态传感器
	BH1750_Init(); //初始化bh1750 -- 光照传感器
	sr04_init(); //初始化sr04 -- 超声波测距传感器
	
	while(mpu_dmp_init())                            //初始化mpu_dmp库
 	{									
		SYN_FrameInfo("[v5][m0][t5]启动中");
		delay_ms(1800);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	SYN_FrameInfo("[v10][m0][t5]启动完成");
	mpu6050.flag = 0;                               //采集成功标志位初始化
	mpu6050.speed = 0;							//上报速度初始化
}

 //  @brief  MPU6050数据读取函数
void MPU_Read(void)
{
	int count = 0;
	while(mpu6050.flag == 0||count < 500)
	{
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp处理得到数据，对返回值进行判断
	{ 
		temp=MPU_Get_Temperature();	                //得到温度值
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		mpu6050.speed++;                            //上报速度自加
		if(mpu6050.speed == 4)						//上报速度阈值设置
		{
			mpu6050.flag = 1;						//采集成功标志位设置为有效
			mpu6050.speed = 0;						//上报速度归零
		}	
	}
	count++;
	if(count>500)
	{
		count = 0;
		return;
	}
}
	
}
/**
  * @brief  MPU6050数据上报
  * @param  无
  * @retval 无
  */
void DATA_Report(void)
{
		if(mpu6050.flag == 1)						//采集成功时
	{ 
		if(temp<0)								//对数据正负判断，判断为负时
		{
			temp=-temp;							//对负数据取反
		}
		else       ;                             //判断为正时
		

		
		temp=pitch*10;							 //赋temp为pitch
		if(temp<0)								//对数据正负判断，判断为负时
		{
			temp=-temp;						    //对负数据取反		
		}
		else     ;                               //判断为正时 
		down = temp/10;
		
		temp=roll*10;                            //赋temp为roll
		if(temp<0)								//对数据正负判断，判断为负时
		{
			temp=-temp;						    //对负数据取反	
		}
		else      ;                              //判断为正时
		temp=yaw*10;                           //赋temp为yaw
		if(temp<0)								//对数据正负判断，判断为负时
		{
			temp=-temp;						    //对负数据取反
		}
		else     ;                               //判断为正时
		
		mpu6050.flag = 0;									//采集成功标志位设置为无效
	}
	else ;														//防卡死
}


//光照值发送函数
void lightpub(int x,int y,int z)
{
			MQTTDATA3[29]=x+'0';//光照值写入字符串
		MQTTDATA3[30]=y+'0';
	  MQTTDATA3[31]=z+'0';
		ESP8266_SendCmd(MQTTDATA3, "");//通过esp8266向MQTT发送数据
		delay_ms(1000);
}


//测距发送函数
void dspub(int x,int y)
{
		MQTTDATA1[28]=x+'0';//测距值写入字符串
		MQTTDATA1[29]=y+'0';
		ESP8266_SendCmd(MQTTDATA1, "");//通过esp8266向MQTT发送数据
		delay_ms(1000);
}

//光照值控制led灯函数
void light_ctrl(float light)
{
	
	if(light<50.0)//判断光照值
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}



//判断姿态情况函数
void mpu6050_ctrl()
{
	int j = 0;

	MPU_Read();    //MPU6050数据读取
	DATA_Report(); //MPU6050数据上报
	if(down>50)
	{	
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		delay_ms(50);
		tip_flag = 1;	//姿态发生异常，姿态标志位置1			
	}
	else
	{
		tip_flag = 0;//姿态恢复正常，姿态标志位置0
		GPIO_SetBits(GPIOA,GPIO_Pin_8);	
	}
}

//由k210识别物体，判断k210传输值
void sr04_ctrl(u8 ds)
{
	if(sta==0X31)
	{
		buf1[30]=ds/100+'0';
		buf1[32]=(ds/10)%10+'0';
		buf1[33]=ds%10+'0';
				
				
		SYN_FrameInfo(buf1);	//行人
		for(i=0;i<=3;i++){delay_ms(1800);}
		sta=0x00;	 		
	}
	else if(sta==0X32)
	{
		buf1[32]=ds/100+'0';
		buf1[34]=(ds/10)%10+'0';
		buf1[35]=ds%10+'0';
				
		SYN_FrameInfo(buf2);//机动车
		for(i=0;i<=3;i++){delay_ms(1800);}		
		sta=0x00;
	}
	else if(sta==0X33)	
	{	
	buf3[30]=ds/100+'0';
	buf3[32]=(ds/10)%10+'0';
	buf3[33]=ds%10+'0';
				
	SYN_FrameInfo(buf3);//动物
	for(i=0;i<=3;i++){delay_ms(1800);}		
	sta=0x00;;
			 	
	}
	else if(sta==0X34)	
	{	
		buf4[30]=ds/100+'0';
		buf4[32]=(ds/10)%10+'0';
		buf4[33]=ds%10+'0';
				
		SYN_FrameInfo(buf4);	//座椅
		for(i=0;i<=3;i++){delay_ms(1800);}
		sta=0x00;	
	}
	else if(ds>5&&ds<30)//若有识别不出的物体，自动转化为障碍物
	{
		buf6[32]=ds/100+'0';
		buf6[34]=(ds/10)%10+'0';
		buf6[35]=ds%10+'0';
				
		SYN_FrameInfo(buf6);
		for(i=0;i<=3;i++){delay_ms(1800);}		
		sta=0x00;
	}		
}


//根据姿态标志位判断
void tipsub()
{
	if(tip_flag == 1)//为1则说明姿态异常
		ESP8266_SendCmd(MQTTDATA2, "");
	else
		ESP8266_SendCmd(MQTTDATA4, "");
}