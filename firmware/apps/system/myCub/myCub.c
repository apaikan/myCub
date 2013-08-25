/****************************************************************************
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

/* Collects and reports system information.
 *
 * TODO: Gather information also from low-level devices, kernel/sched, clock,
 *   and further reporting as: sysinfo rtc, or sysinfo sched, ... with
 *   sysinfo help to report all of the options.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <sys/types.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>
#include <string.h>


#include <nuttx/clock.h>
#include <nuttx/arch.h>
#include <nuttx/servo.h>
#include <nuttx/sensors/range_srf04.h>

//#include "lm_gpio.h"

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


int servo_devinit(void);
int range_devinit(void);


unsigned int getRange(int id, const char* str)
{
    char dummy[32];
    sprintf(dummy, "(%d ", id);
    char* ptr = strstr(str, dummy);
    if(!ptr)
        return 0;
    ptr += strlen(dummy)-1; 
    char* ptr2 = strchr(ptr, ')');
    if(!ptr2)
        return 0;
    int i = 0;
    while(ptr++ != ptr2)
        dummy[i++] = *ptr;
    dummy[i-1] = NULL;        
    return atoi(dummy);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int myCub_main(int argc, char *argv[])
{
    int fd_range, fd_servo;
    int ret, i;
    struct servo_info_s srv_front;
    struct servo_info_s srv_back;
    struct servo_info_s srv_right;
    struct servo_info_s srv_left;

    //struct range_info_s range_front;
    struct range_info_s range_right;


    printf("-- myCub Demo --\n");  

    // initializing range finder sensors
    ret = range_devinit();
    if (ret != OK)
    {
        printf("myCub_main: range_devinit failed: %d\n", ret);
        return ERROR;
    }


    // Open the PWM device for reading
    fd_range = open("/dev/range0", O_RDONLY);
    if (fd_range < 0)
    {
        printf("myCub_main: range finder open failed: %d\n", errno);
        return ERROR;
    }

    range_right.id = 0;
    range_right.trig_port = GPIO_PORTE;
    range_right.trig_pin = GPIO_PIN_2; 
    range_right.echo_port = GPIO_PORTE;
    range_right.echo_pin = GPIO_PIN_1; 
    ioctl(fd_range, RANGEIOC_ATTACHE, (unsigned long)((uintptr_t)&range_right));


    ret = servo_devinit();
    if (ret != OK)
    {
        printf("myCub_main: servo_devinit failed: %d\n", ret);
        return ERROR;
    }

    // Open the PWM device for reading
    fd_servo = open("/dev/servo0", O_RDONLY);
    if (fd_servo < 0)
    {
        printf("myCub_main: open failed: %d\n", errno);
        return ERROR;
    }
 
    // Configure servo pins
    srv_front.id   = 0;
    srv_front.port = GPIO_PORTD;
    srv_front.pin = GPIO_PIN_0;

    srv_back.id   = 1;
    srv_back.port = GPIO_PORTD;
    srv_back.pin = GPIO_PIN_1;

    srv_right.id   = 2;
    srv_right.port = GPIO_PORTD;
    srv_right.pin = GPIO_PIN_2;

    srv_left.id   = 3;
    srv_left.port = GPIO_PORTD;
    srv_left.pin = GPIO_PIN_3;

    ioctl(fd_servo, SERVOIOC_SETPINCONFIG, (unsigned long)((uintptr_t)&srv_front));
    ioctl(fd_servo, SERVOIOC_SETPINCONFIG, (unsigned long)((uintptr_t)&srv_back));
    ioctl(fd_servo, SERVOIOC_SETPINCONFIG, (unsigned long)((uintptr_t)&srv_right));
    ioctl(fd_servo, SERVOIOC_SETPINCONFIG, (unsigned long)((uintptr_t)&srv_left));


    srv_right.pos = 8;
    srv_left.pos = 8;
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left));
 
    while(1)
    {
        char data[128];
        ret = read(fd_range, data, sizeof(data));
        if(ret > 0)
        {
            unsigned int dist = getRange(0, data);
            if(dist > 0 && dist < 300)
            {
                uint32_t value = (uint32_t) (8.0 + ((180.0 - 8.0) / (300.0 - 5.0)) * (300-dist - 5.0));
                //printf("dist:%d, value:%d\n", dist, value);
                srv_front.pos = value;
                srv_back.pos = value;
                ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
                ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));     
            }
        }
        usleep(1000);
    }


   /*    
    srv_front.pos = 8;
    srv_back.pos = 8;
    srv_right.pos = 8;
    srv_left.pos = 8;
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left));
           
    // all streching
    printf("Streching...\n");  
    for(i=8; i<160; i+=2)       
    {
        srv_front.pos = i;
        srv_back.pos = i;
        srv_right.pos = i;
        srv_left.pos = i;
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left)); 
        usleep(20000);
    }

    for(i=160; i>8; i-=2)
    {
        srv_front.pos = i;
        srv_back.pos = i;
        srv_right.pos = i;
        srv_left.pos = i;
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left)); 
        usleep(20000);
    }      

    srv_front.pos = 40;
    srv_back.pos = 40;
    srv_right.pos = 40;
    srv_left.pos = 40;
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left));

    sleep(2);
*/

    printf("done!\n");
    close(fd_servo);
    close(fd_range);

    return 0;
}

