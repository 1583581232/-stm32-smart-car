#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "beep.h"
#include "ir.h"
#include "sr04.h"
#include "usart.h"
#include "sys.h"
#include "motor - 2.0.h"
#include "timer.h"
#include "delay.h"

extern char USART1_RX_BUF[128];     	        // ���ջ���,���USART_REC_LEN���ֽ�.
extern u16 USART1_RX_STA;       					// ����״̬���	
extern u16 USART1_RX_num;						// ���ռ���ƫ��

extern char USART3_RX_BUF[128];     	        // ���ջ���,���USART_REC_LEN���ֽ�.
extern u16 USART3_RX_STA;       					// ����״̬���	
extern u16 USART3_RX_num;						// ���ռ���ƫ��

u8 PWM_left=0;
u8 PWM_right=0;

int Directive(void){
	
	if (USART1_RX_STA == 1)
		{
		printf("data:%s\n",USART1_RX_BUF);

		if(strstr(USART1_RX_BUF, "go" ) != NULL)
		{   
			Anquan();
			GO();  
		}

		else if (strstr(USART1_RX_BUF, "left") != NULL)
		{	
			Noanquan();
			BIGLEFT();
		}
		else if (strstr(USART1_RX_BUF, "right") != NULL)
		{
			Noanquan();
			BIGRIGHT();
		
		}
		else if (strstr(USART1_RX_BUF, "back") != NULL)
		{
			BACK();
			Minianquan();
			
		}
		else if (strstr(USART1_RX_BUF, "stop") != NULL)
		{
			STOP();
			Minianquan();
		}
		else if (strstr(USART1_RX_BUF, "off") != NULL)
		{
			Anquan();
		}
		else if (strstr(USART1_RX_BUF, "on") != NULL)
		{
			Noanquan();
		}
		else if(strstr(USART1_RX_BUF, "sxxl") != NULL)
		{   
			SanxianControl();	
		}
		else if(strstr(USART1_RX_BUF, "hwbz") != NULL)
		{   
			HongwaiControl();
							
		}
		else if(strstr(USART1_RX_BUF, "moshushou") != NULL)
		{
			Moshushou();
		}
		else if(strstr(USART1_RX_BUF, "zcrk") != NULL)
		{
			Ruku();
		}
		memset(USART1_RX_BUF, 0, strlen(USART1_RX_BUF));
		USART1_RX_STA = 0;
		}
		
		if (USART3_RX_STA == 1)
		{
		printf("data:%s\n",USART3_RX_BUF);
		
		if(strstr(USART3_RX_BUF, "go") != NULL)
		{   
			Anquan();
			GO(); 			
		}
		else if (strstr(USART3_RX_BUF, "left") != NULL)
		{
			Noanquan();
			BIGLEFT();
		}
		else if (strstr(USART3_RX_BUF, "right") != NULL)
		{
			Noanquan();
			BIGRIGHT();
		
		}
		else if (strstr(USART3_RX_BUF, "back") != NULL)
		{
			BACK();
			Minianquan();
		}
		else if (strstr(USART3_RX_BUF, "stop") != NULL)
		{
			STOP();
			Minianquan();
		}
		else if (strstr(USART3_RX_BUF, "off") != NULL)
		{
			Anquan();
		}
		else if (strstr(USART3_RX_BUF, "on") != NULL)
		{
			Noanquan();
		}
		else if(strstr(USART3_RX_BUF, "sxxl") != NULL)
		{   
			SanxianControl();
		}
		else if(strstr(USART3_RX_BUF, "hwbz") != NULL)
		{   
			HongwaiControl();	
		}
		else if(strstr(USART3_RX_BUF, "moshushou") != NULL)
		{
			Moshushou();
		}
		else if(strstr(USART3_RX_BUF, "zcrk") != NULL)
		{
			Ruku();
		}
		
		memset(USART3_RX_BUF, 0, strlen(USART3_RX_BUF));
		USART3_RX_STA = 0;
		}
		delay_ms(10);	
	}

int main(void) {
	
	//����1,������115200bps,���������ж�;����3,������9600bps,���������ж�
	USART1_Init(115200);
	USART3_Init(9600);
	SysTick_Init();
	
	led_init();
	fengming_init();
	hongwai_init();
	sanxian_init();
	Pump_init();
	timer_init();
	
	sr04_init();
	sr04_get_distance();
	
	PWM_left=50;
	PWM_right=50;
	PEout(14) = 1;

	//�����ж����ȼ�����2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//ϵͳ��ʱ����ʼ����ʱ��Դ����HCLK���ҽ���8��Ƶ��ϵͳ��ʱ��ʱ��Ƶ��=168MHz/8=21MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	
	
		
	while(1)
	{		
		 Directive();
	}
		
}

//��д��ʱ��2 �� �жϷ�����- ��������startup_stm32f10x_md.s�ļ�����
void TIM2_IRQHandler(){		// 100us����һ��

	static u8 cycle=0;
	//�ж��Ƿ�Ϊ��ʱ��ɵ��ж������źŴ���
	if(  TIM_GetITStatus( TIM2, TIM_IT_Update) == SET )
	{
		if(cycle>100)
			cycle=0;
		else
			cycle++;
		
		if(PWM_left<cycle)
			GPIO_SetBits(GPIOD,GPIO_Pin_4);			// ENA����ߵ�ƽ		1
		else
			GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// ENA����͵�ƽ		0
		
		if(PWM_right<cycle)
			GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ENB����ߵ�ƽ		1
		else
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);		// ENB����͵�ƽ		1
		
		//����жϴ����ı�־
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void USART1_IRQHandler(void){             						// ����1�жϷ������ �����ַ���{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  			// �����ж�(���յ������ݱ�����0x0d 0x
	{
		USART1_RX_BUF[USART1_RX_num++] = USART_ReceiveData(USART1);	// ��ȡ���յ�������
        
        //�����յ�������ת��������1��ʾ
        USART_SendData(USART1,USART1_RX_BUF[USART1_RX_num-1]);	
        while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);   
        
        //��⵽���з�����յ���������ʱ����������		
		if(USART1_RX_BUF[USART1_RX_num-1] == '\n' || USART1_RX_num >= (sizeof USART1_RX_BUF)-1) 	//��
		{
			USART1_RX_STA = 1;										// �������б�ʾ���յ�һ�����ַ��� ��
//			USART1_RX_num = 0;										// ����ƫ�����¼���
            USART3_SendBytes(USART1_RX_BUF,USART1_RX_num);
            USART1_RX_num=0;
		}
		
	}
}

void USART3_IRQHandler(void){	
    //uint8_t d;
	/* USART in Receiver mode */
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)  
	{
		//�Ӵ���2��������
		USART3_RX_BUF[USART3_RX_num++]=USART_ReceiveData(USART3);

        //�����յ�������ת��������1��ʾ
		USART_SendData(USART1,USART3_RX_BUF[USART3_RX_num-1]);	
        while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);   
		
		//��⵽���з�����յ���������ʱ����������
		if(USART3_RX_BUF[USART3_RX_num-1]=='\n' || USART3_RX_num>=(sizeof USART1_RX_BUF)-1)
		{
			
			USART1_SendBytes(USART3_RX_BUF,USART3_RX_num);
			USART3_RX_STA = 1;		
			USART3_RX_num = 0;
            
		}

	}
}