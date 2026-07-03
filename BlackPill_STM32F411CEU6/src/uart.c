#include "uart.h"
#include "handlers.h"
#include "gpio.h"
#include "string.h"

#define UART_BUFFER_SIZE (uint16_t)(16)

USART_HandleTypeDef UartTxInit()
{
    __HAL_RCC_USART1_CLK_ENABLE();

    USART_InitTypeDef uartTxInitStruct = {0};
    uartTxInitStruct.BaudRate = 115200;
    uartTxInitStruct.WordLength = USART_WORDLENGTH_8B;
    uartTxInitStruct.StopBits = USART_STOPBITS_1;
    uartTxInitStruct.Parity = USART_PARITY_NONE;
    uartTxInitStruct.Mode = USART_MODE_TX;

    USART_HandleTypeDef uartTxHandle = {0};
    uartTxHandle.Init = uartTxInitStruct;
    uartTxHandle.Instance = USART1;

    if (HAL_USART_Init(&uartTxHandle) != HAL_OK)
        Error_Handler();

    return uartTxHandle;
}

USART_HandleTypeDef UartInit()
{
    __HAL_RCC_USART1_CLK_ENABLE();

    USART_InitTypeDef uartInitStruct = {0};
    uartInitStruct.BaudRate = 115200;
    uartInitStruct.WordLength = USART_WORDLENGTH_8B;
    uartInitStruct.StopBits = USART_STOPBITS_1;
    uartInitStruct.Parity = USART_PARITY_NONE;
    uartInitStruct.Mode = USART_MODE_TX_RX;

    USART_HandleTypeDef uartHandle = {0};
    uartHandle.Init = uartInitStruct;
    uartHandle.Instance = USART1;

    if (HAL_USART_Init(&uartHandle) != HAL_OK)
        Error_Handler();

    return uartHandle;
}

HAL_StatusTypeDef UartSendMessage(USART_HandleTypeDef uartHandle, const char* message)
{
    uint16_t messageLength = strlen(message);

    const uint8_t uartTxBuffer[messageLength];
    uartHandle.pTxBuffPtr = uartTxBuffer;

    memset((void*)uartHandle.pTxBuffPtr, 0, messageLength);

    memcpy((void*)uartHandle.pTxBuffPtr, (const void*)message, messageLength);

    HAL_StatusTypeDef statusTransmit = HAL_USART_Transmit(&uartHandle, uartHandle.pTxBuffPtr, messageLength, 100);

    return UartHandleResult(statusTransmit);
}

HAL_StatusTypeDef UartRecieveMessage(USART_HandleTypeDef uartHandle)
{
    uint8_t uartRxBuffer[UART_BUFFER_SIZE];
    uartHandle.pRxBuffPtr = uartRxBuffer;

    memset((void*)uartHandle.pRxBuffPtr, 0, UART_BUFFER_SIZE);

    HAL_StatusTypeDef statusRecieve = HAL_USART_Receive(&uartHandle, uartHandle.pRxBuffPtr, UART_BUFFER_SIZE, 10000);

    return UartHandleResult(statusRecieve);
}

HAL_StatusTypeDef UartHandleResult(HAL_StatusTypeDef status)
{
    switch (status)
    {
        case HAL_OK:
            ToggleLedPC13();
            return HAL_OK;
        case HAL_BUSY:
            ToggleLedPC13();
            HAL_Delay(100);
            return HAL_BUSY;
        case HAL_TIMEOUT:
            ToggleLedPC13();
            HAL_Delay(250);
            return HAL_TIMEOUT;
        case HAL_ERROR:
            Error_Handler();
        default:
            Error_Handler();
    }

    return HAL_ERROR;
}
