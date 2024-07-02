#ifndef __MY_INCLUDE_H
#define __MY_INCLUDE_H

#include "stm32f10x.h"                  // Device header
#include "main.h"
#include "OLED.h"
#include "Delay_JKD.h"
#include "Matrix_Key.h"
#include "MyGPIO.h"
#include "usart.h"
#include "SysBitBand.h"

#include "stdbool.h"

#include "relay5V.h"				// 继电器3.3控制5
#include "switchCtrlRelay.h"		// 开关控制继电器

#include "timer_doorLock.h"			// 定时器控制门锁	
#include "string.h"
#include "stdio.h"



#include "esp8266.h"
#include "mqtt_esp8266.h"
// 5/30
//#include "esp8266testLED.h"

// #include "esp8266_01sV5.h"

// #include "serial.h"
// 6/16

//#ifdef DEBUG_esp8266_V1
//	#include "esp8266_01s.h"
//#endif

//#ifdef DEBUG_esp8266_V2
//	#include "esp8266_01sV2.h"
//#endif

//#ifdef DEBUG_esp8266_V3
//	#include "esp8266_01sV3.h"
//#endif

//#if 
//	#ifdef DEBUG_esp8266_V1
//		#include "esp8266_01s.h"
//	#endif
//#elseif
//	#ifdef DEBUG_esp8266_V2
//		#include "esp8266_01sV2.h"
//	#endif
//#else
//	#ifdef DEBUG_esp8266_V3
//		#include "esp8266_01sV3.h"
//	#endif


#endif
