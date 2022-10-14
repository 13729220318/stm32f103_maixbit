#include "syn.h"
#include "usart.h"
#include "string.h"
#include "delay.h"

//Music:ѡ�񱳾����֡�0:�ޱ������֣�1~15��ѡ�񱳾�����
void SYN_FrameInfo(u8 *HZdata)
{
  /****************��Ҫ���͵��ı�**********************************/
  unsigned  char  Frame_Info[50];
  unsigned  char  HZ_Length;
  unsigned  char  ecc  = 0;  			//����У���ֽ�
  unsigned  int i = 0;
  HZ_Length = strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���

  /*****************֡�̶�������Ϣ**************************************/
  Frame_Info[0] = 0xFD ; 			//����֡ͷFD
  Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
  Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
  Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
  Frame_Info[4] = 0x01 | 0 << 4 ; //����������������������趨

  /*******************У�������***************************************/
  for(i = 0; i < 5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
  {
    ecc = ecc ^ (Frame_Info[i]);		//�Է��͵��ֽڽ������У��
  }

  for(i = 0; i < HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
  {
    ecc = ecc ^ (HZdata[i]); 				//�Է��͵��ֽڽ������У��
  }
  /*******************����֡��Ϣ***************************************/
  memcpy(&Frame_Info[5], HZdata, HZ_Length);
  Frame_Info[5 + HZ_Length] = ecc;
  USART3_SendString(Frame_Info, 5 + HZ_Length + 1);
}

//Ĭ�ϲ�����9600bps��
void YS_SYN_Set(u8 *Info_data)
{
  u8 Com_Len;
  Com_Len = strlen((char*)Info_data);
  USART3_SendString(Info_data, Com_Len);
}
////////////////////////////////////////////////////////////////////////
u8 d;      //������ľ���

//��������ʼ������
void sr04_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
		 	       
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʱ��ʹ��
    
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN;		//PC8��TRIG
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	      //�ٶ�
	GPIO_Init(TRIG_PORT, &GPIO_InitStructure);	         //��ʼ��GPIO 

	GPIO_InitStructure.GPIO_Pin = ECHO_PIN;				 //PC9��ECH0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(ECHO_PORT,&GPIO_InitStructure);		//��ʼ��GPIO 

	GPIO_ResetBits(GPIOA,GPIO_Pin_0);//PC8��ʼ״̬Ϊ�͵�ƽ����ʱ��ͼ	
}

/*��ȡ����*/
int32_t sr04_get_distance(void)
{
	uint32_t t;	
		
	//PC8�ߵ�ƽ
	GPIO_SetBits(TRIG_PORT,TRIG_PIN);
	delay_us(20);	//����10us����
	GPIO_ResetBits(TRIG_PORT,TRIG_PIN);//PC8�͵�ƽ
	
	while(!GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN));	//�ȴ��ߵ�ƽ
	
	//�ȴ�PC9���ָߵ�ƽ
	t=0;
	while(ECHO_PIN==0)
	{	
		//��ʱ����
		t++;
		delay_us(1);
		
		//�����ʱ���ͷ���һ��������
		if(t >= 10000)
			return -1;
	
	}

	//�����ߵ�ƽ��ʱ��
	t=0;
	while(GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN))
	{	
		t++;
		delay_us(9);	//9us == 3mm
		
		//�����ʱ���ͷ���һ��������
		if(t >= 10000)
			return -2;
	}
		
	//���ڲ�����ʱ�䣬���ǳ������ӷ��䵽���ص�ʱ��	
	d = t*0.3;
	
	return d;
	
}
////////////////////////////////////////////////////////////////////////
