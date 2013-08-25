/****************************************************************************
 * include/nuttx/range_srf04.h
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

#ifndef __INCLUDE_NUTTX_RANGESRF04_H
#define __INCLUDE_NUTTX_RANGESRF04_H

/* For the purposes of this driver, a RANGESRF04 device is any device that generates
 * periodic output pulses s of controlled frequency and pulse width.  Such a
 * device might be used, for example, to perform pulse-width modulated output or
 * frequency/pulse-count modulated output (such as might be needed to control
 * a stepper motor).
 *
 * The RANGESRF04 driver is split into two parts:
 *
 * 1) An "upper half", generic driver that provides the comman RANGESRF04 interface
 *    to application level code, and
 * 2) A "lower half", platform-specific driver that implements the low-level
 *    timer controls to implement the RANGESRF04 functionality.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <fixedmath.h>

#include <nuttx/fs/ioctl.h>

#ifdef CONFIG_RANGE

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* IOCTL Commands ***********************************************************/

 /*
 * RANGESRF04IOC_SETPINCONFIG - 
 *
 * RANGESRF04IOC_GETPINCONFIG - Get the currently selected characteristics of
 *
 * RANGESRF04IOC_SETPOS - Start the pulsed output.  The RANGESRF04IOC_SETCHARACTERISTICS
 *   command must have previously been sent. If CONFIG_RANGESRF04_PULSECOUNT is
 *   defined and the pulse count was configured to a non-zero value, then
 *   this ioctl call will, by default, block until the programmed pulse count
 *   completes.  That default blocking behavior can be overridden by using
 *   the O_NONBLOCK flag when the RANGESRF04 driver is opened.
 *
 *   ioctl argument:  None
 */

#define RANGEIOC_ATTACHE    _RANGEIOC(1)
#define RANGEIOC_DETACH     _RANGEIOC(2)

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct range_info_s
{
  uint16_t       id;
  unsigned long  trig_port;
  unsigned char  trig_pin;      
  unsigned long  echo_port;
  unsigned char  echo_pin;
};

/* This structure is a set a callback functions used to call from the upper-
 * half, generic RANGESRF04 driver into lower-half, platform-specific logic that
 * supports the low-level timer outputs.
 */

struct range_lowerhalf_s;
struct range_ops_s
{
  CODE int (*setup)(FAR struct range_lowerhalf_s *dev);

  CODE int (*shutdown)(FAR struct range_lowerhalf_s *dev);

  CODE int (*attach)(FAR struct range_lowerhalf_s *dev,
                    FAR const struct range_info_s *info);
  CODE int (*detach)(FAR struct range_lowerhalf_s *dev,
                    FAR const struct range_info_s *info);
  
  CODE int (*read)(FAR struct range_lowerhalf_s *dev, 
                   FAR char* buff, size_t buflen);

  /* Lower-half logic may support platform-specific ioctl commands */
  CODE int (*ioctl)(FAR struct pwm_lowerhalf_s *dev,
                    int cmd, unsigned long arg);
};

/* This structure is the generic form of state structure used by lower half
 * timer driver.  This state structure is passed to the range driver when the
 * driver is initialized.  Then, on subsequent callbacks into the lower half
 * timer logic, this structure is provided so that the timer logic can
 * maintain state information.
 *
 * Normally that timer logic will have its own, custom state structure
 * that is simply cast to struct range_lowerhalf_s.  In order to perform such casts,
 * the initial fields of the custom state structure match the initial fields
 * of the following generic RANGESRF04 state structure.
 */

struct range_lowerhalf_s
{
  /* The first field of this state structure must be a pointer to the RANGESRF04
   * callback structure:
   */

  FAR const struct range_ops_s *ops;
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
 * "Upper-Half" RANGESRF04 Driver Interfaces
 ****************************************************************************/
/****************************************************************************
 * Name: range_register
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
 *     as "/dev/range0", "/dev/range1", etc.  where the driver path differs only
 *     in the "minor" number at the end of the device name.
 *   dev - A pointer to an instance of lower half timer driver.  This instance
 *     is bound to the RANGESRF04 driver and must persists as long as the driver
 *     persists.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

EXTERN int range_register(FAR const char *path, FAR struct range_lowerhalf_s *dev);

/****************************************************************************
 * Platform-Independent "Lower-Half" RANGESRF04 Driver Interfaces
 ****************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_RANGE */
#endif /* __INCLUDE_NUTTX_RANGESRF04_H */
