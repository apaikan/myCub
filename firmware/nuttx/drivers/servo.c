/****************************************************************************
 * drivers/servo.c
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
#include <nuttx/servo.h>

#include <arch/irq.h>

#ifdef CONFIG_SERVO

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/* Debug ********************************************************************/
/* Non-standard debug that may be enabled just for testingSERVOIOC_START SERVO */

#ifdef CONFIG_DEBUG_SERVO
#  define servodbg    dbg
#  define servovdbg   vdbg
#  define servolldbg  lldbg
#  define servollvdbg llvdbg
#else
#  define servodbg(x...)
#  define servovdbg(x...)
#  define servolldbg(x...)
#  define servollvdbg(x...)
#endif

/****************************************************************************
 * Private Type Definitions
 ****************************************************************************/

/* This structure describes the state of the upper half driver */

struct servo_upperhalf_s
{
  uint8_t           crefs;    /* The number of times the device has been opened */
  volatile bool     started;  /* True: pulsed output is being generated */
  sem_t             exclsem;  /* Supports mutual exclusion */
  struct servo_info_s info;     /* Pulsed output characteristics */
  FAR struct servo_lowerhalf_s *dev;  /* lower-half state */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int     servo_open(FAR struct file *filep);
static int     servo_close(FAR struct file *filep);
static ssize_t servo_read(FAR struct file *filep, FAR char *buffer, size_t buflen);
static ssize_t servo_write(FAR struct file *filep, FAR const char *buffer, size_t buflen);
static int     servo_ioctl(FAR struct file *filep, int cmd, unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct file_operations g_servoops =
{
  servo_open,  /* open */
  servo_close, /* close */
  servo_read,  /* read */
  servo_write, /* write */
  0,         /* seek */
  servo_ioctl  /* ioctl */
#ifndef CONFIG_DISABLE_POLL
  , 0        /* poll */
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/************************************************************************************
 * Name: servo_open
 *
 * Description:
 *   This function is called whenever the SERVO device is opened.
 *
 ************************************************************************************/

static int servo_open(FAR struct file *filep)
{
  FAR struct inode           *inode = filep->f_inode;
  FAR struct servo_upperhalf_s *upper = inode->i_private;
  uint8_t                     tmp;
  int                         ret;

  servovdbg("crefs: %d\n", upper->crefs);

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
      FAR struct servo_lowerhalf_s *lower = upper->dev;

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
 * Name: servo_close
 *
 * Description:
 *   This function is called when the SERVO device is closed.
 *
 ************************************************************************************/

static int servo_close(FAR struct file *filep)
{
  FAR struct inode           *inode = filep->f_inode;
  FAR struct servo_upperhalf_s *upper = inode->i_private;
  int                         ret;

  servovdbg("crefs: %d\n", upper->crefs);

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
      FAR struct servo_lowerhalf_s *lower = upper->dev;

      /* There are no more references to the port */

      upper->crefs = 0;

      /* Disable the SERVO device */

      DEBUGASSERT(lower->ops->shutdown != NULL);
      servovdbg("calling shutdown: %d\n");

      lower->ops->shutdown(lower);
    }
  ret = OK;

//errout_with_sem:
  sem_post(&upper->exclsem);
  
errout:
  return ret;
}

/************************************************************************************
 * Name: servo_read
 *
 * Description:
 *   A dummy read method.  This is provided only to satsify the VFS layer.
 *
 ************************************************************************************/

static ssize_t servo_read(FAR struct file *filep, FAR char *buffer, size_t buflen)
{
  /* Return zero -- usually meaning end-of-file */

  return 0;
}

/************************************************************************************
 * Name: servo_write
 *
 * Description:
 *   A dummy write method.  This is provided only to satsify the VFS layer.
 *
 ************************************************************************************/

static ssize_t servo_write(FAR struct file *filep, FAR const char *buffer, size_t buflen)
{
  return 0;
}

/************************************************************************************
 * Name: servo_ioctl
 *
 * Description:
 *   The standard ioctl method.  This is where ALL of the SERVO work is done.
 *   
 ************************************************************************************/

static int servo_ioctl(FAR struct file *filep, int cmd, unsigned long arg)
{
  FAR struct inode           *inode = filep->f_inode;
  FAR struct servo_upperhalf_s *upper = inode->i_private;
  FAR struct servo_lowerhalf_s *lower = upper->dev;
  int                         ret;

  servovdbg("cmd: %d arg: %ld\n", cmd, arg);

  /* Get exclusive access to the device structures */

  ret = sem_wait(&upper->exclsem);
  if (ret < 0)
    {
      return ret;
    }

  /* Handle built-in ioctl commands */

  switch (cmd)
    {
      case SERVOIOC_SETPINCONFIG:
        {
          FAR const struct servo_info_s *info = (FAR const struct servo_info_s*)((uintptr_t)arg);
          DEBUGASSERT(info != NULL && lower->ops->attach != NULL);
          /* Save the pulse train characteristics */
          memcpy(&upper->info, info, sizeof(struct servo_info_s));
          ret = lower->ops->attach(lower, &upper->info);           
        }
        break;

       case SERVOIOC_GETPINCONFIG:
        {
          FAR struct servo_info_s *info = (FAR struct servo_info_s*)((uintptr_t)arg);
          DEBUGASSERT(info != NULL);
          memcpy(info, &upper->info, sizeof(struct servo_info_s));
        }
        break;

      case SERVOIOC_SETPOS:
        {
          FAR const struct servo_info_s *info = (FAR const struct servo_info_s*)((uintptr_t)arg);
          memcpy(&upper->info, info, sizeof(struct servo_info_s));
          DEBUGASSERT(info != NULL && lower->ops->setpos != NULL);
          ret = lower->ops->setpos(lower, &upper->info); 
        }
        break;

      /* Any unrecognized IOCTL commands might be platform-specific ioctl commands */

      default:
        {
          servovdbg("Forwarding unrecognized cmd: %d arg: %ld\n", cmd, arg);
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
 * Name: servo_register
 *
 * Description:
 *   This function binds an instance of a "lower half" timer driver with the
 *   "upper half" SERVO device and registers that device so that can be used
 *   by application code.
 *
 *   When this function is called, the "lower half" driver should be in the
 *   reset state (as if the shutdown() method had already been called).
 *
 * Input parameters:
 *   path - The full path to the driver to be registers in the NuttX pseudo-
 *     filesystem.  The recommended convention is to name all SERVO drivers
 *     as "/dev/servo0", "/dev/servo1", etc.  where the driver path differs only
 *     in the "minor" number at the end of the device name.
 *   dev - A pointer to an instance of lower half timer driver.  This instance
 *     is bound to the SERVO driver and must persists as long as the driver
 *     persists.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int servo_register(FAR const char *path, FAR struct servo_lowerhalf_s *dev)
{
  FAR struct servo_upperhalf_s *upper;

  /* Allocate the upper-half data structure */

  upper = (FAR struct servo_upperhalf_s *)kzalloc(sizeof(struct servo_upperhalf_s));
  if (!upper)
    {
      servodbg("Allocation failed\n");
      return -ENOMEM;
    }

  /* Initialize the SERVO device structure (it was already zeroed by kzalloc()) */

  sem_init(&upper->exclsem, 0, 1);
#ifdef CONFIG_SERVO_PULSECOUNT
  sem_init(&upper->waitsem, 0, 0);
#endif
  upper->dev = dev;

  /* Register the SERVO device */

  servovdbg("Registering %s\n", path);
  return register_driver(path, &g_servoops, 0666, upper);
}

#endif /* CONFIG_SERVO */

