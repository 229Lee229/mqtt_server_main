#include "My_include.h"
#define doorLock_countDown_MAX	6				// ���ؿ����Ŵż�ʱʱ��


u8 Time_doorLock = 0;			// ���³��ſ��غ� �Ŵ�25s���� ���¿��ؿ�ʼ����





void timer_doorLock_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision 	= 	TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode 		= 	TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period 			= 	(36000 - 1);						// ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler 		= 	(2000 - 1);							// PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 	0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	// TIM_InternalClockConfig(TIM4);						// ѡ���ڲ�ʱ��Դ
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
	
	
	// �ڰ��¿���ǰ ��ʹ��!!!!
	TIM_Cmd(TIM4, DISABLE);

	
}


void TIM4_IRQHandler(void){
	char stringDebug[20];
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

		Time_doorLock++;
		
		#ifdef 	DEBUG_DoorLock
//			Delay_ms_JKD(10);
//			PCout(13) = !PCout(13);	
//			Delay_ms_JKD(10);
			PCout(13) = !PCout(13);	
			sprintf(stringDebug,"%d\r\n",Time_doorLock);
			printf("%s",stringDebug);
		#endif
		

		if(Time_doorLock == doorLock_countDown_MAX){
			Time_doorLock = 0;
			// 12sʱ�䵽 �Ŵ��ϵ� ��ʹ��TIM4 �Զ������ر�
			PBout(14) = 1;				// �Ŵ��ϵ�  ���ɿ���
			TIM_Cmd(TIM4, DISABLE);
 		}
	}
}





