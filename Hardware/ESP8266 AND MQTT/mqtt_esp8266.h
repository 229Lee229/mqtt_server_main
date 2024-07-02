#ifndef __MQTT_ESP8266_H
#define __MQTT_ESP8266_H



#define MQTTUSERCFG_MyConf 		"0,1,\"Michael_esp\",\"esp8266_li\",\"1234567\",0,0,\"\""
#define MQTTCONNCFG_MyConf  	"0,60,1,\"\",\"\",0,0"
#define MQTTCONN_Host_MyConf 	"192.168.1.42"
#define MQTTCONN_Port_MyConf	1883

void MQTT_Init(void);


// MQTT Functions
uint8_t esp8266_at_MQTTUSERCFG(char *config);
uint8_t esp8266_at_MQTTLONGCLIENTID(char *client_id);
uint8_t esp8266_at_MQTTLONGUSERNAME(char *username);
uint8_t esp8266_at_MQTTLONGPASSWORD(char *password);
uint8_t esp8266_at_MQTTCONNCFG(char *config);
uint8_t esp8266_at_MQTTCONN(char *host, int port);
uint8_t esp8266_at_MQTTPUB(char *topic, char *message);
uint8_t esp8266_at_MQTTPUBRAW(char *topic, uint8_t *data, uint16_t len);
uint8_t esp8266_at_MQTTSUB(char *topic);
uint8_t esp8266_at_MQTTUNSUB(char *topic);
uint8_t esp8266_at_MQTTCLEAN(void);

#endif
