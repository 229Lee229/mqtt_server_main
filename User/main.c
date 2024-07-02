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
	V1.0		 5/04/2024	  Lee				S0805ʹ3.3V����5Vͨ�� Rc = 5k Rb = 1k  ʹ��һ�������� ���������Ϊ�������
												5Vͨ�Ͽ��Ƽ̵���  �̵��������Ŵ�(12V)ͨ�� 
												���ڿ���ʹ�ÿ��ض��ŴŽ���ʧ��(����)	����ʱ��Ϊ12s	
												�ڰ��¿��ź�(ʹ��TIM4)��������ʼ���� ����12s�����̵���(ʧ��TIM4(��ʡcpu��Դ))->�Ŵ��ϵ�(����)
												���ڿ��ŵ�ʱ�䵱��,�����ٰ��¿���,�����¼���12s
												
    V1.1		 5/30/2024    Lee				��ʼ��ֲesp8266��������-Ұ�� ���dmt��ʱ�����뽭�ƴ���ʱ������ͻ  ���߸ĳ�������������														
	V1.2		 6/03/2024    Lee               �����ж����ȼ�
	
	V1.3 		 6/17/2024	  Lee				8266V3���ӷ��ͺ������־λ  ���Կ���ʹ��usart2����ָ���esp8266  ���յ�������ͨ��usart1�����pc��
	V1.4		 6/27/2024    Lee				�жϺ������ܼ���ʱ����,����жϺ��ܷ��������������,�����Ƿ���Ҫ��������
												�ڰ�����������,ʹ��whileѭ��,�����Լ�����timeout,�Դ˴ﵽ���ݵ���ʱЧ��.
 ***********************************************************************/

#include "stm32f10x.h"                  // Device header
#include "My_include.h"


// bool IfEnableDoorLock = false;


// extern u8 Time_doorLock;



/*
	usart1 �ض��� ���Դ���
	usart2 wifi����

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
	
	Usart_PC_Init(USART_PC_BAUDRATE);						// PC����
	Usart_ESP8266_Init(USART_2_BAUDRATE);					// esp8266
	// Usart_ESP8266_Init(USART_ESP8266_BAUDRATE);				// esp8266 ����esp8266�������г�ʼ��
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
//            // ͨ��USART1����ESP8266�ش�������
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
		PBout(14) = 0;				// �Ŵ�ʧ�� ���Կ���
		Delay_ms_JKD(500);
		PBout(14) = 1;				// �Ŵ�ʧ�� ���Կ���
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


