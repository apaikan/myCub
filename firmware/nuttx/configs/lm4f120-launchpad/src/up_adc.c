/****************************************************************************
 * arch/arm/src/lm4f/lm4f_adclm4f.c
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
#include <nuttx/analog/adc_lm4f.h>
#include <arch/board/board.h>


#include "chip.h"
#include <arch/board/inc/hw_types.h>
#include <arch/board/inc/hw_memmap.h>
//#include <arch/board/driverlib/pin_map.h>
#include <arch/board/driverlib/gpio.h>
#include <arch/board/driverlib/sysctl.h>
#include <arch/board/driverlib/adc.h>

#define CHANNEL_MAX_COUNT           12

/****************************************************************************
 * Private Types
 ****************************************************************************/
/* This structure representst the state of one Servo controller */

struct lm4f_adclm4f_s
{
  FAR const struct adclm4f_ops_s *ops;    /* Servo operations */
  size_t            count;
};

/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/
/* Servo driver methods */

static int adclm4f_setup(FAR struct adclm4f_lowerhalf_s *dev);
static int adclm4f_shutdown(FAR struct adclm4f_lowerhalf_s *dev);
static int adclm4f_configure(FAR struct adclm4f_lowerhalf_s *dev,
                        FAR const struct adclm4f_info_s *info);
static int adclm4f_read(FAR struct adclm4f_lowerhalf_s *dev,
                      FAR char* buff, size_t buflen);
static int adclm4f_ioctl(FAR struct adclm4f_lowerhalf_s *dev,
                       int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/
/* This is the list of lower half RANGE driver methods used by the upper half driver */

static const struct adclm4f_ops_s g_adclm4fops =
{
  .setup      = adclm4f_setup,
  .shutdown   = adclm4f_shutdown,
  .configure     = adclm4f_configure,
  .read       = adclm4f_read,
  //.ioctl      = adclm4f_ioctl,
};

static struct lm4f_adclm4f_s g_adclm4fdev =
{
  .ops        = &g_adclm4fops,
};

typedef struct {
    unsigned char   channel;
    unsigned long   freq;
    unsigned long   nsamples;
} adclm4f_t;


adclm4f_t   g_adclm4info;   

unsigned long g_adcchannels[CHANNEL_MAX_COUNT] = { 
 ADC_CTL_CH0, ADC_CTL_CH1, ADC_CTL_CH2, ADC_CTL_CH3, ADC_CTL_CH4, ADC_CTL_CH5, 
 ADC_CTL_CH6, ADC_CTL_CH7, ADC_CTL_CH8, ADC_CTL_CH9, ADC_CTL_CH10, ADC_CTL_CH11, 
};


/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int adclm4f_setup(FAR struct adclm4f_lowerhalf_s *dev)
{
    FAR struct lm4f_adclm4ftimer_s *priv = (FAR struct lm4f_adclm4ftimer_s *)dev;

    g_adclm4info.channel = 0;
    g_adclm4info.nsamples = 1;
    g_adclm4info.freq = 1000;  // 1KHrz default
    // The ADC0 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);    

    //SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    //TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC_UP);
    //TimerEnable(TIMER5_BASE, TIMER_A);
    return OK;
}


static int adclm4f_shutdown(FAR struct adclm4f_lowerhalf_s *dev)
{
    SysCtlPeripheralDisable(SYSCTL_PERIPH_ADC0);
    //TimerDisable(TIMER5_BASE, TIMER_A);
    return OK;
}

static int adclm4f_configure(FAR struct adclm4f_lowerhalf_s *dev,
                     FAR const struct adclm4f_info_s *info)
{
    if(info->channel >= CHANNEL_MAX_COUNT)
    {
        dbg("adclm4f_configure: cannot %d is out of range! (CHANNEL_MAX_COUNT : %d)\n", 
            info->channel, CHANNEL_MAX_COUNT);
        return ERANGE;
    }
   
    //printf("adclm4f_configure: channel:%d, freq: %d \n", 
    //        info->channel, info->freq);
   
    g_adclm4info.channel = info->channel;
    g_adclm4info.freq = info->freq;
    g_adclm4info.nsamples = info->nsamples;

    switch(info->channel) {
        case 0: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);
            break;
        case 1: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
            break;
        case 2: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
            break;
        case 3: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);
            break;
        case 4: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
            break;
        case 5: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_2);
            break;
        case 6: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_1);
            break;
        case 7: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_0);
            break;
        case 8: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_5);
            break;
        case 9: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_4);
            break;
        case 10: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
            GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_4);
            break;
        case 11: 
            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
            GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);
            break;
        default:
            break;
    };
    
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, g_adcchannels[info->channel] | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
           
    //lm_configgpio(GPIO_FUNC_OUTPUT | GPIO_VALUE_ONE | adclm4f->trig_port | adclm4f->trig_pin);
    //lm_configgpio(GPIO_FUNC_INPUT  | GPIO_VALUE_ONE | adclm4f->echo_port | adclm4f->echo_pin);
    return OK;
}


static int adclm4f_read(FAR struct adclm4f_lowerhalf_s *dev,
                      FAR char* buff, size_t buflen)
{
    // Loop through al adclm4f configs and see if they need to be set low yet
    int i;
    unsigned long ulADC0_Value[1];
    FAR uint16_t *ptr = (uint16_t*) buff; 
    unsigned long delay = SysCtlClockGet() / g_adclm4info.freq / 3;
    for(i=0; (i<g_adclm4info.nsamples) && (i*sizeof(uint16_t) < buflen); i++)
    {
        ADCProcessorTrigger(ADC0_BASE, 3);
        while(!ADCIntStatus(ADC0_BASE, 3, false));
        ADCIntClear(ADC0_BASE, 3);
        ADCSequenceDataGet(ADC0_BASE, 3, ulADC0_Value);
        //printf("adclm4f_read: len: %d\n", ulADC0_Value[0]);
        *ptr = (uint16_t) ulADC0_Value[0];
        ptr++;
        SysCtlDelay(delay);
    }
    return i;
}


static int adclm4f_ioctl(FAR struct adclm4f_lowerhalf_s *dev, int cmd, unsigned long arg)
{
    return -ENOTTY;
}


/****************************************************************************
 * Public Functions
 ****************************************************************************/

FAR struct adclm4f_lowerhalf_s *lm4f_adclm4finitialize()
{
    /* return device intance */ 
    FAR struct lm4f_adclm4f_s *lower;
    lower = &g_adclm4fdev;   
    return (FAR struct adclm4f_lowerhalf_s *)lower;
}


/************************************************************************************
 * Name: adclm4f_devinit
 *
 * Description:
 *   All LMF4 architectures must provide the following interface to work with
 *   examples/adclm4f.
 *
 ************************************************************************************/

int adclm4f_devinit(void)
{
  static bool adclm4f_initialized = false;
  struct adclm4f_lowerhalf_s *adclm4f;
  int ret;

  /* Have we already initialized? */

  if (!adclm4f_initialized)
    {
      adclm4f = lm4f_adclm4finitialize();
      if (!adclm4f)
        {
          dbg("Failed to get the LM4F adclm4f lower half\n");
          return -ENODEV;
        }

      /* Register the RANGE driver at "/dev/adclm4f0" */
      ret = adclm4f_register("/dev/adclm4f0", adclm4f);
      if (ret < 0)
        {
          adbg("adclm4f_register failed: %d\n", ret);
          return ret;
        }

      /* Now we are initialized */

      adclm4f_initialized = true;
    }

  return OK;
}

