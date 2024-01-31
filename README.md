# SAME54Xplained_UARTdbg_FreeRTOS

This is a simple Microchip Studio project for Microchip SAME54 Xplained board (based on ATSAME54P20A MCU).
Here we use UART (SERCOM2 UART: PB24(RX) and PB25(TX)) for debug messaging to the PC through the virtual COM-port.
We use FreeRTOS periodic task to blink an on-board Yellow LED (PC18), change the parameter passed to the task and send a message through the UART.