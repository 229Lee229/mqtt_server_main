
/**********************************************************************
 * flie：  USART_Init
 * brief： 配置usart1 usart2 usart3 
			收发模式,无硬件流控制,一位停止位,无奇偶检验位,字长为8位数据格式
 * Input： u32 bound
 * Output：无
 * Retval：无
 *
 * History--------------------------------------------------------------
 * Version       Date         Name    			Changes and comments
 *=====================================================================
	V1.0		 3/25/2024	  Lee			    配置usart1|2|3非空中断 
 ***********************************************************************/

#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "stdio.h"

static volatile uint16_t usart2_rx_index = 0;
static volatile uint8_t usart2_data_ready = 0;
volatile char usart2_rx_buffer[256];

#if SYSTEM_SUPPORT_OS
#include "includes.h"
#endif

#if 1
#pragma import(__use_no_semihosting)             
// 标准库需要的支持函数             
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) 
{ 
	x = x; 
} 
// 重定义fputc函数
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0)
			;	//循环发送, 直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
	




#if EN_USART1_RX		/*` 如果使能了接收 */
/* 串口1 中中断服务程序 */
/* 注意 读取USARTx->SR能避免莫名其妙的错误 */
u8 USART1_RX_BUF[USART1_REC_LEN];			/* 接收缓冲 最大USART_REC_LEN个字节 */
/* 接收状态 */
/* bit15		接收完成标志   */
/* bit14		接收到0x0d     */
/* bit13~0		接收到有效字符 */		
u16 USART1_RX_STA = 0;

void Usart1_Init(u32 bound){
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	USART_DeInit(USART1);			// 复位串口1	
	
	
	/* GPIOA_9  USART1_TX  AF_PP */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	/* GPIOA_10 USART1_RX IN_FLOATING */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	
	/* USART1 NVIC Init */
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	
	/* USART1 Init */
	USART_InitTypeDef USART_InitStructre;
	USART_InitStructre.USART_BaudRate = bound;
	USART_InitStructre.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructre.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	/* Sending and Receiving mode */
	USART_InitStructre.USART_Parity = USART_Parity_No;				/* No Parity bit */
	USART_InitStructre.USART_StopBits = USART_StopBits_1;
	USART_InitStructre.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructre);
	// 6/18 USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);					/* 开启串口中断, 非空中断 */
	
	USART_Cmd(USART1,ENABLE);		/* Enable Serial Port */
}



//void USART1_IRQHandler(void){
//	u8 Res;
//#if SYSTEM_SUPPORT_OS
//	OSInitEnter();
//#endif	
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){			/* 接收中断 接收到的数据必须是0x0d\r 0x0a\n结尾 */
//		Res = USART_ReceiveData(USART1);		/* 读取接收到的数据 */
//		if((USART1_RX_STA&0x8000) == 0) {		/* 接收未完成 */
//			if(USART1_RX_STA&0x4000){			/* 接收到0x0d\r */
//				if(Res != 0x0a)USART1_RX_STA = 0;		/* 接收错误, 重新开始 */
//				else USART1_RX_STA |= 0x8000;			/* 接收完毕 */
//			}else{				/* 未收到0x0d\r */
//				if(Res == 0x0d)	USART1_RX_STA |= 0x4000;
//				else{
//					USART1_RX_BUF[USART1_RX_STA&0x3FFF] = Res;
//					USART1_RX_STA++;
//					if(USART1_RX_STA > (USART1_REC_LEN-1))		USART1_RX_STA = 0; /* 接收数据错误, 重新接收 */
//				}
//			}
//			
//			
//		}
//	}
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//	OSIntExit();  											 
//#endif
//	
//}

#endif

#if EN_USART2_RX
u8  USART2_RX_BUF[USART2_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART2_RX_STA = 0;         			//接收状态标记	

void Usart2_Init(u32 bound)
{	
	GPIO_InitTypeDef GPIO_InitStrue;
	USART_InitTypeDef USART_InitStrue;
	
	// 外设使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);  //复位串口2 -> 可以没有
	
	// 初始化 串口对应IO口  TX-PA2  RX-PA3
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	// 初始化 串口模式状态
	USART_InitStrue.USART_BaudRate=bound; // 波特率
	USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None; // 无硬件流控制
	USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
	USART_InitStrue.USART_Parity=USART_Parity_No; // 没有奇偶校验
	USART_InitStrue.USART_StopBits=USART_StopBits_1; // 一位停止位
	USART_InitStrue.USART_WordLength=USART_WordLength_8b; // 每次发送数据宽度为8位
	USART_Init(USART2,&USART_InitStrue);
	
	USART_Cmd(USART2,ENABLE);//使能串口
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//开启接收中断
	
//	// 初始化 中断优先级
	NVIC_InitTypeDef NVIC_InitStrue;
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStrue.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStrue);
}


//void USART2_IRQHandler(void) {
//    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//        char received_char = USART_ReceiveData(USART2);
//        USART_SendData(USART1, received_char);
//        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//    }
//}

//void USART2_IRQHandler(void) {
//    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//        char received_char = USART_ReceiveData(USART2);
////        if (received_char != '\r' && received_char != '\n') { // 忽略换行符


////        } 
//			USART_SendString(USART_PC_PORT, &received_char);

//		    // usart2_rx_buffer[usart2_rx_index++] = received_char;
//			
//            // usart2_data_ready = 1;
////		else if (received_char == '\n') { // 完整的行
////            usart2_rx_buffer[usart2_rx_index] = '\0'; // 终止符
////        }
//    }
//}

//void USART2_IRQHandler(void) {
//    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
//        char received_char = USART_ReceiveData(USART2);
//        USART_SendData(USART1, received_char);
//        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//    }
//}


// 清空USART接收缓冲区
void USART_ClearBuffer(USART_TypeDef* USARTx) {
    while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) != RESET) {
        USART_ReceiveData(USARTx);
    }
}
 
//void USART2_IRQHandler(void) // 串口2中断服务函数
//{
//	u8 res;
//	if(USART_GetITStatus(USART2,USART_IT_RXNE)) // 中断标志
//	{
//		res= USART_ReceiveData(USART2);  // 串口2 接收
////		USART_SendData(USART2,res);   // 串口2 发送
//		if((USART2_RX_STA&0x8000)==0)//接收未完成
//		{
//		if(USART2_RX_STA&0x4000)//接收到了0x0d
//			{
//			if(res!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
//			else USART2_RX_STA|=0x8000;	//接收完成了 
//			}
//		else //还没收到0X0D
//			{	
//			if(res==0x0d)USART2_RX_STA|=0x4000;
//			else
//				{
//				USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res ;
//				USART2_RX_STA++;
//				if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		} 
//	}
//}

#endif

#if EN_USART3_RX
u8  USART3_RX_BUF[USART3_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART3_RX_STA = 0;         			//接收状态标记	

void Usart3_Init(u32 bound)
{

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能
 
 	USART_DeInit(USART3);  //复位串口3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口	3
  
 
	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	USART3_RX_STA=0;		//清零
}
 
void USART3_IRQHandler(void) // 串口3中断服务函数
{
	u8 res;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)) // 中断标志
	{
		res= USART_ReceiveData(USART3);  // 串口3 接收
//		USART_SendData(USART2,res);   // 串口3 发送
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
		if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
			if(res!=0x0a)USART3_RX_STA=0;//接收错误,重新开始
			else USART3_RX_STA|=0x8000;	//接收完成了 
			}
		else //还没收到0X0D
		{	
			if(res==0x0d)USART3_RX_STA|=0x4000;
			else
				{
				USART3_RX_BUF[USART2_RX_STA&0X3FFF]=res ;
				USART3_RX_STA++;
				if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
		}
	}	 
	}
}




#endif

/*   // 原始发送程序，带有清除标志位的函数 
void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
	while(DataString[i] != '\0')												//字符串结束符
	{
		USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//等待数据发送成功
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//发送字符后清空标志位
		i++;
	}
}

*/
// 发送字符串到USART			GPT程序，没有清除标志位的函数  6/17
void USART_SendString(USART_TypeDef* USARTx, char* str) {
    while (*str) {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}

// 接收ESP8266的响应
//int USART_ReceiveString(char* buffer, int max_length) {
//    int length = 0;
//    while (length < max_length - 1) {
//        while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//        buffer[length++] = USART_ReceiveData(USART1);
//        if (buffer[length - 1] == '\n') {
//            break;
//        }
//    }
//    buffer[length] = '\0';
//    return length;
//}

// 检查ESP8266的响应
//int check_response(char* expected_response) {
//    char buffer[256];
//    USART_ReceiveString(buffer, 256);
//    return strstr(buffer, expected_response) != NULL;
//}

void USART2_SendString(const char* str) {
    while (*str) {
        USART_SendData(USART2, *str++);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
		// 发送字符后清空标志位		6/17
		USART_ClearFlag(USART_EPS8266_PORT,USART_FLAG_TC);									//发送字符后清空标志位
    }
}

//int USART2_ReceiveString(char* buffer, int max_length, int timeout_ms) {
//    int length = 0;
//    uint32_t start_tick = SysTick->VAL;
//    uint32_t ticks_per_ms = SystemCoreClock / 1000;
//    
//    while (length < max_length - 1) {
//        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
//            buffer[length++] = USART_ReceiveData(USART2);
//            if (buffer[length - 1] == '\n') {
//                break;
//            }
//        }
//        // 检查超时
////        if ((SysTick->VAL - start_tick) / ticks_per_ms > timeout_ms) {
//		if ((start_tick - SysTick->VAL) / ticks_per_ms > timeout_ms) {
//            break;
//        }
//    }
//    buffer[length] = '\0';
//    return length;
//}


