#include "stm32f10x.h"                  // Device header 
#include "main.h"//������ͷ�ļ�

//@brief  ������

unsigned char buf1[200]="[v10][m0][t5]ǰ�������� ����Ϊ0.00��";
unsigned char buf2[200]="[v10][m0][t5]ǰ���л����� ����Ϊ0.00��";
unsigned char buf3[200]="[v10][m0][t5]ǰ���ж��� ����Ϊ0.00��";
unsigned char buf4[200]="[v10][m0][t5]ǰ�������� ����Ϊ0.00��";
unsigned char buf6[200]="[v10][m0][t5]ǰ�����ϰ��� ����Ϊ0.00��";



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

	SYS_Init();//ϵͳ��ʼ���ܺ���
	
  timer_init_config(); 		                             // ��ʱ��2��ʼ������
  ESP8266_Init();                      

	while(1)   //��ѭ��
	{
		Light = LIght_Intensity(); //��ȡ����ֵ
		ds=sr04_get_distance();	//��ȡ���������ֵ
		lightpub(Light/10/10,(int)Light/10%10,(int)Light%10);//mqtt�ϴ�����ֵ
		dspub((int)ds/10%10,ds%10);//mqtt���������ֵ
		mpu6050_ctrl();//�����̬���
		tipsub();//mqtt�ϴ���̬���
		sr04_ctrl(ds);   //k210ʶ�����壬ͬʱ��������        
	  light_ctrl(Light);//���ݹ���ֵ����led��
	}
}
	


  //* @brief  ϵͳ��ʼ���ܺ���
void SYS_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����麯��
	delay_init();	    							 //��ʱ������ʼ��	  
	uart_init(115200);	 	                         //����1��ʼ��Ϊ115200
	Usart2_Init(115200);//����2��ʼ��Ϊ115200
	USART3_Init(9600);//����3��ʼ��Ϊ9600
	LED_Init();		                               	 //��ʼ����LED���ӵ�Ӳ���ӿ�
	MPU_Init();	                                     //��ʼ��MPU6050 --��̬������
	BH1750_Init(); //��ʼ��bh1750 -- ���մ�����
	sr04_init(); //��ʼ��sr04 -- ��������ഫ����
	
	while(mpu_dmp_init())                            //��ʼ��mpu_dmp��
 	{									
		SYN_FrameInfo("[v5][m0][t5]������");
		delay_ms(1800);
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	SYN_FrameInfo("[v10][m0][t5]�������");
	mpu6050.flag = 0;                               //�ɼ��ɹ���־λ��ʼ��
	mpu6050.speed = 0;							//�ϱ��ٶȳ�ʼ��
}

 //  @brief  MPU6050���ݶ�ȡ����
void MPU_Read(void)
{
	int count = 0;
	while(mpu6050.flag == 0||count < 500)
	{
	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp����õ����ݣ��Է���ֵ�����ж�
	{ 
		temp=MPU_Get_Temperature();	                //�õ��¶�ֵ
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
		mpu6050.speed++;                            //�ϱ��ٶ��Լ�
		if(mpu6050.speed == 4)						//�ϱ��ٶ���ֵ����
		{
			mpu6050.flag = 1;						//�ɼ��ɹ���־λ����Ϊ��Ч
			mpu6050.speed = 0;						//�ϱ��ٶȹ���
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
  * @brief  MPU6050�����ϱ�
  * @param  ��
  * @retval ��
  */
void DATA_Report(void)
{
		if(mpu6050.flag == 1)						//�ɼ��ɹ�ʱ
	{ 
		if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;							//�Ը�����ȡ��
		}
		else       ;                             //�ж�Ϊ��ʱ
		

		
		temp=pitch*10;							 //��tempΪpitch
		if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;						    //�Ը�����ȡ��		
		}
		else     ;                               //�ж�Ϊ��ʱ 
		down = temp/10;
		
		temp=roll*10;                            //��tempΪroll
		if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;						    //�Ը�����ȡ��	
		}
		else      ;                              //�ж�Ϊ��ʱ
		temp=yaw*10;                           //��tempΪyaw
		if(temp<0)								//�����������жϣ��ж�Ϊ��ʱ
		{
			temp=-temp;						    //�Ը�����ȡ��
		}
		else     ;                               //�ж�Ϊ��ʱ
		
		mpu6050.flag = 0;									//�ɼ��ɹ���־λ����Ϊ��Ч
	}
	else ;														//������
}


//����ֵ���ͺ���
void lightpub(int x,int y,int z)
{
			MQTTDATA3[29]=x+'0';//����ֵд���ַ���
		MQTTDATA3[30]=y+'0';
	  MQTTDATA3[31]=z+'0';
		ESP8266_SendCmd(MQTTDATA3, "");//ͨ��esp8266��MQTT��������
		delay_ms(1000);
}


//��෢�ͺ���
void dspub(int x,int y)
{
		MQTTDATA1[28]=x+'0';//���ֵд���ַ���
		MQTTDATA1[29]=y+'0';
		ESP8266_SendCmd(MQTTDATA1, "");//ͨ��esp8266��MQTT��������
		delay_ms(1000);
}

//����ֵ����led�ƺ���
void light_ctrl(float light)
{
	
	if(light<50.0)//�жϹ���ֵ
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}



//�ж���̬�������
void mpu6050_ctrl()
{
	int j = 0;

	MPU_Read();    //MPU6050���ݶ�ȡ
	DATA_Report(); //MPU6050�����ϱ�
	if(down>50)
	{	
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		delay_ms(50);
		tip_flag = 1;	//��̬�����쳣����̬��־λ��1			
	}
	else
	{
		tip_flag = 0;//��̬�ָ���������̬��־λ��0
		GPIO_SetBits(GPIOA,GPIO_Pin_8);	
	}
}

//��k210ʶ�����壬�ж�k210����ֵ
void sr04_ctrl(u8 ds)
{
	if(sta==0X31)
	{
		buf1[30]=ds/100+'0';
		buf1[32]=(ds/10)%10+'0';
		buf1[33]=ds%10+'0';
				
				
		SYN_FrameInfo(buf1);	//����
		for(i=0;i<=3;i++){delay_ms(1800);}
		sta=0x00;	 		
	}
	else if(sta==0X32)
	{
		buf1[32]=ds/100+'0';
		buf1[34]=(ds/10)%10+'0';
		buf1[35]=ds%10+'0';
				
		SYN_FrameInfo(buf2);//������
		for(i=0;i<=3;i++){delay_ms(1800);}		
		sta=0x00;
	}
	else if(sta==0X33)	
	{	
	buf3[30]=ds/100+'0';
	buf3[32]=(ds/10)%10+'0';
	buf3[33]=ds%10+'0';
				
	SYN_FrameInfo(buf3);//����
	for(i=0;i<=3;i++){delay_ms(1800);}		
	sta=0x00;;
			 	
	}
	else if(sta==0X34)	
	{	
		buf4[30]=ds/100+'0';
		buf4[32]=(ds/10)%10+'0';
		buf4[33]=ds%10+'0';
				
		SYN_FrameInfo(buf4);	//����
		for(i=0;i<=3;i++){delay_ms(1800);}
		sta=0x00;	
	}
	else if(ds>5&&ds<30)//����ʶ�𲻳������壬�Զ�ת��Ϊ�ϰ���
	{
		buf6[32]=ds/100+'0';
		buf6[34]=(ds/10)%10+'0';
		buf6[35]=ds%10+'0';
				
		SYN_FrameInfo(buf6);
		for(i=0;i<=3;i++){delay_ms(1800);}		
		sta=0x00;
	}		
}


//������̬��־λ�ж�
void tipsub()
{
	if(tip_flag == 1)//Ϊ1��˵����̬�쳣
		ESP8266_SendCmd(MQTTDATA2, "");
	else
		ESP8266_SendCmd(MQTTDATA4, "");
}