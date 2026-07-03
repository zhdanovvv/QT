#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
    extern "C"
    {
#endif

void GpioInitLedPC13();
void GpioInitPB6PB7();
void ToggleLedPC13();

#ifdef __cplusplus
    }
#endif

#endif // GPIO_H
