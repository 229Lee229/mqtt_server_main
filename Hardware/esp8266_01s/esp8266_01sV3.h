#ifndef __ESP8266_01SV3_H
#define __ESP8266_01SV3_H

#define WIFI_ID				"ChinaNet_PaRf"
#define WIFI_PASSWORD		"gj5qmdxe"

// void ESP8266_Init(void);
int USART2_ReceiveString(char *buffer, int buffer_size, int timeout_ms);

// ����ATָ��ȴ���Ӧ
int sendATCommand(char *cmd, char *response, int retry_count, int timeout_ms) ;
// ����WiFi����
int connectWiFi(char *ssid, char *password);

// Ping����IP����
int pingPublicIP(void);
#endif
