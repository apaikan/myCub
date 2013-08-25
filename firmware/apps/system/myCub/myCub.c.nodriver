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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <nuttx/config.h>
#include <nuttx/version.h>
#include <arch/board/board.h>
#include <arch/board/board.h>
#include <arch/board/servo.h>
#include <arch/board/inc/hw_types.h>
#include <arch/board/inc/hw_memmap.h>
#include <arch/board/driverlib/pin_map.h>
#include <arch/board/driverlib/gpio.h>
#include <arch/board/driverlib/sysctl.h>
//#include <arch/board/driverlib/rom.h>
//#include <arch/board/driverlib/rom_map.h>


/****************************************************************************
 * Public Functions
 ****************************************************************************/

servo_t *servo_front;
servo_t *servo_back;
servo_t *servo_right;
servo_t *servo_left;

void setupServos(void) {

    // PD0, PD1, PD2, PD3
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // Zero out configs
    servoInit();

    // Bind pads to servo configs
    servo_front = servoAttach(GPIO_PORTD_BASE, GPIO_PIN_0);
    servo_back = servoAttach(GPIO_PORTD_BASE, GPIO_PIN_1);
    servo_right = servoAttach(GPIO_PORTD_BASE, GPIO_PIN_2);
    servo_left = servoAttach(GPIO_PORTD_BASE, GPIO_PIN_3);

    // Start the servo timers
    servoStart();
}


int myCub_main(int argc, char *argv[])
{
    printf("myCub Demo\n");  

    // Setup servos and start the timer for them
    setupServos();

    // Continually check which button is pressed and move the servo position that direction
   // while(1) {

        int i; 
        int rep;

        servoSet(servo_front, 8);
        servoSet(servo_back, 8);           
        servoSet(servo_right, 8);           
        servoSet(servo_left, 8);           
                
        // all streching
        printf("Streching...\n");  
        for(i=8; i<160; i+=2)       
        {
            servoSet(servo_front, i);
            servoSet(servo_back, i);           
            servoSet(servo_right, i);           
            servoSet(servo_left, i); 
            usleep(20000);
        }

        for(i=160; i>8; i-=2)
        {
            servoSet(servo_front, i);
            servoSet(servo_back, i);           
            servoSet(servo_right, i);           
            servoSet(servo_left, i); 
            usleep(20000);
        }      
        
        
        // moving wings
        printf("Flying...\n");  
        servoSet(servo_front, 8);
        servoSet(servo_back, 8);
        servoSet(servo_right, 160);
        servoSet(servo_left, 160);

        for(rep=0; rep<3; rep++)
        {
            for(i=160; i>80; i-=1)
            {
                servoSet(servo_right, i);           
                servoSet(servo_left, i); 
                usleep(5000);
            }

            for(i=80; i<160; i+=1)
            {
                servoSet(servo_right, i);           
                servoSet(servo_left, i);                
                usleep(5000);
            }
        }
        

        // walking front
        printf("Walking forward...\n");  
        servoSet(servo_back, 140);
        servoSet(servo_right, 160);
        servoSet(servo_left, 160);

        for(rep=0; rep<10; rep++)
        {
            for(i=80; i>8; i-=2)
            {
                servoSet(servo_front, i);
                usleep(20000);
            }
        }

        servoSet(servo_front, 8);
        servoSet(servo_back, 8);           
        servoSet(servo_right, 8);           
        servoSet(servo_left, 8);           
        sleep(1);
        printf("-- done! --\n"); 

    //} // end main loop

  return 0;
}

