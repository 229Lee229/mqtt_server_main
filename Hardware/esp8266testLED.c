#include "My_include.h"


void LED_GPIO_Config(void){
	GPIO_Pin_Init(PA0,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PA1,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PA2,GPIO_Mode_Out_PP);
	
	PAout(0) = 0;			// ��ʼ��Ĭ��Ϩ��		ע�����ŵ�ƽ
	PAout(1) = 0;
	PAout(2) = 0;
}


