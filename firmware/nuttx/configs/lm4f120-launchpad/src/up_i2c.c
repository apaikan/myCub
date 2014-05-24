/*******************************************************************************
 * arch/arm/src/lm4fxx/lm4f_i2c.c
 *
 *   Copyright (C) 2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Ported from from the LMM4F version:
 *
 *   Copyright (C) 2011 Li Zhuoyi. All rights reserved.
 *   Author: Li Zhuoyi <lzyy.cn@gmail.com>
 *   History: 0.1 2011-08-20 initial version
 *
 * Derived from arch/arm/src/lpc31xx/lpc31_i2c.c
 *
 *   Author: Ali Paikan
 *
 *   Copyright (C) 2010-2011 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 *******************************************************************************/

/*******************************************************************************
 * Included Files
 *******************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/i2c.h>

#include <arch/irq.h>
#include <arch/board/board.h>

/*
#include "wdog.h"
#include "chip.h"
#include "up_arch.h"
#include "up_internal.h"
#include "os_internal.h"
*/

#include "inc/hw_gpio.h"
#include "inttypes.h"
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/i2c.h"


#if defined(CONFIG_LMM4F_I2C0) || defined(CONFIG_LMM4F_I2C1)

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Private Data
 ****************************************************************************/

struct lm4f_i2cdev_s
{
  struct i2c_dev_s dev;        /* Generic I2C device */
  unsigned int     base;       /* Base address of registers */
  uint8_t          address;
  uint8_t          nbits;
};

#ifdef CONFIG_LMM4F_I2C0
static struct lm4f_i2cdev_s g_i2c0dev;
#endif
#ifdef CONFIG_LMM4F_I2C1
static struct lm4f_i2cdev_s g_i2c1dev;
#endif
#ifdef CONFIG_LMM4F_I2C2
static struct lm4f_i2cdev_s g_i2c2dev;
#endif
#ifdef CONFIG_LMM4F_I2C3
static struct lm4f_i2cdev_s g_i2c3dev;
#endif


/****************************************************************************
 * I2C device operations
 ****************************************************************************/

static uint32_t i2c_setfrequency(FAR struct i2c_dev_s *dev,
                                 uint32_t frequency);
static int      i2c_setaddress(FAR struct i2c_dev_s *dev, int addr,
                               int nbits);
static int      i2c_write(FAR struct i2c_dev_s *dev, const uint8_t *buffer,
                          int buflen);
static int      i2c_read(FAR struct i2c_dev_s *dev, uint8_t *buffer,
                         int buflen);
#ifdef CONFIG_I2C_TRANSFE
static int      i2c_transfer(FAR struct i2c_dev_s *dev,
                             FAR struct i2c_msg_s *msgs, int count);
#endif

struct i2c_ops_s lm4f_i2c_ops =
{
  .setfrequency = i2c_setfrequency,
  .setaddress   = i2c_setaddress,
  .write        = i2c_write,
  .read         = i2c_read,
#ifdef CONFIG_I2C_TRANSFER
  .transfer     = i2c_transfer
#endif
};

/*******************************************************************************
 * Name: lm4f_i2c_setfrequency
 *
 * Description:
 *   Set the frequence for the next transfer
 *
 *******************************************************************************/

static uint32_t i2c_setfrequency(FAR struct i2c_dev_s *dev, uint32_t frequency)
{
    struct lm4f_i2cdev_s *priv = (struct lm4f_i2cdev_s *) dev;

    if (frequency == 400) 
    {
        I2CMasterInitExpClk(priv->base, SysCtlClockGet(), 1); // 1 : 400Khz, 0 : 100Khz
        return 400;
    }    
    
    I2CMasterInitExpClk(priv->base, SysCtlClockGet(), 0); // 1 : 400Khz, 0 : 100Khz
    return frequency;
}

/*******************************************************************************
 * Name: lm4f_i2c_setaddress
 *
 * Description:
 *   Set the I2C slave address for a subsequent read/write
 *
 *******************************************************************************/

static int i2c_setaddress(FAR struct i2c_dev_s *dev, int addr, int nbits)
{
    struct lm4f_i2cdev_s *priv = (struct lm4f_i2cdev_s *)dev;
    priv->address = addr; 
    priv->nbits = nbits;
    return OK;
}

/*******************************************************************************
 * Name: lm4f_i2c_write
 *
 * Description:
 *   Send a block of data on I2C using the previously selected I2C
 *   frequency and slave address.
 *
 *******************************************************************************/

static int i2c_write(FAR struct i2c_dev_s *dev, const uint8_t *buffer,
                     int buflen)
{
    struct lm4f_i2cdev_s *priv = (struct lm4f_i2cdev_s *)dev;

    DEBUGASSERT(dev != NULL);

    uint8_t nBytesCount;            // local variable used for byte counting/state determination
    uint16_t MasterOptionCommand;           // used to assign the commands for I2CMasterControl() function

    // Wait until master module is done transferring.
    while(I2CMasterBusy(priv->base));

    // Tell the master module what address it will place on the bus when
    // writing to the slave.
    I2CMasterSlaveAddrSet(priv->base, priv->address, false);

    // Place the value to be sent in the data register.
    I2CMasterDataPut(priv->base, priv->nbits);

    // Initiate send of data from the master.
    I2CMasterControl(priv->base, I2C_MASTER_CMD_BURST_SEND_START);

    // Wait until master module is done transferring.
    while(I2CMasterBusy(priv->base));

    // Check for errors.
    if(I2CMasterErr(priv->base) != I2C_MASTER_ERR_NONE)
        return -ETIMEDOUT;

    // Start with CONT for more than one byte to write
    MasterOptionCommand = I2C_MASTER_CMD_BURST_SEND_CONT;

    for(nBytesCount = 0; nBytesCount < buflen; nBytesCount++)
    {
        // The second and intermittent byte has to be send with CONTINUE control word
        if(nBytesCount == 1)
                MasterOptionCommand = I2C_MASTER_CMD_BURST_SEND_CONT;

        // The last byte has to be send with FINISH control word
        if(nBytesCount == buflen - 1)
                MasterOptionCommand = I2C_MASTER_CMD_BURST_SEND_FINISH;

        // Re-configure to SINGLE if there is only one byte to write
        if(buflen == 1)
                MasterOptionCommand = I2C_MASTER_CMD_SINGLE_SEND;

        // Send data byte
        I2CMasterDataPut(priv->base, buffer[nBytesCount]);

        // Initiate send of data from the master.
        I2CMasterControl(priv->base, MasterOptionCommand);

        // Wait until master module is done transferring.
        while(I2CMasterBusy(priv->base));

        // Check for errors.
        if(I2CMasterErr(priv->base) != I2C_MASTER_ERR_NONE)
            return -ETIMEDOUT;
    }

    return OK;
}

/*******************************************************************************
 * Name: lm4f_i2c_read
 *
 * Description:
 *   Receive a block of data on I2C using the previously selected I2C
 *   frequency and slave address.
 *
 *******************************************************************************/

static int i2c_read(FAR struct i2c_dev_s *dev, uint8_t *buffer, int buflen)
{
    struct lm4f_i2cdev_s *priv = (struct lm4f_i2cdev_s *)dev;

    DEBUGASSERT(dev != NULL);

    uint8_t nBytesCount;            // local variable used for byte counting/state determination
    uint16_t MasterOptionCommand;           // used to assign the commands for I2CMasterControl() function

    // Wait until master module is done transferring.
    while(I2CMasterBusy(priv->base));

    // Tell the master module what address it will place on the bus when
    // writing to the slave.
    I2CMasterSlaveAddrSet(priv->base, priv->address, 0);

    // Place the command to be sent in the data register.
    I2CMasterDataPut(priv->base, priv->nbits);

    // Initiate send of data from the master.
    I2CMasterControl(priv->base, I2C_MASTER_CMD_SINGLE_SEND);

    // Wait until master module is done transferring.
    while(I2CMasterBusy(priv->base));

    // Check for errors.
    if(I2CMasterErr(priv->base) != I2C_MASTER_ERR_NONE)
        return -ETIMEDOUT;


    // Tell the master module what address it will place on the bus when
    // reading from the slave.
    I2CMasterSlaveAddrSet(priv->base, priv->address, true);

    // Start with BURST with more than one byte to write
    MasterOptionCommand = I2C_MASTER_CMD_BURST_RECEIVE_START;

    for(nBytesCount = 0; nBytesCount < buflen; nBytesCount++)
    {
        // The second and intermittent byte has to be read with CONTINUE control word
        if(nBytesCount == 1)
                MasterOptionCommand = I2C_MASTER_CMD_BURST_RECEIVE_CONT;

        // The last byte has to be send with FINISH control word
        if(nBytesCount == buflen - 1)
                MasterOptionCommand = I2C_MASTER_CMD_BURST_RECEIVE_FINISH;

        // Re-configure to SINGLE if there is only one byte to read
        if(buflen == 1)
                MasterOptionCommand = I2C_MASTER_CMD_SINGLE_RECEIVE;

        // Initiate read of data from the slave.
        I2CMasterControl(priv->base, MasterOptionCommand);

        // Wait until master module is done reading.
        while(I2CMasterBusy(priv->base));

        // Check for errors.
        if(I2CMasterErr(priv->base) != I2C_MASTER_ERR_NONE)
            return -ETIMEDOUT;

            buffer[nBytesCount] = I2CMasterDataGet(priv->base);
    }

    return OK;
}


#ifdef CONFIG_I2C_TRANSFER
int i2c_transfer(FAR struct i2c_dev_s *dev,
                 FAR struct i2c_msg_s *msgs, int count)
{
    return -ETIMEDOUT;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/*******************************************************************************
 * Name: up_i2cinitialize
 *
 * Description:
 *   Initialise an I2C device
 *
 *******************************************************************************/

struct i2c_dev_s *up_i2cinitialize(int port)
{
  struct lm4f_i2cdev_s *priv;

  if (port>3)
    {
      dbg("lm4f I2C Only support 0,1,2,3\n");
      return NULL;
    }

#ifdef CONFIG_LMM4F_I2C0
  if (port == 0)
    {
      priv        = &g_i2c0dev;
      priv->base  = I2C1_MASTER_BASE;
      // ...
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
    }
  else
#endif
#ifdef CONFIG_LMM4F_I2C1
  if (port == 1)
    {
        priv        = &g_i2c1dev;
        priv->base  = I2C1_MASTER_BASE;
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
        GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
        GPIOPinConfigure(GPIO_PA6_I2C1SCL);
        GPIOPinConfigure(GPIO_PA7_I2C1SDA);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    }
  else
#endif
#ifdef CONFIG_LMM4F_I2C2
  if (port == 2)
    {
      priv        = &g_i2c0dev;
      priv->base  = I2C1_MASTER_BASE;
      // ...
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    }
  else
#endif
#ifdef CONFIG_LMM4F_I2C3
  if (port == 0)
    {
      priv        = &g_i2c0dev;
      priv->base  = I2C1_MASTER_BASE;
      // ...
      SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
    }
  else
#endif
    {
        return NULL;
    }

  /* Install our operations */
  priv->dev.ops = &lm4f_i2c_ops;
  return &priv->dev;
}

/*******************************************************************************
 * Name: up_i2cuninitalize
 *
 * Description:
 *   Uninitialise an I2C device
 *
 *******************************************************************************/

int up_i2cuninitialize(FAR struct i2c_dev_s * dev)
{
  //struct lm4f_i2cdev_s *priv = (struct lm4f_i2cdev_s *) dev;
  return OK;
}

#endif
