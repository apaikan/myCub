
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
#include <driverlib/timer.h>


#define SERVO_MAX_COUNT         4
#define SERVO_MIN_PULSE         544
#define SERVO_MAX_PULSE         2400
#define SERVO_PERIOD            20000

unsigned long g_servo_timers[SERVO_MAX_COUNT] = {
                                               TIMER2_BASE, 
                                               TIMER3_BASE, 
                                               TIMER1_BASE, 
                                               TIMER0_BASE };

unsigned long g_servo_peripherals[SERVO_MAX_COUNT] = {
                                               SYSCTL_PERIPH_TIMER2, 
                                               SYSCTL_PERIPH_TIMER3, 
                                               SYSCTL_PERIPH_TIMER1, 
                                               SYSCTL_PERIPH_TIMER0 };
bool servo_setpos(int id, int pos);

int servo_start(void) 
{
    ROM_GPIOPinConfigure(GPIO_PB0_T2CCP0);
    ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_0);
    
    ROM_GPIOPinConfigure(GPIO_PB2_T3CCP0);
    ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);

    ROM_GPIOPinConfigure(GPIO_PB4_T1CCP0);
    ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_4);
    
    ROM_GPIOPinConfigure(GPIO_PB6_T0CCP0);
    ROM_GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);

    for(int i=0; i<SERVO_MAX_COUNT; i++) 
    {
        ROM_SysCtlPeripheralEnable(g_servo_peripherals[i]);
        ROM_TimerConfigure(g_servo_timers[i], TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
        HWREG(g_servo_timers[i] + TIMER_O_CTL) |= TIMER_CTL_TAPWML;
        uint32_t period1 = ROM_SysCtlClockGet() / 50; /* 50Hz*/
        uint8_t extender1 = period1 >> 16;
        period1 &= 0xFFFF;
        ROM_TimerPrescaleSet(g_servo_timers[i], TIMER_A, extender1);
        ROM_TimerLoadSet(g_servo_timers[i], TIMER_A, period1);     
        ROM_TimerEnable(g_servo_timers[i], TIMER_A);
    }

    return true;
}

void servo_shutdown(void)
{
    for(int i=0; i<SERVO_MAX_COUNT; i++) 
        ROM_TimerDisable(g_servo_timers[i], TIMER_A);
}

bool servo_setpos(int id, int pos)
{
    if(id >= SERVO_MAX_COUNT)
        return false;

    uint32_t value = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180) * pos;
    value = value <= SERVO_MAX_PULSE ? value : SERVO_MAX_PULSE;
    value = value >= SERVO_MIN_PULSE ? value : SERVO_MIN_PULSE;
    uint32_t period2 = (ROM_SysCtlClockGet() / 50) * value / SERVO_PERIOD;
    uint8_t extender2 = period2 >> 16;
    period2 &= 0xFFFF;
    ROM_TimerPrescaleMatchSet(g_servo_timers[id], TIMER_A, extender2);
    ROM_TimerMatchSet(g_servo_timers[id], TIMER_A, period2);
    ROM_TimerEnable(g_servo_timers[id], TIMER_A);
    return true;
}
