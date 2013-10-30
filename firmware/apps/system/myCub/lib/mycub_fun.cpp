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
#include <mycub_fun.h>
#include <nuttx_thread.h>

extern "C"
{

NxThread dancer1, dancer2;


/* dancer threads */
void dancer_body(void *pParams) 
{
    printf("...dancing...\n");
    /*
    static bool body_flag = false;
    MyCubInterface* mycub = (MyCubInterface*) pParams;

    if(body_flag)
    {
        mycub->gotoPose(FRONT_JOINT, 60, 0.5);
        mycub->gotoPose(BACK_JOINT, 8, 0.5);
    }        
    else
    {
        mycub->gotoPose(FRONT_JOINT, 8, 0.5);
        mycub->gotoPose(BACK_JOINT, 60, 0.5);        
    } 
    */
}   

void dancer_wing(void *pParams) 
{
    //walk_param_t* params = (walk_param_t *) pParams;

}   


} // end of extern "C"


void MyCubFun::stop(void)
{
    dancer1.stop();
    //dancer2.stop();
}

bool MyCubFun::dance(MyCubInterface& mycub)
{
    //MyCubFun::stop();

    mycub.gotoPose(FRONT_JOINT, 60, 0.5);
    mycub.gotoPose(BACK_JOINT, 60, 0.5);
    mycub.gotoPose(RIGHT_JOINT, 150, 0.5);
    mycub.gotoPose(LEFT_JOINT, 150, 0.5);

    dancer1.setCallback(dancer_body, &mycub);
    dancer1.setPeriod(1000000);
    dancer1.start();

    return true;
}


