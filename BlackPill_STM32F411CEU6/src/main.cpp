#include "board_init.h"
#include "gpio.h"
#include "uart.h"

int main()
{    
    SystemClock_Config();

    GpioInitLedPC13();
    GpioInitPB6PB7();

    USART_HandleTypeDef uartHandle = UartInit();

    const char* message = "Hello USART\r\n";

    while (1)
    {
        HAL_Delay(1000);        

        if (UartSendMessage(uartHandle, message) != HAL_OK)
            continue;
    }
}
