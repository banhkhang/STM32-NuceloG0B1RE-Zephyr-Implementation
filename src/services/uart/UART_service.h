/* UART_service.h - UART service interface */
#ifndef UART_SERVICE_H
#define UART_SERVICE_H

void UART_ServiceInit(void);

void UART_SendString(char *msg);

#endif