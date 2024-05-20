#include"motor - 2.0.h"
#include "sys.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"


extern char USART1_RX_BUF[128];     	        // ���ջ���,���USART_REC_LEN���ֽ�.
extern u16 USART1_RX_STA;       					// ����״̬���	
extern u16 USART1_RX_num;						// ���ռ���ƫ��

extern char USART3_RX_BUF[128];     	        // ���ջ���,���USART_REC_LEN���ֽ�.
extern u16 USART3_RX_STA;       					// ����״̬���	
extern u16 USART3_RX_num;						// ���ռ���ƫ��

int Pump_init(void)
{
	//	1	�ṹ���������
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//	2 	��Ӧ��ʱ��ʹ��
	/* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		// ʹ�ܶ�Ӧ��ĵ�Դ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);		// ʹ�ܶ�Ӧ��ĵ�Դ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);		// ʹ�ܶ�Ӧ��ĵ�Դ
	
	//	3	��������	
	/* Configure  in output pushpull mode */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//	��ƽ�л����ٶ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			// �������

	//	4	��ʼ��
		//	��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;					// ��Ӧ�������
	GPIO_Init(GPIOC, &GPIO_InitStructure);						// ͨ���ṹ�� ��ʼ�� C ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4;					// ��Ӧ�������
	GPIO_Init(GPIOD, &GPIO_InitStructure);						// ͨ���ṹ�� ��ʼ�� C ��
	
		// ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_6;					// ��Ӧ�������
	GPIO_Init(GPIOD, &GPIO_InitStructure);						// ͨ���ṹ�� ��ʼ�� D ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;					// ��Ӧ�������
	GPIO_Init(GPIOG, &GPIO_InitStructure);						// ͨ���ṹ�� ��ʼ�� G ��
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);					// ������͵�ƽ
	GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_4);		// ������͵�ƽ
	
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);					// ������͵�ƽ
	GPIO_ResetBits(GPIOD,GPIO_Pin_2 | GPIO_Pin_6);		// ������͵�ƽ
}

int GO (void)
{
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_4);			// ��PD4����ߵ�ƽ		1
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		// ��PC11����ߵ�ƽ		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// ��PD0����͵�ƽ		0
	
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ������ߵ�ƽ		1
	GPIO_SetBits(GPIOD,GPIO_Pin_6);			// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// ������͵�ƽ		0
}

int BACK(void)
{
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_4);			// ��PD4����ߵ�ƽ		1
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// ��PC11����ߵ�ƽ		0
	GPIO_SetBits(GPIOD,GPIO_Pin_0);			// ��PD0����͵�ƽ		1
	
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);		// ������ߵ�ƽ		0
	GPIO_SetBits(GPIOG,GPIO_Pin_9);			// ������͵�ƽ		1
}
int LEFT(void)	// ��
{
	//	��
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// ������ߵ�ƽ		0
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// ������ߵ�ƽ		0
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// ������͵�ƽ		0
	
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ������ߵ�ƽ		1
	GPIO_SetBits(GPIOD,GPIO_Pin_6);			// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// ������͵�ƽ		0
}
int BIGLEFT(void)	// ��
{
	//	��
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// ������ߵ�ƽ		0
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// ������ߵ�ƽ		0
	GPIO_SetBits(GPIOD,GPIO_Pin_0);		 // ������͵�ƽ		0
	
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ������ߵ�ƽ		1
	GPIO_SetBits(GPIOD,GPIO_Pin_6);			// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// ������͵�ƽ		0
}
int RIGHT(void)	// ��
{
	//	��
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// ������ߵ�ƽ		0
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// ������͵�ƽ		0
	
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);			// ������ߵ�ƽ		0
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// ������͵�ƽ		0
}
int BIGRIGHT(void)	// ��
{
	//	��
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// ������ߵ�ƽ		0
	GPIO_SetBits(GPIOC,GPIO_Pin_11);		// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// ������͵�ƽ		0
	
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ������ߵ�ƽ		1
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);			// ������ߵ�ƽ		0
	GPIO_SetBits(GPIOG,GPIO_Pin_9);		// ������͵�ƽ		0
}
int STOP(void)
{
	//	��
	//GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// ��PD4����͵�ƽ		0
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);		// ��PC11����͵�ƽ		0
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);		// ��PD0����͵�ƽ		0
	
	//	��
	//GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ������ߵ�ƽ		0
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);		// ������ߵ�ƽ		0
	GPIO_ResetBits(GPIOG,GPIO_Pin_9);		// ������͵�ƽ		0
}

int SanxianControl(void){
		while((strstr(USART1_RX_BUF, "sxxl") != NULL)||(strstr(USART3_RX_BUF, "sxxl") != NULL)){
		
		if((PGin(4) == 0 && PGin(6) == 0 && PGin(8) == 1)&&(PGin(2) == 1 && PCin(7) ==1))
		{
			GO();
			Anquan();
		}
		else if(((PGin(4) == 1 && PGin(6) == 0) && (PGin(8) == 0 || 
			PGin(8) ==1)) && (PGin(2) == 1 && PCin(7) == 1))
		{
			RIGHT();
			Noanquan();
		}
		else if(((PGin(4) == 0 && PGin(6) == 1 )&&( PGin(8) == 0 || 
			PGin(8) ==1)) && (PGin(2) == 1 && PCin(7) == 1))
		{
			LEFT();
			Noanquan();
		}
		else if(((PGin(4) == 1 && PGin(6) == 1 && PGin(8) == 1) ||
		(PGin(4) == 0 && PGin(6) == 0 && PGin(8) == 0)) &&
		((PGin(2) == 0 || PGin(2) == 1)||(PCin(7) == 0 || PCin(7) == 1)))
		{
			BIGLEFT();
			Minianquan();
		}	
		else{
			STOP();
			Minianquan();
		}

		}
		return 0;
}


int HongwaiControl(void){
	while((strstr(USART1_RX_BUF, "hwbz") != NULL)||(strstr(USART3_RX_BUF, "hwbz") != NULL)){
		if(PGin(2) == 1 && PCin(7) == 1){
		GO();
		Anquan();
		}
		else if(PGin(2)==0&&PCin(7)== 1){
		BACK();
		BIGRIGHT();
		delay_ms(700);
		Noanquan();
		}
		else if(PGin(2)==1&&PCin(7) == 0){
		BACK();
		BIGLEFT();
		delay_ms(700);
		Noanquan();
		}
		else{
		STOP();
		BACK();
		delay_ms(1000);
		Noanquan();
		}
	}
}