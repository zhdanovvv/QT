#include "stm32f4xx_hal.h"
#include "handlers.h"
#include "gpio.h"

void SysTick_Handler()
{
    HAL_IncTick();
}

void Error_Handler()
{
    while(1)
    {
        ToggleLedPC13();
        HAL_Delay(50);
    }
}
