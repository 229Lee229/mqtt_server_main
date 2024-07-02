#include "My_include.h"



#ifdef DEBUG_esp8266_V3


//void ESP8266_Init(void){
//		// ��ʼ��ESP8266
//    if (!sendATCommand("AT", "OK", 3, 7000)) {
//        // ��ʼ��ʧ��
//		printf("AT��ʼ��ʧ��\r\n");
//        while (1);
//    }
//    printf("AT��ʼ���ɹ�\r\n");

//    // ����WiFi
//    if (!connectWiFi(WIFI_ID, WIFI_PASSWORD)) {
//        // WiFi����ʧ��
//        while (1);
//    }
//    
//    // Ping����IP
//    if (!pingPublicIP()) {
//        // Pingʧ��
//        while (1);
//    }

//	
//	
//}


// ����2���պ���������ʱ
int USART2_ReceiveString(char *buffer, int buffer_size, int timeout_ms) {
    int index = 0;
    while (timeout_ms > 0) {
        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
            buffer[index++] = USART_ReceiveData(USART2);
            if (index >= buffer_size - 1) break; // ��ֹ���
        }
        Delay_ms_JKD(1);
        timeout_ms--;
    }
    buffer[index] = '\0'; // �ַ�����β
    return (timeout_ms > 0) ? 1 : 0; // �����Ƿ�ɹ����յ�����
}


// ����ATָ��ȴ���Ӧ
int sendATCommand(char *cmd, char *response, int retry_count, int timeout_ms) {
    char buffer[128];
    for (int i = 0; i < retry_count; i++) {
        USART2_SendString(cmd);
        USART2_SendString("\r\n");
        if (USART2_ReceiveString(buffer, sizeof(buffer), timeout_ms)) {
            if (strstr(buffer, response) != NULL) {
                return 1; // �ɹ�
            }
        }
    }
    return 0; // ʧ��
}

// ����WiFi����
int connectWiFi(char *ssid, char *password) {
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    return sendATCommand(cmd, "OK", 3, 7000);
}



// Ping����IP����
int pingPublicIP(void) {
    return sendATCommand("AT+PING=\"8.8.8.8\"", "OK", 3, 7000);
}


#endif
