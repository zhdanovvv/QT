Команды для сборки и загрузки прошивки на stm32, модель: Black Pill STM32F411CEU6 Mini Core Board (with 25M HSE).
ОС Ubuntu 22.04

Микроконтроллер: https://www.chipdip.ru/product/black-pill-stm32f411ceu6-mini-core-board-with-weact-studio-9001922977
Программатор/отладчик: https://www.chipdip.ru/product/st-link-v2-mini-multicolor-vnutrishemnyy-9000563621
Преобразователь USB-UART: https://www.chipdip.ru/product/ft232-usb-uart-board-micro-preobrazovatel-waveshare-9000419827?from=suggest_product


Распакуйте архив gcc-arm-none-eabi-10.3-2021.10.tar.xz в директорию /opt

После выполните команды ниже.

Сборка прошивки:

cd build/
mkdir debug && cd debug

Ninja:
cmake ../../ --toolchain=toolchain.cmake  --debug-output -G "Ninja" && ninja

Make:
cmake ../../ --toolchain=toolchain.cmake  --debug-output -G "Unix Makefiles" && make

Загрузка прошивки:

Подключите программатор/отладчик:

apt install stlink-tools
st-flash write BlackPill.bin 0x08000000

Команды для открытия консольного порта.

Установка и запуск:

apt install picocom
picocom /dev/ttyUSB0 -b 115200

Уточнение по архиву gcc-arm-none-eabi-10.3-2021.10.tar.xz:
arm-none-eabi-gdb из архива для подключения в qtcreator - нерабочий, поэтому лучше использовать gdb-multiarch вместе с компиляторами из тулчейна
Команда установки:
apt install gdb-multiarch
