#ifdef DEBUG_esp8266_V5
#include "esp8266_01sV5.h"
#include "string.h"
#include "Delay_JKD.h"
#include <stdarg.h>
#include "My_include.h"
 // USART_InitTypeDef USART_InitStructure;
uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];
uint8_t g_uart_tx_buf[ESP8266_UART_TX_BUF_SIZE];
uint16_t esp8266_cnt = 0, esp8266_cntPre = 0;

void UART_Transmit(USART_TypeDef* USARTx, unsigned char* pData, uint16_t Size, uint32_t Timeout)
{
    uint32_t start_tick = SysTick->VAL;

    for (uint16_t i = 0; i < Size; i++) {
        USART_SendData(USARTx, pData[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET) {
            // 检查超时
            if ((SysTick->VAL - start_tick) >= Timeout) {
                return;  // 超时返回
            }
        }
    }
}


/**
 * @brief       ESP8266 UART初始化
 * @param       baudrate: UART波特率
 * @retval      无
 */
//void esp8266_uart_init(uint32_t baudrate)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    // NVIC_InitTypeDef NVIC_InitStructure;

//    // 启用时钟
//    ESP8266_UART_CLK_ENABLE();
//    ESP8266_UART_TX_GPIO_CLK_ENABLE();
//    ESP8266_UART_RX_GPIO_CLK_ENABLE();

//    // 配置USART TX引脚
//    GPIO_InitStructure.GPIO_Pin = ESP8266_UART_TX_GPIO_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(ESP8266_UART_TX_GPIO_PORT, &GPIO_InitStructure);

//    // 配置USART RX引脚
//    GPIO_InitStructure.GPIO_Pin = ESP8266_UART_RX_GPIO_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    GPIO_Init(ESP8266_UART_RX_GPIO_PORT, &GPIO_InitStructure);

//    // 配置USART
//    USART_InitStructure.USART_BaudRate = baudrate;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    USART_Init(ESP8266_UART_INTERFACE, &USART_InitStructure);
//    USART_Cmd(ESP8266_UART_INTERFACE, ENABLE);

//    // 配置USART中断
//    USART_ITConfig(ESP8266_UART_INTERFACE, USART_IT_RXNE, ENABLE);

//    // 配置NVIC
//    NVIC_InitStructure.NVIC_IRQChannel = ESP8266_UART_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}

/**
 * @brief       ESP8266 UART中断处理程序
 * @param       无
 * @retval      无
 */
//void ESP8266_UART_IRQHandler(void)
//{
//    if (USART_GetITStatus(ESP8266_UART_INTERFACE, USART_IT_RXNE) != RESET) {
//        char received_char = USART_ReceiveData(ESP8266_UART_INTERFACE);
//        if (esp8266_cnt >= sizeof(g_uart_rx_buf)) {
//            esp8266_cnt = 0; // 防止缓冲区溢出
//        }
//        g_uart_rx_buf[esp8266_cnt++] = received_char;
//    }
//}


/* 有超时时间 中断函数 */
//void USART2_IRQHandler(void) {
//    uint32_t timeout = 1000;  // 超时时间
//    uint32_t start_tick = SysTick->VAL;
//    uint8_t receive_data = 0;

//    while (timeout) {
//        if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
//            if (esp8266_cnt >= sizeof(g_uart_rx_buf))
//                esp8266_cnt = 0; // 防止缓冲区溢出
//            receive_data = USART_ReceiveData(USART2); // 串口2接收1位数据
//            g_uart_rx_buf[esp8266_cnt++] = receive_data;
//            break;
//        }

//        if ((SysTick->VAL - start_tick) >= timeout) {
//            break;
//        }
//    }

//    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//}


uint8_t esp8266_wait_receive(void)
{

	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return ESP8266_ERROR;
		
	if(esp8266_cnt == esp8266_cntPre) {				//如果上一次的值和这次相同，则说明接收完毕
		esp8266_cnt = 0;							//清0接收计数
		return ESP8266_EOK;							//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	return ESP8266_ERROR;							//返回接收未完成标志
}

/**
 * @brief       清空接收缓冲区
 * @param       无
 * @retval      无
 */
void esp8266_clear(void)
{
    memset(g_uart_rx_buf, 0, sizeof(g_uart_rx_buf));
    esp8266_cnt = 0;
}

uint8_t esp8266_send_command(char *cmd, char *res)
{
    uint8_t timeOut = 250;

    esp8266_clear();
//    for (uint16_t i = 0; i < strlen(cmd); i++) {
//        USART_SendData(USART_EPS8266_PORT, cmd[i]);
//        while (USART_GetFlagStatus(USART_EPS8266_PORT, USART_FLAG_TXE) == RESET);			// 等待不出去  6/18 debug
//    }
	UART_Transmit(USART2, (unsigned char *)cmd, strlen((const char *)cmd),1000);
//    while (timeOut--) {					// timeout 出不去？？？？
//        if (esp8266_wait_receive() == ESP8266_EOK) { // 如果收到数据
//            if (strstr((const char *)g_uart_rx_buf, res) != NULL) { // 如果检索到关键词
//                return ESP8266_EOK;
//            }
//        }
//        Delay_ms_JKD(10);
//    }
	
	while(timeOut--){
		
		Delay_ms_JKD(20);
	}
	
    return ESP8266_ERROR;
}
uint8_t esp8266_restore(void)
{
    return esp8266_send_command("AT+RESTORE", "ready");
}

uint8_t esp8266_at_test(void)
{
    return esp8266_send_command("AT\r\n", "OK");
}
uint8_t esp8266_set_mode(uint8_t mode)
{
    switch (mode) {
        case ESP8266_STA_MODE:
            return esp8266_send_command("AT+CWMODE=1\r\n", "OK");    /* Station模式 */
        
        case ESP8266_AP_MODE:
            return esp8266_send_command("AT+CWMODE=2\r\n", "OK");    /* AP模式 */
        
        case ESP8266_STA_AP_MODE:
            return esp8266_send_command("AT+CWMODE=3\r\n", "OK");    /* AP+Station模式 */
        
        default:
            return ESP8266_EINVAL;
    }
}
uint8_t esp8266_sw_reset(void)
{
    return esp8266_send_command("AT+RST\r\n", "OK");
}
uint8_t esp8266_join_ap(char *ssid, char *pwd)
{
    char cmd[64];
    
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
    
    // return esp8266_send_command(cmd, "WIFI GOT IP");
	return esp8266_send_command(cmd, "WIFI CONNECTED");				/* 先接收此信息 */
}
uint8_t esp8266_get_ip(char *buf)
{
    char *p_start;
    char *p_end;

    if (esp8266_send_command("AT+CIFSR\r\n", "STAIP") != ESP8266_EOK)
        return ESP8266_ERROR;

    p_start = strstr((const char *)g_uart_rx_buf, "\"");
    p_end = strstr(p_start + 1, "\"");
    *p_end = '\0';
    sprintf(buf, "%s", p_start + 1);
    
    return ESP8266_EOK;
}
uint8_t esp8266_connect_tcp_server(char *server_ip, char *server_port)
{
    char cmd[64];

    sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n", server_ip, server_port);

    return esp8266_send_command(cmd, "CONNECT");
}
uint8_t esp8266_disconnect_tcp_server(void)
{
    return esp8266_send_command("AT+CIPCLOSE\r\n", "");

}
uint8_t esp8266_enter_unvarnished(void)
{
    uint8_t ret;
    
    ret  = esp8266_send_command("AT+CIPMODE=1\r\n", "OK");
    ret += esp8266_send_command("AT+CIPSEND\r\n", ">");
    if (ret == ESP8266_EOK)
        return ESP8266_EOK;
    else
        return ESP8266_ERROR;
}
uint8_t esp8266_exit_unvarnished(void)
{
    return esp8266_send_command("+++", "");

}
uint8_t esp8266_single_connection(void)
{
    return esp8266_send_command("AT+CIPMUX=0\r\n", "OK");
}

uint8_t esp8266_CipSend(void)
{
	return esp8266_send_command("AT+CIPSEND=4\r\n","AT+CIPSEND=4\r\n");
	
}
uint8_t esp8266_Byte_Data_1(void)
{
	return esp8266_send_command("tesT\r\n","\r\n");
}
uint8_t esp8266_init(uint32_t baudrate)
{
    char ip_buf[16];

	Usart_ESP8266_Init(baudrate);					// esp8266
//    /* 让WIFI退出透传模式 */
//	while(esp8266_exit_unvarnished())
//        Delay_ms_JKD(500);

	printf("1.AT\r\n");
	while(esp8266_at_test())
        Delay_ms_JKD(500);
	
	printf("2.RST\r\n");
    while(esp8266_sw_reset())
        Delay_ms_JKD(500);
	while(esp8266_disconnect_tcp_server())
        Delay_ms_JKD(500);
	
	printf("3.CWMODE\r\n");
	while(esp8266_set_mode(ESP8266_STA_MODE))
        Delay_ms_JKD(500);
	
	printf("4.AT+CIPMUX\r\n");  //设置单路连接模式，透传只能使用此模式
	while(esp8266_single_connection())
        Delay_ms_JKD(500);
	
	printf("5.CWJAP\r\n");      //连接WIFI
	while(esp8266_join_ap(WIFI_SSID, WIFI_PWD))
        Delay_ms_JKD(1000);
    
    printf("6.CIFSR\r\n");
    while(esp8266_get_ip(ip_buf))
        Delay_ms_JKD(500);

    printf("ESP8266 IP: %s\r\n", ip_buf);
    
    printf("7.CIPSTART\r\n");
    while(esp8266_connect_tcp_server(TCP_SERVER_IP, TCP_SERVER_PORT))
        Delay_ms_JKD(500);
//    
//    printf("8.CIPMODE\r\n");
//    while(esp8266_enter_unvarnished())
//        delay_ms(500);
    
    printf("ESP8266_Init OK\r\n");
	
	
	// TCP客户端发送数据
	printf("AT+CIPSEND LOAD...\r\n");
	while(esp8266_CipSend())
		Delay_ms_JKD(500);
	
	// 发送字节数据 发送输入字节数据
    printf("start send byte stream...\r\n");
	while(esp8266_Byte_Data_1())
		Delay_ms_JKD(500);

	printf("successful!\r\n");
    return ESP8266_EOK;
}


//void esp8266_uart_printf(char *fmt, ...)
//{
//    va_list ap;
//    uint16_t len;
//    
//    va_start(ap, fmt);
//    vsprintf((char *)g_uart_tx_buf, fmt, ap);
//    va_end(ap);
//    
//    len = strlen((const char *)g_uart_tx_buf);
//    HAL_UART_Transmit(&g_uart_handle, g_uart_tx_buf, len, HAL_MAX_DELAY);
//}
#endif
