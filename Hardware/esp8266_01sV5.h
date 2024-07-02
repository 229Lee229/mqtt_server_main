#ifndef __ESP8266_01SV5_H__
#define __ESP8266_01SV5_H__

#include <stdint.h>

// UART����
//#define ESP8266_UART_TX_GPIO_PORT           GPIOA
//#define ESP8266_UART_TX_GPIO_PIN            GPIO_Pin_2
//#define ESP8266_UART_TX_GPIO_CLK_ENABLE()   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

//#define ESP8266_UART_RX_GPIO_PORT           GPIOA
//#define ESP8266_UART_RX_GPIO_PIN            GPIO_Pin_3
//#define ESP8266_UART_RX_GPIO_CLK_ENABLE()   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

//#define ESP8266_UART_INTERFACE              USART2
#define ESP8266_UART_IRQn                     USART2_IRQn
//#define ESP8266_UART_IRQHandler             USART2_IRQHandler
//#define ESP8266_UART_CLK_ENABLE()           RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE)

// UART���պͷ��ͻ�������С
#define ESP8266_UART_RX_BUF_SIZE            128
#define ESP8266_UART_TX_BUF_SIZE            64

// �������
#define ESP8266_EOK                         0   // û�д���
#define ESP8266_ERROR                       1   // ͨ�ô���
#define ESP8266_ETIMEOUT                    2   // ��ʱ����
#define ESP8266_EINVAL                      3   // ��������

// ����ģʽ
#define ESP8266_STA_MODE                    1
#define ESP8266_AP_MODE                     2
#define ESP8266_STA_AP_MODE                 3

// WiFi����
#define WIFI_SSID                           "ChinaNet-PaRf"
#define WIFI_PWD                            "gj5qmdxe"

// TCP����������
#define TCP_SERVER_IP                       "192.168.1.26"
#define TCP_SERVER_PORT                     "1885"

// ��������
// void esp8266_uart_init(uint32_t baudrate);
void esp8266_clear(void);
void esp8266_uart_printf(char *fmt, ...);
uint8_t esp8266_at_test(void);
uint8_t esp8266_init(uint32_t baudrate);
#endif
