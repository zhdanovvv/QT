#ifndef UART_H
#define UART_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_usart.h"

#ifdef __cplusplus
    extern "C"
    {
#endif

USART_HandleTypeDef UartInit();
HAL_StatusTypeDef UartSendMessage(USART_HandleTypeDef uartHandle, const char* message);
HAL_StatusTypeDef UartRecieveMessage(USART_HandleTypeDef uartHandle);
HAL_StatusTypeDef UartHandleResult(HAL_StatusTypeDef status);

#ifdef __cplusplus
    }
#endif

#endif // UART_H
