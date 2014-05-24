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

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "mycub_compass.h"

MyCubCompass::MyCubCompass()
{
    hmc_dev = 0;
    _hmc5883_Gauss_LSB_XY = 1100.0F;  // Varies with gain
    _hmc5883_Gauss_LSB_Z  = 980.0F;   // Varies with gain
}

bool MyCubCompass::init(int i2c_port) 
{
    hmc_dev = up_i2cinitialize(i2c_port);
    // set the frequency to 100
    I2C_SETFREQUENCY(hmc_dev, 100);

    // Enable the magnetometer
    uint8_t val = 0x00;
    I2C_SETADDRESS(hmc_dev, HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_MR_REG_M);
    I2C_WRITE(hmc_dev, &val, 1);
    
    // Set the gain to a known level
    setGain(HMC5883_MAGGAIN_1_3);

    return true;
}

bool MyCubCompass::setGain(hmc5883MagGain gain) 
{
    I2C_SETADDRESS(hmc_dev, HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_CRB_REG_M);
    I2C_WRITE(hmc_dev, (uint8_t*)(&gain), 1); 

    _magGain = gain;

    switch(gain)
    {
    case HMC5883_MAGGAIN_1_3:
      _hmc5883_Gauss_LSB_XY = 1100;
      _hmc5883_Gauss_LSB_Z  = 980;
      break;
    case HMC5883_MAGGAIN_1_9:
      _hmc5883_Gauss_LSB_XY = 855;
      _hmc5883_Gauss_LSB_Z  = 760;
      break;
    case HMC5883_MAGGAIN_2_5:
      _hmc5883_Gauss_LSB_XY = 670;
      _hmc5883_Gauss_LSB_Z  = 600;
      break;
    case HMC5883_MAGGAIN_4_0:
      _hmc5883_Gauss_LSB_XY = 450;
      _hmc5883_Gauss_LSB_Z  = 400;
      break;
    case HMC5883_MAGGAIN_4_7:
      _hmc5883_Gauss_LSB_XY = 400;
      _hmc5883_Gauss_LSB_Z  = 255;
      break;
    case HMC5883_MAGGAIN_5_6:
      _hmc5883_Gauss_LSB_XY = 330;
      _hmc5883_Gauss_LSB_Z  = 295;
      break;
    case HMC5883_MAGGAIN_8_1:
      _hmc5883_Gauss_LSB_XY = 230;
      _hmc5883_Gauss_LSB_Z  = 205;
      break;
    }
    return true;
}

bool MyCubCompass::read()
{
    if(hmc_dev == 0 )
        return false;

    uint8_t buf[6];
    I2C_SETADDRESS(hmc_dev, HMC5883_ADDRESS_MAG, HMC5883_REGISTER_MAG_OUT_X_H_M);
    I2C_READ(hmc_dev, buf, 6);

    uint8_t xhi = buf[0];
    uint8_t xlo = buf[1];
    uint8_t zhi = buf[2];
    uint8_t zlo = buf[3];
    uint8_t yhi = buf[4];
    uint8_t ylo = buf[5];

    // Shift values to create properly formed integer (low byte first)
    _magData.x = (int16_t)(xlo | ((int16_t)xhi << 8));
    _magData.y = (int16_t)(ylo | ((int16_t)yhi << 8));
    _magData.z = (int16_t)(zlo | ((int16_t)zhi << 8));

    //printf("%f, %f, %f\n", _magData.x, _magData.y, _magData.z);
    return true;
}

bool MyCubCompass::getEvent(sensors_event_t& event)
{
    if(hmc_dev == 0 )
        return false;

    /* Clear the event */
    memset(&event, 0, sizeof(sensors_event_t));

    /* Read new data */
    MyCubCompass::read();
    event.magnetic.x = _magData.x / _hmc5883_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event.magnetic.y = _magData.y / _hmc5883_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
    event.magnetic.z = _magData.z / _hmc5883_Gauss_LSB_Z * SENSORS_GAUSS_TO_MICROTESLA;
    return true;
}

float MyCubCompass::getHeading(void) 
{
    if(hmc_dev == 0 )
        return -1.0;

    sensors_event_t event;
    getEvent(event);

    //printf("%d %d %d\n", event.magnetic.x, event.magnetic.y, event.magnetic.z); 

    // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
    // Calculate heading when the magnetometer is level, then correct for signs of axis.
    float heading = atan2(event.magnetic.y, event.magnetic.x);

    // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
    // Find yours here: http://www.magnetic-declination.com/
    // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
    // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
    // Genova : 53* 2'
    heading += 0.92;

    // Correct for when signs are reversed.
    if(heading < 0)
        heading += 2*M_PI;

    // Check for wrap due to addition of declination.
    if(heading > 2*M_PI)
        heading -= 2*M_PI;

    // Convert radians to degrees for readability.
    return (heading * 180/M_PI);
}


