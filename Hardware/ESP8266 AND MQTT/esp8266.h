#ifndef __ESP8266_H
#define __ESP8266_H


#define ESP8266_UART_RX_BUF_SIZE            128
#define ESP8266_UART_TX_BUF_SIZE            64
#define ESP8266_EOK                         0   /* 没有错误 */
#define ESP8266_ERROR                       1   /* 通用错误 */
#define ESP8266_ETIMEOUT                    2   /* 超时错误 */
#define ESP8266_EINVAL                      3   /* 参数错误 */
#define BUFFER_SIZE 						100

//#define ESP8266_WIFI_SSID					"STM32"
//#define ESP8266_WIFI_PASSWORD				"12345678"
#define ESP8266_WIFI_SSID					"ChinaNet-PaRf"
#define ESP8266_WIFI_PASSWORD				"gj5qmdxe"

#define ESP8266_PING_DomainName				"http://nestbase.meseee.com"
#define ESP8266_PING_IP						""


uint8_t esp8266_wait_receive(void);
void esp8266_clear(void);
uint8_t esp8266_send_command(char *cmd, char *res);
void USART2_IRQHandler(void);

uint8_t esp8266_at_test(void);
uint8_t esp8266_at_RST(void);
uint8_t esp8266_at_CWMODE(void);
uint8_t esp8266_at_CWJAP(char *ssid, char *pwd);
uint8_t esp8266_at_PING(void);


void ESP8266_Init(void);
#endif
