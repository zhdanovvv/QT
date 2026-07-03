#include "stm32f4xx_hal.h"
#include "handlers.h"

void GpioInitLedPC13()
{
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef gpioInit = {0};
   gpioInit.Pin = GPIO_PIN_13;
   gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
   gpioInit.Pull = GPIO_PULLUP;
   gpioInit.Speed = GPIO_SPEED_HIGH;
   HAL_GPIO_Init(GPIOC, &gpioInit);

   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

void ToggleLedPC13()
{    
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);    
}

void GpioInitPB6PB7()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpioInitPB6 = {0};
    gpioInitPB6.Pin = GPIO_PIN_6;
    gpioInitPB6.Mode = GPIO_MODE_AF_PP;
    gpioInitPB6.Pull = GPIO_PULLUP;
    gpioInitPB6.Speed = GPIO_SPEED_HIGH;
    gpioInitPB6.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &gpioInitPB6);

    GPIO_InitTypeDef gpioInitPB7 = {0};
    gpioInitPB7.Pin = GPIO_PIN_7;
    gpioInitPB7.Mode = GPIO_MODE_AF_PP;
    gpioInitPB7.Pull = GPIO_PULLUP;
    gpioInitPB7.Speed = GPIO_SPEED_HIGH;
    gpioInitPB6.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &gpioInitPB7);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
}
