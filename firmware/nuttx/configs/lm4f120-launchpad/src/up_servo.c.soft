/****************************************************************************
 * arch/arm/src/lm4f/lm4f_servo.c
 *
 *   Copyright (C) 2013-2014 Ali Paikan. All rights reserved.
 *   Author: Ali Paikan <ali.paikan@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/servo.h>
#include <arch/board/board.h>


#include "chip.h"
#include "arch/board/inc/hw_ints.h"
#include <arch/board/inc/hw_types.h>
#include <arch/board/inc/hw_memmap.h>
//#include <arch/board/driverlib/pin_map.h>
//#include <arch/board/driverlib/gpio.h>
#include <arch/board/driverlib/sysctl.h>
#include <arch/board/driverlib/timer.h>
#include <arch/board/driverlib/interrupt.h>

#include "lm_gpio.h"


/*
 * TODO: these paramaters should be taken from nuttx/config.h
 */ 
#define SERVO_MAX_COUNT         5
#define SERVO_TIMER             TIMER2_BASE
#define SERVO_TIMER_TRIGGER     TIMER_TIMA_TIMEOUT
#define SERVO_TIMER_INTERRUPT   INT_TIMER2A
#define SERVO_TIMER_A           TIMER_A
#define SERVO_TIMER_PERIPH      SYSCTL_PERIPH_TIMER2
#define SERVO_TIMER_RESOLUTION  10                      // Microseconds
#define SERVO_MIN_PULSE         544
#define SERVO_MAX_PULSE         2400
#define SERVO_PERIOD            20000


/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure representst the state of one Servo controller */

struct lm4f_servo_s
{
  FAR const struct servo_ops_s *ops;    /* Servo operations */
  uint16_t                      count;  /* the number of the initialized servos */  
  uint16_t                      id;     /* the id the servo */
  uint16_t                      pos;    /* servo position value */ 
  unsigned long                 base;   /* The base address of the port */
  unsigned char                 pin;    /* Output pin configuration */
};

/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/
/* Servo driver methods */

static int servo_setup(FAR struct servo_lowerhalf_s *dev);
static int servo_shutdown(FAR struct servo_lowerhalf_s *dev);
static int servo_attach(FAR struct servo_lowerhalf_s *dev,
                        FAR const struct servo_info_s *info);
static int servo_setpos(FAR struct servo_lowerhalf_s *dev,
                        FAR const struct servo_info_s *info);
static int servo_ioctl(FAR struct servo_lowerhalf_s *dev,
                       int cmd, unsigned long arg);

int up_timerisr_servo(int irq, uint32_t *regs);

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* This is the list of lower half SERVO driver methods used by the upper half driver */

static const struct servo_ops_s g_servoops =
{
  .setup      = servo_setup,
  .shutdown   = servo_shutdown,
  .attach     = servo_attach,
  .setpos     = servo_setpos,
  //.ioctl      = servo_ioctl,
};

static struct lm4f_servo_s g_servodev =
{
  .ops        = &g_servoops,
  .count      = SERVO_MAX_COUNT,
};

typedef enum {
    SERVO_STATE_INIT = 0,
    SERVO_STATE_ENABLED = 1
} SERVO_STATE;

typedef struct {
    uint16_t value;
    unsigned long port;
    unsigned char pin;
    uint8_t state;
} servo_t;

servo_t g_servos[SERVO_MAX_COUNT];      // Servo configurations
uint16_t g_pulseTime;                   // Keep track of how far the period has gone


/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int servo_setup(FAR struct servo_lowerhalf_s *dev)
{
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    uint8_t i;
    for(i=0; i<SERVO_MAX_COUNT; i++) {
        servo_t *servo = &g_servos[i];
        servo->value = SERVO_MIN_PULSE;
        servo->port = 0;
        servo->pin = 0;
        servo->state = SERVO_STATE_INIT;
    }
    g_pulseTime = 0;

   (void)irq_attach(LM_IRQ_TIMER2A, (xcpt_t)up_timerisr_servo);
    SysCtlPeripheralEnable(SERVO_TIMER_PERIPH);
    IntMasterEnable();
    TimerConfigure(SERVO_TIMER, TIMER_CFG_PERIODIC);
    TimerLoadSet(SERVO_TIMER, SERVO_TIMER_A, (SysCtlClockGet() / 1000000) * SERVO_TIMER_RESOLUTION);
    IntEnable(SERVO_TIMER_INTERRUPT);
    TimerIntEnable(SERVO_TIMER, SERVO_TIMER_TRIGGER);
    TimerEnable(SERVO_TIMER, SERVO_TIMER_A);
    up_enable_irq(LM_IRQ_TIMER2A);
    return OK;
}


static int servo_shutdown(FAR struct servo_lowerhalf_s *dev)
{
    
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    up_disable_irq(LM_IRQ_TIMER2A);
    TimerDisable(SERVO_TIMER, SERVO_TIMER_A);
    TimerIntDisable(SERVO_TIMER, SERVO_TIMER_TRIGGER);
    SysCtlPeripheralDisable(SERVO_TIMER_PERIPH);
    irq_detach(LM_IRQ_TIMER2A);
    //printf("%s: %d\n", __FILE__, __LINE__);

    return OK;
}

static int servo_attach(FAR struct servo_lowerhalf_s *dev,
                     FAR const struct servo_info_s *info)
{
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    //printf("info->port: %x, info->pin: %x, GPIO_POTRD :%x, GPIO_PIN_0): %x \n", 
    //        info->port, info->pin, GPIO_PORTD,  GPIO_PIN_0);
    if(info->id >= SERVO_MAX_COUNT)
    {
        dbg("servo_attach: servo %d is out of range! (SERVO_MAX_COUNT : %d)\n", 
            info->id, SERVO_MAX_COUNT);
        return ERANGE;
    }

    servo_t *servo = &g_servos[info->id];
    servo->port = info->port;
    servo->pin = info->pin;

    /* configure the servo pin */
    lm_configgpio(GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | servo->port | servo->pin);

    return OK;
}

static int servo_setpos(FAR struct servo_lowerhalf_s *dev,
                        FAR const struct servo_info_s *info)
{
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;

    if(info->id >= SERVO_MAX_COUNT)
    {
        dbg("servo_attach: servo %d is out of range! (SERVO_MAX_COUNT : %d)\n", 
            info->id, SERVO_MAX_COUNT);
        return ERANGE;
    }

    servo_t *servo = &g_servos[info->id];
    uint32_t value = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180) * info->pos;
    value = value <= SERVO_MAX_PULSE ? value : SERVO_MAX_PULSE;
    value = value >= SERVO_MIN_PULSE ? value : SERVO_MIN_PULSE;
    servo->value = value;
    servo->state |= SERVO_STATE_ENABLED;
    return OK;
}


static int servo_ioctl(FAR struct servo_lowerhalf_s *dev, int cmd, unsigned long arg)
{
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    return -ENOTTY;
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
                lm_gpiowrite( GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | servo->port | servo->pin, false); 
            } else  {
                // Beginning of pulse, set high
                lm_gpiowrite( GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | servo->port | servo->pin, true); 
            }
        }
    }

    return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lm4f_servoinitialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level SERVO driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use.  The number of valid timer
 *     IDs varies with the STM32 MCU and MCU family but is somewhere in
 *     the range of {1,..,14}.
 *
 * Returned Value:
 *   On success, a pointer to the STM32 lower half SERVO driver is returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/

FAR struct servo_lowerhalf_s *lm4f_servoinitialize()
{
    /* return device intance */ 
    FAR struct lm4f_servo_s *lower;
    lower = &g_servodev;   
    return (FAR struct servo_lowerhalf_s *)lower;
}


/************************************************************************************
 * Name: servo_devinit
 *
 * Description:
 *   All LMF4 architectures must provide the following interface to work with
 *   examples/servo.
 *
 ************************************************************************************/

int servo_devinit(void)
{
  static bool servo_initialized = false;
  struct servo_lowerhalf_s *servo;
  int ret;

  /* Have we already initialized? */

  if (!servo_initialized)
    {
      servo = lm4f_servoinitialize();
      if (!servo)
        {
          dbg("Failed to get the LM4F servo lower half\n");
          return -ENODEV;
        }

      /* Register the SERVO driver at "/dev/servo0" */
      ret = servo_register("/dev/servo0", servo);
      if (ret < 0)
        {
          adbg("servo_register failed: %d\n", ret);
          return ret;
        }

      /* Now we are initialized */

      servo_initialized = true;
    }

  return OK;
}

