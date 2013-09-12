/****************************************************************************
 * apps/nshlib/nsh.h
 *
 *   Copyright (C) 2007-2013 Gregory Nutt. All rights reserved.
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
 ****************************************************************************/

#ifndef __APPS_MYCUB_INTERFACE_H
#define __APPS_MYCUB_INTERFACE_H


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <sys/ioctl.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>


#include <nuttx/arch.h>
#include <nuttx/servo.h>
#include <nuttx/sensors/range_srf04.h>
#include <nuttx/analog/adc_lm4f.h>

/* This identifies the GPIO port
 * .... .... .... .... .... .... .PPP P...
 */

#define GPIO_PORT_SHIFT               3                          /* Bit 3-6:  Port number */
#define GPIO_PORT_MASK                (15 << GPIO_PORT_SHIFT)
#  define GPIO_PORTA                  (0 << GPIO_PORT_SHIFT)     /*   GPIOA */
#  define GPIO_PORTB                  (1 << GPIO_PORT_SHIFT)     /*   GPIOB */
#  define GPIO_PORTC                  (2 << GPIO_PORT_SHIFT)     /*   GPIOC */
#  define GPIO_PORTD                  (3 << GPIO_PORT_SHIFT)     /*   GPIOD */
#  define GPIO_PORTE                  (4 << GPIO_PORT_SHIFT)     /*   GPIOE */
#  define GPIO_PORTF                  (5 << GPIO_PORT_SHIFT)     /*   GPIOF */
#  define GPIO_PORTG                  (6 << GPIO_PORT_SHIFT)     /*   GPIOG */
#  define GPIO_PORTH                  (7 << GPIO_PORT_SHIFT)     /*   GPIOH */
#  define GPIO_PORTJ                  (8 << GPIO_PORT_SHIFT)     /*   GPIOJ */

/* This identifies the pin number in the port:
 * .... .... .... .... .... .... .... .BBB
 */

#define GPIO_PIN_SHIFT                 0                           /* Bits 0-2: GPIO pin: 0-7 */
#define GPIO_PIN_MASK                 (7 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_0                  (0 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_1                  (1 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_2                  (2 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_3                  (3 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_4                  (4 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_5                  (5 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_6                  (6 << GPIO_PIN_SHIFT)
#  define GPIO_PIN_7                  (7 << GPIO_PIN_SHIFT)



/*
 * Joint index mapping
 */

#define FRONT_JOINT     0
#define RIGHT_JOINT     1
#define BACK_JOINT      2
#define LEFT_JOINT      3


#define FRONT_RANGE     0
#define RIGHT_RANGE     1


typedef struct __JointContext {
    int joint_0;
    int joint_1;
    int joint_2;
    int joint_3;
} JointContext;


class MyCubInterface {

public:
    
    MyCubInterface(void);
    ~MyCubInterface();

    bool init(void);
    void fini(void);

    // joint position and speed controller 
    bool setPose(const unsigned int joint, const int pos);
    bool gotoPose(const unsigned int joint, const int pos, const double t=0.0);
    bool gotoPoseSync(const unsigned int joint, const int pos, const double t=0.0);
    int getPose(const unsigned int joint);
    bool checkMotionDone(const unsigned int joint);
    bool checkMotionDone(void);
    JointContext getContext(void);
    void setContext(JointContext &ctx);

    // ultrasonic range finder 
    int getDistance(const int id);

private:
    // servo controller driver  variables 
    int fd_servo;
    struct servo_info_s servo_drv[4];
    int joints_pos[4];      // joint positions

    // ultrasonic range finder driver variables 
    int fd_range;
    struct range_info_s range_drv[2];

    // adc 
    int fd_adc;

private:
    int getRange(int id, const char* str);
    int getRawAnalogData(const int channel, unsigned long freq,
                         uint16_t* data, size_t len);
};

#endif /* __APPS_MYCUB_INTERFACE_H */
