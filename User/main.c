/**********************************************************************
 * flie:  main
 * brief:  
 * Input: 
 * Output:
 * Retval:
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 5/04/2024	  Lee				S0805使3.3V控制5V通断 Rc = 5k Rb = 1k  使用一个三极管 故输出极性为反相输出
												5V通断控制继电器  继电器控制门磁(12V)通断 
												室内可以使用开关对门磁进行失电(开门)	开门时间为12s	
												在按下开门后(使能TIM4)计数器开始工作 计满12s动作继电器(失能TIM4(节省cpu资源))->门磁上电(锁门)
												若在开门的时间当中,有人再按下开关,则重新计数12s
												
    V1.1		 5/30/2024    Lee				开始移植esp8266驱动代码-野火 解决dmt延时函数与江科大延时函数冲突  后者改成作者命名函数														
	V1.2		 6/03/2024    Lee               考虑中断优先级
	
	V1.3 		 6/17/2024	  Lee				8266V3增加发送后清除标志位  可以考虑使用usart2发送指令给esp8266  接收到的数据通过usart1输出到pc端
	V1.4		 6/27/2024    Lee				中断函数不能加延时函数,解决中断后不能返回主程序的问题,考虑是否需要按键消抖
												在按键消抖方面,使用while循环,不断自减变量timeout,以此达到短暂的延时效果.
 ***********************************************************************/

#include "stm32f10x.h"                  // Device header
#include "My_include.h"


// bool IfEnableDoorLock = false;


// extern u8 Time_doorLock;



/*
	usart1 重定向到 电脑串口
	usart2 wifi串口

*/
void RCC_Configuration(void);

// extern uint8_t Serial_RxData;
int main(void){
	RCC_Configuration();
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(GPIOC, &GPIO_InitStructure);
	PCout(13) = 1;
//	
//	
//	
//	
	relay5V_Init();
	timer_doorLock_Init();
	
	Usart_PC_Init(USART_PC_BAUDRATE);						// PC串口
	Usart_ESP8266_Init(USART_2_BAUDRATE);					// esp8266
	// Usart_ESP8266_Init(USART_ESP8266_BAUDRATE);				// esp8266 已在esp8266函数体中初始化
//	USART_ClearBuffer(USART_EPS8266_PORT);
//	USART_SendString(USART_EPS8266_PORT, "AT\r\n");
	// SystemInit();
	// SysTick_Init();
	 //ESP8266_Init();
	//Serial_SendString("AT\r\n");			// usart2
	// Serial_SendString_usart1("??");
//	if(Serial_GetRxFlag() == 1){
//		// Serial_SendString_usart1(Serial_GetRxData());
//		Serial_SendByte_usart1(Serial_GetRxData());
//	}
	// esp8266_init(USART_2_BAUDRATE);
	// ESP8266_Init();
	// MQTT_Init();
	u32 timeCnt = 0;
	while(1){
//        if (usart2_data_ready) {
//            usart2_data_ready = 0;
//            // 通过USART1发送ESP8266回传的数据
//            USART_SendString(USART1, (char*)usart2_rx_buffer);
            // USART_SendString(USART1, "AT\r\n");
//        }
//////		PCout(13) = !PCout(13);
//			USART_SendString(USART2, "AT\r\n");
//			Delay_ms_JKD(1000);
//			USART_SendString(USART_EPS8266_PORT, "AT\r\n");

//		
//		
		// printf("OK!\r\n");
		PBout(14) = 0;				// 门磁失电 可以开门
		Delay_ms_JKD(500);
		PBout(14) = 1;				// 门磁失电 可以开门
		Delay_ms_JKD(500);
		printf("%d\r\n",timeCnt++);

	}
	
	
}


void RCC_Configuration(void) {
    // Enable HSE (High Speed External clock)
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
    
    // Configure PLL
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  // HSE * 9 = 72 MHz
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    
    // Set system clock
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);
    
    // Enable clocks for peripherals
    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}


