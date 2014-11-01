//*****************************************************************************
//
// startup_gcc.c - Startup code for use with GNU tools.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"

//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);
extern void irq_handler_ada(void);
extern void pendsv_handler(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************
static unsigned long pulStack[128];

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long)pulStack + sizeof(pulStack)),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    irq_handler_ada,                       // The NMI handler
    irq_handler_ada,                      // The hard fault handler
    irq_handler_ada,                      // The MPU fault handler
    irq_handler_ada,                      // The bus fault handler
    irq_handler_ada,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    irq_handler_ada,                      // SVCall handler
    irq_handler_ada,                      // Debug monitor handler
    0,                                      // Reserved
    pendsv_handler,                       // The PendSV handler
    irq_handler_ada,                      // The SysTick handler
    irq_handler_ada,                      // GPIO Port A
    irq_handler_ada,                      // GPIO Port B
    irq_handler_ada,                      // GPIO Port C
    irq_handler_ada,                      // GPIO Port D
    irq_handler_ada,                      // GPIO Port E
    irq_handler_ada,                      // UART0 Rx and Tx
    irq_handler_ada,                      // UART1 Rx and Tx
    irq_handler_ada,                      // SSI0 Rx and Tx
    irq_handler_ada,                      // I2C0 Master and Slave
    irq_handler_ada,                      // PWM Fault
    irq_handler_ada,                      // PWM Generator 0
    irq_handler_ada,                      // PWM Generator 1
    irq_handler_ada,                      // PWM Generator 2
    irq_handler_ada,                      // Quadrature Encoder 0
    irq_handler_ada,                      // ADC Sequence 0
    irq_handler_ada,                      // ADC Sequence 1
    irq_handler_ada,                      // ADC Sequence 2
    irq_handler_ada,                      // ADC Sequence 3
    irq_handler_ada,                      // Watchdog timer
    irq_handler_ada,                      // Timer 0 subtimer A
    irq_handler_ada,                      // Timer 0 subtimer B
    irq_handler_ada,                      // Timer 1 subtimer A
    irq_handler_ada,                      // Timer 1 subtimer B
    irq_handler_ada,                      // Timer 2 subtimer A
    irq_handler_ada,                      // Timer 2 subtimer B
    irq_handler_ada,                      // Analog Comparator 0
    irq_handler_ada,                      // Analog Comparator 1
    irq_handler_ada,                      // Analog Comparator 2
    irq_handler_ada,                      // System Control (PLL, OSC, BO)
    irq_handler_ada,                      // FLASH Control
    irq_handler_ada,                      // GPIO Port F
    irq_handler_ada,                      // GPIO Port G
    irq_handler_ada,                      // GPIO Port H
    irq_handler_ada,                      // UART2 Rx and Tx
    irq_handler_ada,                      // SSI1 Rx and Tx
    irq_handler_ada,                      // Timer 3 subtimer A
    irq_handler_ada,                      // Timer 3 subtimer B
    irq_handler_ada,                      // I2C1 Master and Slave
    irq_handler_ada,                      // Quadrature Encoder 1
    irq_handler_ada,                      // CAN0
    irq_handler_ada,                      // CAN1
    irq_handler_ada,                      // CAN2
    irq_handler_ada,                      // Ethernet
    irq_handler_ada,                      // Hibernate
    irq_handler_ada,                      // USB0
    irq_handler_ada,                      // PWM Generator 3
    irq_handler_ada,                      // uDMA Software Transfer
    irq_handler_ada,                      // uDMA Error
    irq_handler_ada,                      // ADC1 Sequence 0
    irq_handler_ada,                      // ADC1 Sequence 1
    irq_handler_ada,                      // ADC1 Sequence 2
    irq_handler_ada,                      // ADC1 Sequence 3
    irq_handler_ada,                      // I2S0
    irq_handler_ada,                      // External Bus Interface 0
    irq_handler_ada,                      // GPIO Port J
    irq_handler_ada,                      // GPIO Port K
    irq_handler_ada,                      // GPIO Port L
    irq_handler_ada,                      // SSI2 Rx and Tx
    irq_handler_ada,                      // SSI3 Rx and Tx
    irq_handler_ada,                      // UART3 Rx and Tx
    irq_handler_ada,                      // UART4 Rx and Tx
    irq_handler_ada,                      // UART5 Rx and Tx
    irq_handler_ada,                      // UART6 Rx and Tx
    irq_handler_ada,                      // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    irq_handler_ada,                      // I2C2 Master and Slave
    irq_handler_ada,                      // I2C3 Master and Slave
    irq_handler_ada,                      // Timer 4 subtimer A
    irq_handler_ada,                      // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    irq_handler_ada,                      // Timer 5 subtimer A
    irq_handler_ada,                      // Timer 5 subtimer B
    irq_handler_ada,                      // Wide Timer 0 subtimer A
    irq_handler_ada,                      // Wide Timer 0 subtimer B
    irq_handler_ada,                      // Wide Timer 1 subtimer A
    irq_handler_ada,                      // Wide Timer 1 subtimer B
    irq_handler_ada,                      // Wide Timer 2 subtimer A
    irq_handler_ada,                      // Wide Timer 2 subtimer B
    irq_handler_ada,                      // Wide Timer 3 subtimer A
    irq_handler_ada,                      // Wide Timer 3 subtimer B
    irq_handler_ada,                      // Wide Timer 4 subtimer A
    irq_handler_ada,                      // Wide Timer 4 subtimer B
    irq_handler_ada,                      // Wide Timer 5 subtimer A
    irq_handler_ada,                      // Wide Timer 5 subtimer B
    irq_handler_ada,                      // FPU
    irq_handler_ada,                      // PECI 0
    irq_handler_ada,                      // LPC 0
    irq_handler_ada,                      // I2C4 Master and Slave
    irq_handler_ada,                      // I2C5 Master and Slave
    irq_handler_ada,                      // GPIO Port M
    irq_handler_ada,                      // GPIO Port N
    irq_handler_ada,                      // Quadrature Encoder 2
    irq_handler_ada,                      // Fan 0
    0,                                      // Reserved
    irq_handler_ada,                      // GPIO Port P (Summary or P0)
    irq_handler_ada,                      // GPIO Port P1
    irq_handler_ada,                      // GPIO Port P2
    irq_handler_ada,                      // GPIO Port P3
    irq_handler_ada,                      // GPIO Port P4
    irq_handler_ada,                      // GPIO Port P5
    irq_handler_ada,                      // GPIO Port P6
    irq_handler_ada,                      // GPIO Port P7
    irq_handler_ada,                      // GPIO Port Q (Summary or Q0)
    irq_handler_ada,                      // GPIO Port Q1
    irq_handler_ada,                      // GPIO Port Q2
    irq_handler_ada,                      // GPIO Port Q3
    irq_handler_ada,                      // GPIO Port Q4
    irq_handler_ada,                      // GPIO Port Q5
    irq_handler_ada,                      // GPIO Port Q6
    irq_handler_ada,                      // GPIO Port Q7
    irq_handler_ada,                      // GPIO Port R
    irq_handler_ada,                      // GPIO Port S
    irq_handler_ada,                      // PWM 1 Generator 0
    irq_handler_ada,                      // PWM 1 Generator 1
    irq_handler_ada,                      // PWM 1 Generator 2
    irq_handler_ada,                      // PWM 1 Generator 3
    irq_handler_ada                       // PWM 1 Fault
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
//extern unsigned long _etext;
extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void ResetISR(void)
{
    /* Copy the data segment initializers from flash to SRAM */  
    __asm("  movs  r1, #0\n"
          "  b  LoopCopyDataInit\n"
          "CopyDataInit:\n"
          "  ldr  r3, =_sidata\n"
          "  ldr  r3, [r3, r1]\n"
          "  str  r3, [r0, r1]\n"
          "  adds  r1, r1, #4\n"
          "LoopCopyDataInit:\n"
          "  ldr  r0, =_sdata\n"
          "  ldr  r3, =_edata\n"
          "  adds  r2, r0, r1\n"
          "  cmp  r2, r3\n"
          "  bcc  CopyDataInit\n"
          "  ldr  r2, =_sbss\n"
          "  b  LoopFillZerobss");

    /* Zero fill the bss segment. */              
    __asm(  "FillZerobss:\n"
            "  movs  r3, #0\n"
            "  str  r3, [r2], #4\n"
            "LoopFillZerobss:\n"
            "  ldr  r3, = _ebss\n"
            "  cmp  r2, r3\n"
            "  bcc  FillZerobss");
  
    /*
   unsigned long *pulSrc, *pulDest;
    pulSrc = &_sidata;
    for(pulDest = &_sdata; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    __asm("    ldr     r0, =_sbss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

    */
    //
    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).  Any configuration of the floating-point unit using DriverLib
    // APIs must be done here prior to the floating-point unit being enabled.
    //
    // Note that this does not use DriverLib since it might not be included in
    // this project.
    //
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) &
                         ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) |
                        NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

    //
    // Call the application's entry point.
    //
    main();
}

