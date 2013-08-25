/****************************************************************************
 * include/nuttx/servo.h
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

#ifndef __INCLUDE_NUTTX_SERVO_H
#define __INCLUDE_NUTTX_SERVO_H

/* For the purposes of this driver, a SERVO device is any device that generates
 * periodic output pulses s of controlled frequency and pulse width.  Such a
 * device might be used, for example, to perform pulse-width modulated output or
 * frequency/pulse-count modulated output (such as might be needed to control
 * a stepper motor).
 *
 * The SERVO driver is split into two parts:
 *
 * 1) An "upper half", generic driver that provides the comman SERVO interface
 *    to application level code, and
 * 2) A "lower half", platform-specific driver that implements the low-level
 *    timer controls to implement the SERVO functionality.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <fixedmath.h>

#include <nuttx/fs/ioctl.h>

#ifdef CONFIG_SERVO

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* IOCTL Commands ***********************************************************/

 /*
 * SERVOIOC_SETPINCONFIG - 
 *
 * SERVOIOC_GETPINCONFIG - Get the currently selected characteristics of
 *
 * SERVOIOC_SETPOS - Start the pulsed output.  The SERVOIOC_SETCHARACTERISTICS
 *   command must have previously been sent. If CONFIG_SERVO_PULSECOUNT is
 *   defined and the pulse count was configured to a non-zero value, then
 *   this ioctl call will, by default, block until the programmed pulse count
 *   completes.  That default blocking behavior can be overridden by using
 *   the O_NONBLOCK flag when the SERVO driver is opened.
 *
 *   ioctl argument:  None
 */

#define SERVOIOC_SETPINCONFIG   _SERVOIOC(1)
#define SERVOIOC_GETPINCONFIG   _SERVOIOC(2)
#define SERVOIOC_SETPOS         _SERVOIOC(3)

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct servo_info_s
{
  uint16_t       id;
  unsigned long  port;
  unsigned char  pin;      
  uint16_t       pos;        /* servo postion */
};

/* This structure is a set a callback functions used to call from the upper-
 * half, generic SERVO driver into lower-half, platform-specific logic that
 * supports the low-level timer outputs.
 */

struct servo_lowerhalf_s;
struct servo_ops_s
{
  CODE int (*setup)(FAR struct servo_lowerhalf_s *dev);

  CODE int (*shutdown)(FAR struct servo_lowerhalf_s *dev);

  CODE int (*attach)(FAR struct servo_lowerhalf_s *dev,
                    FAR const struct servo_info_s *info);

  CODE int (*setpos)(FAR struct servo_lowerhalf_s *dev,
                    FAR const struct servo_info_s *info);

  /* Lower-half logic may support platform-specific ioctl commands */
  CODE int (*ioctl)(FAR struct pwm_lowerhalf_s *dev,
                    int cmd, unsigned long arg);
};

/* This structure is the generic form of state structure used by lower half
 * timer driver.  This state structure is passed to the servo driver when the
 * driver is initialized.  Then, on subsequent callbacks into the lower half
 * timer logic, this structure is provided so that the timer logic can
 * maintain state information.
 *
 * Normally that timer logic will have its own, custom state structure
 * that is simply cast to struct servo_lowerhalf_s.  In order to perform such casts,
 * the initial fields of the custom state structure match the initial fields
 * of the following generic SERVO state structure.
 */

struct servo_lowerhalf_s
{
  /* The first field of this state structure must be a pointer to the SERVO
   * callback structure:
   */

  FAR const struct servo_ops_s *ops;

  /* The custom timer state structure may include additional fields after
   * the pointer to the SERVO callback structure.
   */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * "Upper-Half" SERVO Driver Interfaces
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

EXTERN int servo_register(FAR const char *path, FAR struct servo_lowerhalf_s *dev);

/****************************************************************************
 * Platform-Independent "Lower-Half" SERVO Driver Interfaces
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_SERVO */
#endif /* __INCLUDE_NUTTX_SERVO_H */
