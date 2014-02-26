/****************************************************************************
 * arch/arm/src/lm4f/lm4f_range.c
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
#include <stdlib.h>
#include <string.h>

#include <nuttx/arch.h>
#include <nuttx/sensors/range_srf04.h>
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

#ifdef CONFIG_RANGE

#include "lm_gpio.h"


#define RANGE_MAX_COUNT     4


/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure representst the state of one Servo controller */

struct lm4f_range_s
{
  FAR const struct range_ops_s *ops;    /* Servo operations */
  size_t            count;
};

/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/
/* Servo driver methods */

static int range_setup(FAR struct range_lowerhalf_s *dev);
static int range_shutdown(FAR struct range_lowerhalf_s *dev);
static int range_attach(FAR struct range_lowerhalf_s *dev,
                        FAR const struct range_info_s *info);
static int range_detach(FAR struct range_lowerhalf_s *dev,
                        FAR const struct range_info_s *info);
static int range_read(FAR struct range_lowerhalf_s *dev,
                      FAR char* buff, size_t buflen);
static int range_ioctl(FAR struct range_lowerhalf_s *dev,
                       int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* This is the list of lower half RANGE driver methods used by the upper half driver */

static const struct range_ops_s g_rangeops =
{
  .setup      = range_setup,
  .shutdown   = range_shutdown,
  .attach     = range_attach,
  .detach     = range_detach,
  .read       = range_read,
  //.ioctl      = range_ioctl,
};

static struct lm4f_range_s g_rangedev =
{
  .ops        = &g_rangeops,
  .count      = RANGE_MAX_COUNT,
};

typedef enum {
    RANGE_STATE_INIT = 0,
    RANGE_STATE_ENABLED = 1
} RANGE_STATE;

typedef struct {
    unsigned long trig_port;
    unsigned char trig_pin;
    unsigned long echo_port;
    unsigned char echo_pin;
    uint16_t value;
    RANGE_STATE state;
} range_t;

range_t g_ranges[RANGE_MAX_COUNT];      // Servo configurations


/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int range_setup(FAR struct range_lowerhalf_s *dev)
{

    //FAR struct lm4f_rangetimer_s *priv = (FAR struct lm4f_rangetimer_s *)dev;
    uint8_t i;
    for(i=0; i<RANGE_MAX_COUNT; i++) {
        range_t *range = &g_ranges[i];
        range->trig_port = 0;
        range->trig_pin = 0;
        range->echo_port = 0;
        range->echo_pin = 0;
        range->value = 0;
        range->state = RANGE_STATE_INIT;
    }

    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC_UP);
    TimerEnable(TIMER5_BASE, TIMER_A);
    return OK;
}


static int range_shutdown(FAR struct range_lowerhalf_s *dev)
{
    TimerDisable(TIMER5_BASE, TIMER_A);
    return OK;
}

static int range_attach(FAR struct range_lowerhalf_s *dev,
                     FAR const struct range_info_s *info)
{
    //printf("info->port: %x, info->pin: %x, GPIO_POTRD :%x, GPIO_PIN_0): %x \n", 
    //        info->port, info->pin, GPIO_PORTD,  GPIO_PIN_0);
    if(info->id >= RANGE_MAX_COUNT)
    {
        dbg("range_attach: sensor %d is out of range! (RANGE_MAX_COUNT : %d)\n", 
            info->id, RANGE_MAX_COUNT);
        return ERANGE;
    }

    range_t *range = &g_ranges[info->id];
    range->trig_port = info->trig_port;
    range->trig_pin = info->trig_pin;
    range->echo_port = info->echo_port;
    range->echo_pin = info->echo_pin;
    range->state  = RANGE_STATE_ENABLED;

    /* configure the range pin */
    lm_configgpio(GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | range->trig_port | range->trig_pin);
    lm_configgpio(GPIO_FUNC_INPUT  | GPIO_VALUE_ONE | range->echo_port | range->echo_pin);

    return OK;
}

static int range_detach(FAR struct range_lowerhalf_s *dev,
                     FAR const struct range_info_s *info)
{
    //printf("info->port: %x, info->pin: %x, GPIO_POTRD :%x, GPIO_PIN_0): %x \n", 
    //        info->port, info->pin, GPIO_PORTD,  GPIO_PIN_0);
    if(info->id >= RANGE_MAX_COUNT)
    {
        dbg("range_attach: sensor %d is out of range! (RANGE_MAX_COUNT : %d)\n", 
            info->id, RANGE_MAX_COUNT);
        return ERANGE;
    }

    range_t *range = &g_ranges[info->id];
    range->trig_port = 0;
    range->trig_pin = 0;
    range->echo_port = 0;
    range->echo_pin = 0;
    range->state  = RANGE_STATE_INIT;
    
    /*
     * TODO: disable the pins
     */
    //lm_configgpio(GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | range->trig_port | range->trig_pin);
    //lm_configgpio(GPIO_FUNC_INPUT  | GPIO_VALUE_ONE | range->echo_port | range->echo_pin);

    return OK;
}

unsigned long pulse_in( unsigned long port, unsigned char pin)
{
    //TimerLoadSet(TIMER5_BASE, TIMER_A, 4294967295L);
    //TimerDisable(TIMER5_BASE, TIMER_A);     
    //TimerEnable(TIMER5_BASE, TIMER_A);  
    unsigned long t, t2;
    
    // wait for any previous pulse to end 
    t = TimerValueGet(TIMER5_BASE, TIMER_A);
    while(lm_gpioread(GPIO_FUNC_INPUT  | GPIO_VALUE_ONE | port | pin, 0) == true)
        if((TimerValueGet(TIMER5_BASE, TIMER_A) - t) > 50000000L)
        {
            //printf("wait for previous pulse ... [timeout]\n");
            return 0;
        }   
    
    // wait for the pulse to start
    t = TimerValueGet(TIMER5_BASE, TIMER_A);
    while(lm_gpioread(GPIO_FUNC_INPUT  | GPIO_VALUE_ONE | port | pin, 0) == false)
        if((TimerValueGet(TIMER5_BASE, TIMER_A) - t) > 50000000L)
        {
            //printf("wait for pulse to start ... [timeout]\n");
            return 0;
        }

    // wait for the pulse to stop    
    //t = TimerValueGet(TIMER5_BASE, TIMER_A);
    while(lm_gpioread(GPIO_FUNC_INPUT  | GPIO_VALUE_ONE | port | pin, 0) == true)
    {
        t2 = TimerValueGet(TIMER5_BASE, TIMER_A);
        if((t2 - t) > 50000000L)
        {
            //printf("wait for pulse to stop ... [timeout]\n");
            return 0;
        }            
    }

    //printf("Pulse start : %lu, Pulse end: %lu, width %lu\n", t, t2, t2 - t);
    return ((t2-t) / 85L);
}


static int range_read(FAR struct range_lowerhalf_s *dev,
                      FAR char* buff, size_t buflen)
{
    // Loop through al range configs and see if they need to be set low yet
    uint8_t i;
    size_t len = 0;
    buff[0] = NULL;
    FAR char* ptr = buff;
    char dummy[64];

    for(i=0; i<RANGE_MAX_COUNT; i++) 
    {
        range_t *range = &g_ranges[i];       
        if(range->state & RANGE_STATE_ENABLED) 
        {
            //tring the sensor  
            lm_gpiowrite( GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | range->trig_port | range->trig_pin, false);           
            //SysCtlDelay(SysCtlClockGet() / 5000000 / 3); 
            usleep(2);
            lm_gpiowrite( GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | range->trig_port | range->trig_pin, true);
            //SysCtlDelay(SysCtlClockGet() / 200000 / 3); 
            usleep(10);
            lm_gpiowrite( GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | range->trig_port | range->trig_pin, false);
            unsigned long width = pulse_in(range->echo_port, range->echo_pin); 
            unsigned int dist = (int)(width / 5.2466);
            sprintf(dummy, "(%d %d) ", i, (dist<4000)? dist : 0);
            strncpy(ptr, dummy, buflen-strlen(buff));
            ptr += strlen(dummy);
        }
    }   

    return (ptr == buff) ? 0 : strlen(buff)+1;
}


static int range_ioctl(FAR struct range_lowerhalf_s *dev, int cmd, unsigned long arg)
{
    return -ENOTTY;
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

FAR struct range_lowerhalf_s *lm4f_rangeinitialize()
{
    /* return device intance */ 
    FAR struct lm4f_range_s *lower;
    lower = &g_rangedev;   
    return (FAR struct range_lowerhalf_s *)lower;
}


/************************************************************************************
 * Name: range_devinit
 *
 * Description:
 *   All LMF4 architectures must provide the following interface to work with
 *   examples/range.
 *
 ************************************************************************************/

int range_devinit(void)
{
  static bool range_initialized = false;
  struct range_lowerhalf_s *range;
  int ret;

  /* Have we already initialized? */

  if (!range_initialized)
    {
      range = lm4f_rangeinitialize();
      if (!range)
        {
          dbg("Failed to get the LM4F range lower half\n");
          return -ENODEV;
        }

      /* Register the RANGE driver at "/dev/range0" */
      ret = range_register("/dev/range0", range);
      if (ret < 0)
        {
          adbg("range_register failed: %d\n", ret);
          return ret;
        }

      /* Now we are initialized */

      range_initialized = true;
    }

  return OK;
}

#endif //CONFIG_RANGE

