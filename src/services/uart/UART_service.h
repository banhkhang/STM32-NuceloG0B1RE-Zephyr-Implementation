/* UART_service.h - UART service interface */
#ifndef UART_SERVICE_H
#define UART_SERVICE_H

void UART_ServiceInit(void);

void UART_ServiceSend(char *msg);
void UART_ServiceSend_IT(char *msg);
void UART_ServiceSend_DMA(char *msg);

#endif