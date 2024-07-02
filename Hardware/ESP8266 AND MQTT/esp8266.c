#include "stm32f10x.h"                  // Device header
#include "My_include.h"
uint8_t g_uart_rx_buf[ESP8266_UART_RX_BUF_SIZE];
uint8_t g_uart_tx_buf[ESP8266_UART_TX_BUF_SIZE];
uint16_t esp8266_cnt = 0, esp8266_cntPre = 0;

/* ------------------------------ 发送指令接口 ------------------------------- */
uint8_t esp8266_at_test(void)
{
    return esp8266_send_command("AT\r\n", "OK");
}
uint8_t esp8266_at_RST(void){
	return esp8266_send_command("AT+RST\r\n","OK");
}
uint8_t esp8266_at_CWMODE(void){
	return esp8266_send_command("AT+CWMODE=1\r\n","OK");
}
uint8_t esp8266_at_PING(void){
	return esp8266_send_command("AT+PING=\"baidu.com\"\r\n","OK");
//	return esp8266_send_command("AT+PING=\"http://nestbase.meseee.com\"\r\n","OK");
}
uint8_t esp8266_at_CWJAP(char *ssid, char *pwd)
{
    char cmd[64];
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);
    // USART1_SendString(cmd);	// test send String
    // return esp8266_send_command(cmd, "WIFI GOT IP");
	return esp8266_send_command(cmd, "WIFI CONNECTED");				/* 先接收此信息 */
}



/* ------------------------------- 驱动函数 -------------------------*/
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

void esp8266_clear(void)
{
	memset(g_uart_rx_buf, 0, sizeof(g_uart_rx_buf));
	esp8266_cnt = 0;
}
uint8_t esp8266_send_command(char *cmd, char *res)
{
	
	uint8_t timeOut = 250;

    esp8266_clear();
//	HAL_UART_Transmit(&g_uart_handle, (unsigned char *)cmd, strlen((const char *)cmd), 100);
	USART2_SendString(cmd);
	while(timeOut--) {
		if(esp8266_wait_receive() == ESP8266_EOK) {						//如果收到数据
			if(strstr((const char *)g_uart_rx_buf, res) != NULL)		//如果检索到关键词
				return ESP8266_EOK;
		}
		Delay_ms_JKD(10);
	}
	return ESP8266_ERROR;

}



void USART2_IRQHandler(void)
{
    uint8_t receive_data = 0;   
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		if(esp8266_cnt >= sizeof(g_uart_rx_buf))
            esp8266_cnt = 0; //防止串口被刷爆
        // HAL_UART_Receive(&g_uart_handle, &receive_data, 1, 1000);//串口2接收1位数据
		receive_data = USART_ReceiveData(USART2);
		g_uart_rx_buf[esp8266_cnt++] = receive_data;  
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
    // HAL_UART_IRQHandler(&g_uart_handle);
}





void ESP8266_Init(void){
	// esp8266 Init 
	if(esp8266_at_test() == 1)
		printf("at test Error!\r\n");
	else printf("at test Successful!\r\n");
	// USART1_SendString((char *)g_uart_rx_buf);   				// 检测回传指令是否正确

	if(esp8266_at_CWMODE() == 1)
		printf("at CWMODE Error!\r\n");
	else printf("at CWMODE Successful!\r\n");
	// USART1_SendString((char *)g_uart_rx_buf);				// 检测回传指令是否正确
	
	if(esp8266_at_CWJAP(ESP8266_WIFI_SSID, ESP8266_WIFI_PASSWORD) == 1)
		 printf("at CWJAP Error!\r\n");
	else printf("at CWJAP Successful!\r\n");
	// USART1_SendString((char *)g_uart_rx_buf);				// 检测回传指令是否正确
	
	Delay_ms_JKD(2);
	// Ping test
	u8 timeout_ping = 10;
	while(timeout_ping--){
		if(esp8266_at_PING() == 1)
			printf("at PING Error!\r\n");
		else{
			printf("at PING Successful!\r\n");
			break;
		}
	// USART1_SendString((char *)g_uart_rx_buf);				// 检测回传指令是否正确
		Delay_ms_JKD(50);
	}


	
	
}


