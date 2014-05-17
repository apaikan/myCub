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
#include <arch/board/inc/hw_ints.h>
#include <arch/board/inc/hw_types.h>
#include <arch/board/inc/hw_memmap.h>
#include <arch/board/inc/hw_timer.h>
#include <arch/board/driverlib/pin_map.h>
#include <arch/board/driverlib/gpio.h>
#include <arch/board/driverlib/sysctl.h>
#include <arch/board/driverlib/timer.h>
//#include <arch/board/driverlib/interrupt.h>

//#include "lm_gpio.h"

#ifdef CONFIG_SERVO

/*
 * TODO: these paramaters should be taken from nuttx/config.h
 */ 
#define SERVO_MAX_COUNT         4
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

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int servo_setup(FAR struct servo_lowerhalf_s *dev)
{
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    uint8_t i;
   
    /* Configuring pwm pinouts */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PB0_T2CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_0);
    
    GPIOPinConfigure(GPIO_PB2_T3CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_2);

    GPIOPinConfigure(GPIO_PB4_T1CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_4);
    
    GPIOPinConfigure(GPIO_PB6_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_6);
 
    //lm_configgpio(GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | servo->port | servo->pin);

    for(i=0; i<SERVO_MAX_COUNT; i++) {

        SysCtlPeripheralEnable(g_servo_peripherals[i]);
        TimerConfigure(g_servo_timers[i], TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM);
        HWREG(g_servo_timers[i] + TIMER_O_CTL) |= TIMER_CTL_TAPWML;
        uint32_t period1 = SysCtlClockGet() / 50; /* 50Hz*/
        uint8_t extender1 = period1 >> 16;
        period1 &= 0xFFFF;
        TimerPrescaleSet(g_servo_timers[i], TIMER_A, extender1);
        TimerLoadSet(g_servo_timers[i], TIMER_A, period1);     
    }
    return OK;
}


static int servo_shutdown(FAR struct servo_lowerhalf_s *dev)
{    
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    uint8_t i;
    for(i=0; i<SERVO_MAX_COUNT; i++) 
        TimerDisable(g_servo_timers[i], TIMER_A);
    return OK;
}

static int servo_attach(FAR struct servo_lowerhalf_s *dev,
                     FAR const struct servo_info_s *info)
{
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    return -ENODEV;
}

static int servo_setpos(FAR struct servo_lowerhalf_s *dev,
                        FAR const struct servo_info_s *info)
{
    irqstate_t flags;
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;

    flags = irqsave();
    sched_lock();
    if(info->id >= SERVO_MAX_COUNT)
    {
        dbg("servo_attach: servo %d is out of range! (SERVO_MAX_COUNT : %d)\n", 
            info->id, SERVO_MAX_COUNT);
        return ERANGE;
    }

    uint32_t value = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180) * info->pos;
    value = value <= SERVO_MAX_PULSE ? value : SERVO_MAX_PULSE;
    value = value >= SERVO_MIN_PULSE ? value : SERVO_MIN_PULSE;
    uint32_t period2 = (SysCtlClockGet() / 50) * value / SERVO_PERIOD;
    uint8_t extender2 = period2 >> 16;
    period2 &= 0xFFFF;
    TimerPrescaleMatchSet(g_servo_timers[info->id], TIMER_A, extender2);
    TimerMatchSet(g_servo_timers[info->id], TIMER_A, period2);
    TimerEnable(g_servo_timers[info->id], TIMER_A);
    irqrestore(flags);
    sched_unlock();
    return OK;
}


static int servo_ioctl(FAR struct servo_lowerhalf_s *dev, int cmd, unsigned long arg)
{
    FAR struct lm4f_servotimer_s *priv = (FAR struct lm4f_servotimer_s *)dev;
    return -ENODEV;
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

#endif //CONFIG_SERVO
