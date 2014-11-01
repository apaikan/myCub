/* ---------------------------------------------------------------------------
--                                                                          --
--                           GNAT RAVENSCAR for NXT                         --
--                                                                          --
--                       Copyright (C) 2010, AdaCore                        --
--                                                                          --
-- This is free software; you can  redistribute it  and/or modify it under  --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 2,  or (at your option) any later ver- --
-- sion. This is distributed in the hope that it will be useful, but WITH-  --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNARL; see file COPYING.  If not, write --
-- to  the Free Software Foundation,  59 Temple Place - Suite 330,  Boston, --
-- MA 02111-1307, USA.                                                      --
--                                                                          --
-- As a special exception,  if other files  instantiate  generics from this --
-- unit, or you link  this unit with other files  to produce an executable, --
-- this  unit  does not  by itself cause  the resulting  executable  to  be --
-- covered  by the  GNU  General  Public  License.  This exception does not --
-- however invalidate  any other reasons why  the executable file  might be --
-- covered by the  GNU Public License.                                      --
--                                                                          --
--------------------------------------------------------------------------- */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_nvic.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"

#define MAX_INTERRUPT_PRIORITY      32     //we use only 32 level of interrupt priorities
#define portNVIC_SYSPRI2			( ( volatile unsigned long * ) 0xe000ed20 )
#define portNVIC_PENDSV_PRI			( ( ( unsigned long ) MAX_INTERRUPT_PRIORITY-1 ) << 16 )
#define portNVIC_SYSTICK_PRI		( ( ( unsigned long ) MAX_INTERRUPT_PRIORITY-1 ) << 24 )

typedef unsigned long IRQn_Type;

typedef union
{
  struct
  {
    uint32_t IPSR:9;                      //!< bit:  0.. 8  Exception number
    uint32_t _reserved0:23;              //!< bit:  9..31  Reserved
  } b;                                   //!< Structure used for bit  access
  uint32_t w;                            //!< Type      used for word access
} IPSR_Type;

static inline uint32_t __get_IPSR(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, ipsr" : "=r" (result) );
  return(result);
}


void init_board() 
{
    // Enable lazy stacking for interrupt handlers.
    ROM_FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    // sets the system clock divider to 2.5 (200 MHz PLL divide by 2.5 = 80 MHz)
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    //_NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    ROM_IntPriorityGroupingSet(NVIC_APINT_PRIGROUP_4_4);


    // Enabling all the peripheral ports
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);

    // The Timer4 peripheral must be enabled for use with RTC    
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);

   
    /*
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
    */

    //
    // Enable ADC
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    //ROM_GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3); // A0
    ROM_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);   // A5
    ROM_GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);   // A6

    // 
    // Enable I2C1
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    ROM_GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    ROM_GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    ROM_GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

}

void setup_systick(int frequency) 
{      
	/* Make PendSV and SysTick the lowest priority interrupts. */
	*(portNVIC_SYSPRI2) |= portNVIC_PENDSV_PRI;
	*(portNVIC_SYSPRI2) |= portNVIC_SYSTICK_PRI;    

    ROM_IntPriorityGet(14); // pendsv
    ROM_IntPriorityGet(15); // systick

    ROM_SysTickPeriodSet(ROM_SysCtlClockGet()/frequency-1);
    // Enable interrupts to the processor.
    ROM_IntMasterEnable();
    // Enable the SysTick Interrupt.
    ROM_SysTickIntEnable();
    // Enable SysTick.
    ROM_SysTickEnable();
}

int get_current_interrupt() 
{
  IPSR_Type ipsr;
  ipsr.w = __get_IPSR();
  return ipsr.b.IPSR;
}

void context_switch() 
{
  *((uint32_t volatile *)0xE000ED04) = 0x10000000; // trigger PendSV
}

uint32_t _NVIC_GetPriority(IRQn_Type IRQn) 
{
    return MAX_INTERRUPT_PRIORITY - ROM_IntPriorityGet(IRQn);
}

void data_abort_pc (void)
{
}

void data_abort_C (void)
{
}
/*
void __aeabi_unwind_cpp_pr0 (void)
{
  while (1) ;
}*/

extern void put_exception (unsigned int) __attribute__ ((weak));

void __attribute__ ((weak)) __gnat_last_chance_handler (void)
{
  unsigned int addr = (int) __builtin_return_address (0);

  if (put_exception != NULL)
    put_exception (addr);

  while (1)
    ;
}

// Adapted from "arti64.c"

long long int __gnat_mulv64 (long long int x, long long int y)
{
  unsigned neg = (x >= 0) ^ (y >= 0);
  long long unsigned xa = x >= 0 ? (long long unsigned) x
                                 : -(long long unsigned) x;
  long long unsigned ya = y >= 0 ? (long long unsigned) y
                                 : -(long long unsigned) y;
  unsigned xhi = (unsigned) (xa >> 32);
  unsigned yhi = (unsigned) (ya >> 32);
  unsigned xlo = (unsigned) xa;
  unsigned ylo = (unsigned) ya;
  long long unsigned mid
    = xhi ? (long long unsigned) xhi * (long long unsigned) ylo
         : (long long unsigned) yhi * (long long unsigned) xlo;
  long long unsigned low = (long long unsigned) xlo * (long long unsigned) ylo;

  if ((xhi && yhi) ||  mid + (low  >> 32) > 0x7fffffff + neg)
    __gnat_last_chance_handler();

  low += ((long long unsigned) (unsigned) mid) << 32;

  return (long long int) (neg ? -low : low);
}


/*
static inline uint32_t _RCC_GetClocksFreq()
{
  __RCC_GetClocksFreq(&rcc_clk);
  return rcc_clk.HCLK_Frequency;
}


#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)
static inline void _NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
  // Check the parameters 
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  // Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}
*/
