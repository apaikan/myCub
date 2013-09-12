/****************************************************************************
 * drivers/adc_lm4f.c
 *
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
 * Compilation Switches
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/kmalloc.h>
#include <nuttx/fs/fs.h>
#include <nuttx/arch.h>
#include <nuttx/analog/adc_lm4f.h>

#include <arch/irq.h>

#ifdef CONFIG_ADCLM4F

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Debug ********************************************************************/
/* Non-standard debug that may be enabled just for testingRANGESRF04IOC_START RANGESRF04 */

#ifdef CONFIG_DEBUG_RANGE
#  define adclm4fdbg    dbg
#  define adclm4fvdbg   vdbg
#  define adclm4flldbg  lldbg
#  define adclm4fllvdbg llvdbg
#else
#  define adclm4fdbg(x...)
#  define adclm4fvdbg(x...)
#  define adclm4flldbg(x...)
#  define adclm4fllvdbg(x...)
#endif

/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

/* This structure describes the state of the upper half driver */

struct adclm4f_upperhalf_s
{
  uint8_t           crefs;    /* The number of times the device has been opened */
  volatile bool     started;  /* True: pulsed output is being generated */
  sem_t             exclsem;  /* Supports mutual exclusion */
  struct adclm4f_info_s info;     /* Pulsed output characteristics */
  FAR struct adclm4f_lowerhalf_s *dev;  /* lower-half state */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int     adclm4f_open(FAR struct file *filep);
static int     adclm4f_close(FAR struct file *filep);
static ssize_t adclm4f_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t adclm4f_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int     adclm4f_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_adclm4fops =
{
  adclm4f_open,  /* open */
  adclm4f_close, /* close */
  adclm4f_read,  /* read */
  adclm4f_write, /* write */
  0,         /* seek */
  adclm4f_ioctl  /* ioctl */
#ifndef CONFIG_DISABLE_POLL
  , 0        /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: adclm4f_open
 *
 * Description:
 *   This function is called whenever the RANGESRF04 device is opened.
 *
 ************************************************************************************/

static int adclm4f_open(FAR struct file *filep)
{
  FAR struct inode           *inode = filep->f_inode;
  FAR struct adclm4f_upperhalf_s *upper = inode->i_private;
  uint8_t                     tmp;
  int                         ret;

  adclm4fvdbg("crefs: %d\n", upper->crefs);

  /* Get exclusive access to the device structures */

  ret = sem_wait(&upper->exclsem);
  if (ret < 0)
    {
      ret = -errno;
      goto errout;
    }

  /* Increment the count of references to the device.  If this the first
   * time that the driver has been opened for this device, then initialize
   * the device.
   */

  tmp = upper->crefs + 1;
  if (tmp == 0)
    {
      /* More than 255 opens; uint8_t overflows to zero */

      ret = -EMFILE;
      goto errout_with_sem;
    }

  /* Check if this is the first time that the driver has been opened. */

  if (tmp == 1)
    {
      FAR struct adclm4f_lowerhalf_s *lower = upper->dev;

      DEBUGASSERT(lower->ops->setup != NULL);

      ret = lower->ops->setup(lower);
      if (ret < 0)
        {
          goto errout_with_sem;
        }
    }

  /* Save the new open count on success */

  upper->crefs = tmp;
  ret = OK;

errout_with_sem:
  sem_post(&upper->exclsem);
  
errout:
  return ret;
}

/************************************************************************************
 * Name: adclm4f_close
 *
 * Description:
 *   This function is called when the RANGESRF04 device is closed.
 *
 ************************************************************************************/

static int adclm4f_close(FAR struct file *filep)
{
  FAR struct inode           *inode = filep->f_inode;
  FAR struct adclm4f_upperhalf_s *upper = inode->i_private;
  int                         ret;

  adclm4fvdbg("crefs: %d\n", upper->crefs);

  /* Get exclusive access to the device structures */

  ret = sem_wait(&upper->exclsem);
  if (ret < 0)
    {
      ret = -errno;
      goto errout;
    }

  /* Decrement the references to the driver.  If the reference count will
   * decrement to 0, then uninitialize the driver.
   */

  if (upper->crefs > 1)
    {
      upper->crefs--;
    }
  else
    {
      FAR struct adclm4f_lowerhalf_s *lower = upper->dev;

      /* There are no more references to the port */

      upper->crefs = 0;

      /* Disable the RANGESRF04 device */

      DEBUGASSERT(lower->ops->shutdown != NULL);
      adclm4fvdbg("calling shutdown: %d\n");

      lower->ops->shutdown(lower);
    }
  ret = OK;

//errout_with_sem:
  sem_post(&upper->exclsem);
  
errout:
  return ret;
}

/************************************************************************************
 * Name: adclm4f_read
 *
 * Description:
 *
 ************************************************************************************/

static ssize_t adclm4f_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
  /* Return zero -- usually meaning end-of-file */
  FAR struct inode             *inode = filep->f_inode;
  FAR struct adclm4f_upperhalf_s *upper = inode->i_private;
  FAR struct adclm4f_lowerhalf_s *lower = upper->dev;
  int                          ret;

  ret = lower->ops->read(lower, buffer, buflen);           

  return ret;
    
}

/************************************************************************************
 * Name: adclm4f_write
 *
 * Description:
 *   A dummy write method.  This is provided only to satsify the VFS layer.
 *
 ************************************************************************************/

static ssize_t adclm4f_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
  return -ENOSYS;
}

/************************************************************************************
 * Name: adclm4f_ioctl
 *
 * Description:
 *   The standard ioctl method.  This is where ALL of the RANGESRF04 work is done.
 *   
 ************************************************************************************/

static int adclm4f_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
  FAR struct inode           *inode = filep->f_inode;
  FAR struct adclm4f_upperhalf_s *upper = inode->i_private;
  FAR struct adclm4f_lowerhalf_s *lower = upper->dev;
  int                         ret;

  adclm4fvdbg("cmd: %d arg: %ld\n", cmd, arg);

  /* Get exclusive access to the device structures */

  ret = sem_wait(&upper->exclsem);
  if (ret < 0)
    {
      return ret;
    }

  /* Handle built-in ioctl commands */

  switch (cmd)
    {
      case ADCIOC_SETCONF:
        {
          FAR const struct adclm4f_info_s *info = (FAR const struct adclm4f_info_s*)((uintptr_t)arg);
          DEBUGASSERT(info != NULL && lower->ops->attach != NULL);
          /* Save the pulse train characteristics */
          memcpy(&upper->info, info, sizeof(struct adclm4f_info_s));
          ret = lower->ops->configure(lower, &upper->info);           
        }
        break;

      /* Any unrecognized IOCTL commands might be platform-specific ioctl commands */

      default:
        {
          adclm4fvdbg("Forwarding unrecognized cmd: %d arg: %ld\n", cmd, arg);
          DEBUGASSERT(lower->ops->ioctl != NULL);
          ret = lower->ops->ioctl(lower, cmd, arg);
        }
        break;
    }

  sem_post(&upper->exclsem);
  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: adclm4f_register
 *
 * Description:
 *   This function binds an instance of a "lower half" timer driver with the
 *   "upper half" RANGESRF04 device and registers that device so that can be used
 *   by application code.
 *
 *   When this function is called, the "lower half" driver should be in the
 *   reset state (as if the shutdown() method had already been called).
 *
 * Input parameters:
 *   path - The full path to the driver to be registers in the NuttX pseudo-
 *     filesystem.  The recommended convention is to name all RANGESRF04 drivers
 *     as "/dev/adclm4f0", "/dev/adclm4f1", etc.  where the driver path differs only
 *     in the "minor" number at the end of the device name.
 *   dev - A pointer to an instance of lower half timer driver.  This instance
 *     is bound to the RANGESRF04 driver and must persists as long as the driver
 *     persists.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int adclm4f_register(FAR const char *path, FAR struct adclm4f_lowerhalf_s *dev)
{
  FAR struct adclm4f_upperhalf_s *upper;

  /* Allocate the upper-half data structure */

  upper = (FAR struct adclm4f_upperhalf_s *)kzalloc(sizeof(struct adclm4f_upperhalf_s));
  if (!upper)
    {
      adclm4fdbg("Allocation failed\n");
      return -ENOMEM;
    }

  /* Initialize the ADCLM4F device structure (it was already zeroed by kzalloc()) */
  sem_init(&upper->exclsem, 0, 1);
  upper->dev = dev;

  /* Register the RANGESRF04 device */

  adclm4fvdbg("Registering %s\n", path);
  return register_driver(path, &g_adclm4fops, 0666, upper);
}

#endif /* CONFIG_ADCLM4F*/

