#include "My_include.h"


void MQTT_Init(void){
	// mqtt Init
	// 1 - AT+MQTTUSERCFG：设置 MQTT 用户属性
	if(esp8266_at_MQTTUSERCFG(MQTTUSERCFG_MyConf) == 1)
		printf("MQTTUSERCFG Error!\r\n");
	else printf("MQTTUSERCFG Successful!\r\n");
	Delay_ms_JKD(1);
	
	// 2 - AT+MQTTCONNCFG：设置 MQTT 连接属性
	if(esp8266_at_MQTTCONNCFG(MQTTCONNCFG_MyConf) == 1)
		printf("MQTTCONNCFG Error!\r\n");
	else printf("MQTTCONNCFG Successful!\r\n");
	Delay_ms_JKD(1);

	// 3 - AT+MQTTCONN：连接 MQTT Broker
	if(esp8266_at_MQTTCONN(MQTTCONN_Host_MyConf,MQTTCONN_Port_MyConf) == 1)
		printf("MQTTCONN Error!\r\n");
	else printf("MQTTCONN Successful!\r\n");
	Delay_ms_JKD(1);

	
}

// MQTT Functions Implementation
uint8_t esp8266_at_MQTTUSERCFG(char *config)
{
    char cmd[64];
	// AT+MQTTUSERCFG=<LinkID>,<scheme>,<"client_id">,<"username">,<"password">,<cert_key_ID>,<CA_ID>,<"path">
    sprintf(cmd, "AT+MQTTUSERCFG=%s\r\n", config);		
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTLONGCLIENTID(char *client_id)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTLONGCLIENTID=%s\r\n", client_id);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTLONGUSERNAME(char *username)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTLONGUSERNAME=%s\r\n", username);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTLONGPASSWORD(char *password)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTLONGPASSWORD=%s\r\n", password);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTCONNCFG(char *config)
{
    char cmd[64];
	// AT+MQTTCONNCFG=<LinkID>,<keepalive>,<disable_clean_session>,<"lwt_topic">,<"lwt_msg">,<lwt_qos>,<lwt_retain>
    sprintf(cmd, "AT+MQTTCONNCFG=%s\r\n", config);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTCONN(char *host, int port)
{
    char cmd[128];
	// AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
    sprintf(cmd, "AT+MQTTCONN=\"%s\",%d\r\n", host, port);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTPUB(char *topic, char *message)
{
    char cmd[256];
    sprintf(cmd, "AT+MQTTPUB=\"%s\",\"%s\"\r\n", topic, message);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTPUBRAW(char *topic, uint8_t *data, uint16_t len)
{
    char cmd[256];
    sprintf(cmd, "AT+MQTTPUBRAW=\"%s\",", topic);
    for (int i = 0; i < len; i++) {
        char byte[4];
        sprintf(byte, "%02X", data[i]);
        strcat(cmd, byte);
    }
    strcat(cmd, "\r\n");
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTSUB(char *topic)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTSUB=\"%s\"\r\n", topic);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTUNSUB(char *topic)
{
    char cmd[128];
    sprintf(cmd, "AT+MQTTUNSUB=\"%s\"\r\n", topic);
    return esp8266_send_command(cmd, "OK");
}

uint8_t esp8266_at_MQTTCLEAN(void)
{
    return esp8266_send_command("AT+MQTTCLEAN\r\n", "OK");
}



