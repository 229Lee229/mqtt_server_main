#include "My_include.h"



#ifdef DEBUG_esp8266_V3


//void ESP8266_Init(void){
//		// 初始化ESP8266
//    if (!sendATCommand("AT", "OK", 3, 7000)) {
//        // 初始化失败
//		printf("AT初始化失败\r\n");
//        while (1);
//    }
//    printf("AT初始化成功\r\n");

//    // 连接WiFi
//    if (!connectWiFi(WIFI_ID, WIFI_PASSWORD)) {
//        // WiFi连接失败
//        while (1);
//    }
//    
//    // Ping公网IP
//    if (!pingPublicIP()) {
//        // Ping失败
//        while (1);
//    }

//	
//	
//}


// 串口2接收函数，带超时
int USART2_ReceiveString(char *buffer, int buffer_size, int timeout_ms) {
    int index = 0;
    while (timeout_ms > 0) {
        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
            buffer[index++] = USART_ReceiveData(USART2);
            if (index >= buffer_size - 1) break; // 防止溢出
        }
        Delay_ms_JKD(1);
        timeout_ms--;
    }
    buffer[index] = '\0'; // 字符串结尾
    return (timeout_ms > 0) ? 1 : 0; // 返回是否成功接收到数据
}


// 发送AT指令并等待响应
int sendATCommand(char *cmd, char *response, int retry_count, int timeout_ms) {
    char buffer[128];
    for (int i = 0; i < retry_count; i++) {
        USART2_SendString(cmd);
        USART2_SendString("\r\n");
        if (USART2_ReceiveString(buffer, sizeof(buffer), timeout_ms)) {
            if (strstr(buffer, response) != NULL) {
                return 1; // 成功
            }
        }
    }
    return 0; // 失败
}

// 连接WiFi函数
int connectWiFi(char *ssid, char *password) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    return sendATCommand(cmd, "OK", 3, 7000);
}



// Ping公网IP函数
int pingPublicIP(void) {
    return sendATCommand("AT+PING=\"8.8.8.8\"", "OK", 3, 7000);
}


#endif
