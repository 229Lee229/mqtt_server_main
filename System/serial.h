#ifndef __SERIAL_H
#define __SERIAL_H


void Serial_SendByte(uint8_t Byte);
void Serial_SendByte_usart1(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendString_usart1(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);

int fputc(int ch, FILE *f);

void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);

uint8_t Serial_GetRxData(void);

#endif
