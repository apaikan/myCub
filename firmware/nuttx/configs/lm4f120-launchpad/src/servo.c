#include <nuttx/arch.h>
#include <arch/board/board.h>
#include <arch/board/servo.h>
#include <arch/board/inc/hw_types.h>
#include <arch/board/inc/hw_memmap.h>
#include <arch/board/driverlib/pin_map.h>
#include <arch/board/driverlib/gpio.h>
#include <arch/board/driverlib/sysctl.h>
#include <arch/board/driverlib/timer.h>


servo_t g_servos[SERVO_MAX_COUNT]; // Servo configurations
uint8_t g_lastServo; // Used to keep track for servoAttach
uint16_t g_pulseTime; // Keep track of how far the period has gone


// Zero out all the values for all servos
void servoInit(void) {
    uint8_t i;
    for(i=0; i<SERVO_MAX_COUNT; i++) {
        servo_t *servo = &g_servos[i];
        servo->value = SERVO_MIN_PULSE;
        servo->port = 0;
        servo->pin = 0;
        servo->state = SERVO_STATE_INIT;
    }

    g_lastServo = 0;
    g_pulseTime = 0;
}

// Configure a servo and bind to a pin
servo_t *servoAttach(unsigned long port, unsigned char pin) {
    if(g_lastServo >= SERVO_MAX_COUNT - 1){
        g_lastServo = g_lastServo - 1;
    }

    servo_t *servo = &g_servos[g_lastServo];

    servo->port = port;
    servo->pin = pin;
    servo->state |= SERVO_STATE_ENABLED;

    g_lastServo++;

    return servo;
}

// Set pulse width for servo based on pos [0:180]
void servoSet(servo_t *servo, uint32_t pos) {            
    uint32_t value = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180) * pos;
    value = value <= SERVO_MAX_PULSE ? value : SERVO_MAX_PULSE;
    value = value >= SERVO_MIN_PULSE ? value : SERVO_MIN_PULSE;
    servo->value = value;
}

// Start the timers and interrupt frequency
void servoStart(void) {
   (void)irq_attach(LM_IRQ_TIMER2A, (xcpt_t)up_timerisr_servo);
    SysCtlPeripheralEnable(SERVO_TIMER_PERIPH);
    IntMasterEnable();
    TimerConfigure(SERVO_TIMER, TIMER_CFG_PERIODIC);
    TimerLoadSet(SERVO_TIMER, SERVO_TIMER_A, (SysCtlClockGet() / 1000000) * SERVO_TIMER_RESOLUTION);
    IntEnable(SERVO_TIMER_INTERRUPT);
    TimerIntEnable(SERVO_TIMER, SERVO_TIMER_TRIGGER);
    TimerEnable(SERVO_TIMER, SERVO_TIMER_A);
    up_enable_irq(LM_IRQ_TIMER2A);
}

// Triggered every SERVO_TIMER_RESOLUTION microseconds
int up_timerisr_servo(int irq, uint32_t *regs)
{
    // Clear the interrupt
    TimerIntClear(SERVO_TIMER, SERVO_TIMER_TRIGGER);
    
    // SERVO_TIMER_RESOLUTION microseconds have passed, increment each counter by that
    // to determine how long to set the pin high for
    g_pulseTime += SERVO_TIMER_RESOLUTION;

    if(g_pulseTime > SERVO_PERIOD) {
        g_pulseTime = 0;
    }

    // Loop through al servo configs and see if they need to be set low yet
    uint8_t i;
    for(i=0; i<SERVO_MAX_COUNT; i++) {
        servo_t *servo = &g_servos[i];
        
        if(servo->state & SERVO_STATE_ENABLED) {
            if(g_pulseTime >= servo->value) {
                // End of pulse, set low
                GPIOPinWrite(servo->port, servo->pin, 0);
            } else  {
                // Beginning of pulse, set high
                GPIOPinWrite(servo->port, servo->pin, servo->pin);
            }
        }
    }

    return 0;
}

