/*  This is a simple Microchip Studio project for Microchip SAME54 Xplained board (based on ATSAME54P20A MCU).
*   Here we use UART (SERCOM2 UART: PB24(RX) and PB25(TX)) for debug messaging to the PC through the virtual COM-port.
*   We use FreeRTOS periodic task to blink an on-board Yellow LED (PC18), change the parameter passed to the task and send a message through the UART.
*/

#include <atmel_start.h>
#include <string.h>
#include <stdio.h>

//#include "driver_examples.h"

#define MAX_DBG_MSG_LEN             128
#define STACK_SIZE                  100
#define PRINT_TASK_PERIOD_MS        1000

struct io_descriptor *dbg_io;
char debug_buff[MAX_DBG_MSG_LEN];
static uint32_t number = 100;


void uart_for_dbg_init(void)
{
    usart_sync_get_io_descriptor(&USART_0, &dbg_io);
    usart_sync_enable(&USART_0);
}


void debugMsg(struct io_descriptor* const io_descr, const char* msg, uint8_t len)
{
    io_write(io_descr, (uint8_t *)msg, len);
}


void periodicPrintTask(void * pvParameters)
{
    for(;;)
    {
        uint32_t *number_to_print = (uint32_t*)pvParameters;
        uint8_t len = snprintf(debug_buff, MAX_DBG_MSG_LEN, "Periodic message: %lu\r\n", *number_to_print);
        debugMsg(dbg_io, debug_buff, len);
        ++(*number_to_print);
        gpio_toggle_pin_level(LED_0);           // or replace LED_0 with GPIO(GPIO_PORTC, 18) to toggle PC18 pin (yellow LED) here
        vTaskDelay(pdMS_TO_TICKS(PRINT_TASK_PERIOD_MS));
    }
}


int main(void)
{
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();
    /* Initializes UART */
    uart_for_dbg_init();

    /* Test blink of a LED */
    gpio_toggle_pin_level(LED_0);
    delay_ms(1000);

    /* Test message before the RTOS starts */
    uint8_t len = snprintf(debug_buff, MAX_DBG_MSG_LEN, "First test message: %lu\r\n", number);
    debugMsg(dbg_io, debug_buff, len);
    delay_ms(1000);

    BaseType_t xReturned = pdPASS;
    TaskHandle_t xHandle = NULL;

    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(periodicPrintTask, "periodicPrintTask", STACK_SIZE, (void*)&number, tskIDLE_PRIORITY, &xHandle);

    if(pdPASS != xReturned)
    {
        /* The task was not created. Use the task's handle to delete the task. */
        vTaskDelete(xHandle);
        ASSERT(0);
    }

   vTaskStartScheduler(); // Don't start the scheduler to get into while-loop below

	/* Replace with your application code */
	while (1) {
        /* you don't get here while RTOS runs OK */
        uint8_t len = snprintf(debug_buff, MAX_DBG_MSG_LEN, "The message in while-loop: %lu\r\n", number);
        debugMsg(dbg_io, debug_buff, len);
        ++number;
        gpio_toggle_pin_level(LED_0);
        delay_ms(1000);
	}
}
