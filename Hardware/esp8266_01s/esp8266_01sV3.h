#ifndef __ESP8266_01SV3_H
#define __ESP8266_01SV3_H

#define WIFI_ID				"ChinaNet_PaRf"
#define WIFI_PASSWORD		"gj5qmdxe"

// void ESP8266_Init(void);
int USART2_ReceiveString(char *buffer, int buffer_size, int timeout_ms);

// 发送AT指令并等待响应
int sendATCommand(char *cmd, char *response, int retry_count, int timeout_ms) ;
// 连接WiFi函数
int connectWiFi(char *ssid, char *password);

// Ping公网IP函数
int pingPublicIP(void);
#endif
