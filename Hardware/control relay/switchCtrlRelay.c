#include "My_include.h"
extern u8 Time_doorLock;

// ���ؿ����Ŵ�   PA15	 PB3   PB4
void switchCtrlRelay_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);			// ������ӳ��
	

	
	GPIO_Pin_Init(PA15, GPIO_Mode_IPU);
	GPIO_Pin_Init(PB3 , GPIO_Mode_IPU);
	GPIO_Pin_Init(PB4 , GPIO_Mode_IPU); 

	
	// Init EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
    // EXTI Line0 
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;		// �����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			// �ж϶���
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
}


void EXTI15_10_IRQHandler(){
	u16 timeout = 50000;
	if((EXTI_GetITStatus(EXTI_Line15) == SET) /* && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0) */ ){			
		
		
		
		/* ���¿��غ�,ʹ��TIM4,��ʼ���� */
		PBout(14) = 0;				// �Ŵ�ʧ�� ���Կ���
		EXTI_ClearITPendingBit(EXTI_Line15);
		while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
			;
		Time_doorLock = 0;			// ����12s��ʱ���ٴΰ��¿��� �����¼�ʱ
		TIM_Cmd(TIM4, ENABLE);
		// Delay_ms_JKD(30);		// ������
		while(timeout--){
			;
		}
	}

}
