#include "My_include.h"



/*
	PB12
    PB13	-> 	  �Ŵ�
    PB14
	PB15
*/ 


void relay5V_Init(void){
	// ��·�̵���
	GPIO_Pin_Init(PB12 ,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PB13 ,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PB14 ,GPIO_Mode_Out_PP);
	GPIO_Pin_Init(PB15 ,GPIO_Mode_Out_PP);				
	
	// ���ؿ����ŴŶ��ݿ������� ��ʼ��
	switchCtrlRelay_Init();

	// ʹ�÷������ �ߵ�ƽ����
	// �̵�������ͨ��  IO��ʼ״̬Ϊ�ߵ�ƽ(�����ܵ�ͨ5v����,�̵���������) 5V����->������
	PBout(12) = 1;
	PBout(13) = 1;
	PBout(14) = 1;
	PBout(15) = 1;	
} 


