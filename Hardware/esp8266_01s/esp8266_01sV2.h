#ifndef __ESP8266_01SV2_H
#define __ESP8266_01SV2_H


#define WIFI_ID				ChinaNet_PaRf
#define WIFI_PASSWORD		gj5qmdxe


//#define WIFI_ID				STM32
//#define WIFI_PASSWORD		12345678


// #define command "AT"

void ESP8266_GPIO_Config(void);
// void ESP8266_Init(void);
int check_response(const char* expected_response, int timeout_ms);
bool ESP8266_SendCommand(const char* command, const char* expected_response, int timeout_ms, int retry_count);
#endif
