#include "My_include.h"



// 6/16 发送AT指令不能正常收到OK


#ifdef DEBUG_esp8266_V2

void ESP8266_GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 配置CH_PD引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置RST引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//void ESP8266_Init(void) {
//	
//	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
//    Delay_ms_JKD(500);
//    GPIO_SetBits(GPIOA, GPIO_Pin_5);
//	GPIO_SetBits(GPIOA, GPIO_Pin_4);

//	
//	
//	// Check AT
//	if (!ESP8266_SendCommand("AT\r\n", "OK", 9000, 3)) {
//    	// USART_SendString(USART_PC_PORT,"1_AT_FAIL!\r\n");// 
//		printf("AT测试失败\n");
//        return;
//    }
//	#ifdef DEBUG_ESP8266_Init
//    	USART_SendString(USART_PC_PORT,"1_AT_OK!\r\n");// 
//    #endif
//	if (!ESP8266_SendCommand("AT+CWMODE=1\r\n", "OK", 9000, 3)) {
//        printf("设置ESP8266为STA模式失败\n");
//        return;
//    }
//	#ifdef DEBUG_ESP8266_Init
//    	USART_SendString(USART_PC_PORT,"2_CWMODE_OK!\r\n");// 
//    #endif

//    // 连接WiFi
//    if (!ESP8266_SendCommand("AT+CWJAP=\"WIFI_ID\",\"WIFI_PASSWORD\"\r\n", "WIFI CONNECTED", 9000, 3)) {
//        printf("连接WiFi失败\n");
//        return;
//    }
//	#ifdef DEBUG_ESP8266_Init
//    	USART_SendString(USART_PC_PORT,"3_CWJAP_OK!\r\n");// 
//    #endif

//    // Ping目标网站以验证连接
//    if (!ESP8266_SendCommand("AT+PING=\"www.example.com\"\r\n", "OK", 9000, 3)) {
//        printf("Ping 测试失败\n");
//        return;
//    }

//    printf("ESP8266 初始化成功并连接到网络\n");
//	
//	
//	
////    // 发送AT指令以确保ESP8266工作正常
////    if (!ESP8266_SendCommand("AT\r\n", "OK", 5000)) {
////        // 错误处理
////		#ifdef DEBUG_ESP8266_Init
////			USART_SendString(USART_PC_PORT,"1_AT_Fail!\r\n");
////		#endif

////        return;
////    }
////	#ifdef DEBUG_ESP8266_Init
////		USART_SendString(USART_PC_PORT,"1_AT_OK!\r\n");
////	#endif
////    // 设置ESP8266为STA模式
////    if (!ESP8266_SendCommand("AT+CWMODE=1\r\n", "OK", 5000)) {
////        // 错误处理
////        return;
////    }
////	#ifdef DEBUG_ESP8266_Init
////		USART_SendString(USART_PC_PORT,"2_CWMODE=1_OK!\r\n");
////	#endif

////    // 连接WiFi
////    if (!ESP8266_SendCommand("AT+CWJAP=\"WIFI_ID\",\"WIFI_PASSWORD\"\r\n", "WIFI_CONNECTED", 7000)) {
////        // 错误处理
////        return;
////    }
////	#ifdef DEBUG_ESP8266_Init
////		USART_SendString(USART_PC_PORT,"3_CWJAP_OK!\r\n");
////	#endif

//}


int check_response(const char* expected_response, int timeout_ms) {
    char buffer[256];
    USART2_ReceiveString(buffer, 256, timeout_ms);
    return strstr(buffer, expected_response) != NULL;
}


//int ESP8266_SendCommand(const char* command, const char* expected_response, int timeout_ms) {
//    USART2_SendString(command);
//    return check_response(expected_response, timeout_ms);
//}

bool ESP8266_SendCommand(const char* command, const char* expected_response, int timeout_ms, int retry_count) {
    for (int i = 0; i < retry_count; i++) {
        USART2_SendString(command);
        if (check_response(expected_response, timeout_ms)) {
            return true;
        }
    }
    return false;
}

#endif

