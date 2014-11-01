
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <inc/hw_ints.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/rom.h>
#include "driverlib/uart.h"


void uart_setup(void)
{
    //
    // Enable the peripherals used by this example.
    // The UART itself needs to be enabled, as well as the GPIO port
    // containing the pins that will be used.
    
    //ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);

    //
    // Configure the GPIO pin muxing for the UART function.
    // This is only necessary if your part supports GPIO pin function muxing.
    // Study the data sheet to see which functions are allocated per pin.
    
    //ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    //ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
    ROM_GPIOPinConfigure(GPIO_PC7_U3TX);
    //
    // Since GPIO A0 and A1 are used for the UART function, they must be
    // configured for use as a peripheral function (instead of GPIO).
    //
    //ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    // This function uses SysCtlClockGet() to get the system clock
    // frequency.  This could be also be a variable or hard coded value
    // instead of a function call.
    
    //ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
    //                    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
    //                     UART_CONFIG_PAR_NONE));
    ROM_UARTConfigSetExpClk(UART3_BASE, ROM_SysCtlClockGet(), 115200,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));
    //
    // Enable the UART interrupt.
    //
    //ROM_IntPriorityGet(INT_UART0); 
    //ROM_IntPrioritySet(INT_UART0, 15);
    //ROM_IntPriorityGet(INT_UART0); 
    //ROM_UARTIntDisable(UART0_BASE, 0xFFFFFFFF);
    //ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    //ROM_IntEnable(INT_UART0);

    ROM_IntPriorityGet(INT_UART3); 
    ROM_IntPrioritySet(INT_UART3, 15);
    ROM_IntPriorityGet(INT_UART3); 
    ROM_UARTIntDisable(UART3_BASE, 0xFFFFFFFF);
    ROM_UARTIntEnable(UART3_BASE, UART_INT_RX | UART_INT_RT);
    ROM_IntEnable(INT_UART3);
}

int16_t combine(uint8_t lo, uint8_t hi)
{
    return (int16_t)(lo | ((int16_t)hi << 8));
}

