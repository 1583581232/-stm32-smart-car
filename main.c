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

extern char USART1_RX_BUF[128];     	        // 接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART1_RX_STA;       					// 接收状态标记	
extern u16 USART1_RX_num;						// 接收计数偏移

extern char USART3_RX_BUF[128];     	        // 接收缓冲,最大USART_REC_LEN个字节.
extern u16 USART3_RX_STA;       					// 接收状态标记	
extern u16 USART3_RX_num;						// 接收计数偏移

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
	
	//串口1,波特率115200bps,开启接收中断;串口3,波特率9600bps,开启接收中断
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

	//设置中断优先级分组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//系统定时器初始化，时钟源来自HCLK，且进行8分频，系统定时器时钟频率=168MHz/8=21MHz
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	
	
		
	while(1)
	{		
		 Directive();
	}
		
}

//编写定时器2 的 中断服务函数- 函数名到startup_stm32f10x_md.s文件中找
void TIM2_IRQHandler(){		// 100us产生一次

	static u8 cycle=0;
	//判断是否为定时完成的中断请求信号触发
	if(  TIM_GetITStatus( TIM2, TIM_IT_Update) == SET )
	{
		if(cycle>100)
			cycle=0;
		else
			cycle++;
		
		if(PWM_left<cycle)
			GPIO_SetBits(GPIOD,GPIO_Pin_4);			// ENA输出高电平		1
		else
			GPIO_ResetBits(GPIOD,GPIO_Pin_4);		// ENA输出低电平		0
		
		if(PWM_right<cycle)
			GPIO_SetBits(GPIOD,GPIO_Pin_2);			// ENB输出高电平		1
		else
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);		// ENB输出低电平		1
		
		//清空中断触发的标志
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

void USART1_IRQHandler(void){             						// 串口1中断服务程序 接收字符串{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  			// 接收中断(接收到的数据必须是0x0d 0x
	{
		USART1_RX_BUF[USART1_RX_num++] = USART_ReceiveData(USART1);	// 读取接收到的数据
        
        //将接收到的数据转发到串口1显示
        USART_SendData(USART1,USART1_RX_BUF[USART1_RX_num-1]);	
        while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);   
        
        //检测到换行符或接收的数据满的时候则发送数据		
		if(USART1_RX_BUF[USART1_RX_num-1] == '\n' || USART1_RX_num >= (sizeof USART1_RX_BUF)-1) 	//不
		{
			USART1_RX_STA = 1;										// 遇到换行表示接收到一整个字符串 中
//			USART1_RX_num = 0;										// 接收偏移重新计数
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
		//从串口2接收数据
		USART3_RX_BUF[USART3_RX_num++]=USART_ReceiveData(USART3);

        //将接收到的数据转发到串口1显示
		USART_SendData(USART1,USART3_RX_BUF[USART3_RX_num-1]);	
        while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);   
		
		//检测到换行符或接收的数据满的时候则发送数据
		if(USART3_RX_BUF[USART3_RX_num-1]=='\n' || USART3_RX_num>=(sizeof USART1_RX_BUF)-1)
		{
			
			USART1_SendBytes(USART3_RX_BUF,USART3_RX_num);
			USART3_RX_STA = 1;		
			USART3_RX_num = 0;
            
		}

	}
}