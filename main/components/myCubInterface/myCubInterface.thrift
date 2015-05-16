# Copyright: (C) 2013 iCub Facility - Istituto Italiano di Tecnologia
# Authors: Ali Paikan
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
#
# myCubInterface.thrift

/**
* myCubInterface_IDL
*
* Interface. 
*/

service myCubInterface_IDL
{


  /**
  * ping the control board
  * @return true/false on success/failure
  */
  bool ping();
  
  /**
  * move all joints to the home position
  * @return true/false on success/failure
  */
  bool homeAll();

  /**
  * start motor controller 
  * @return true/false on success/failure
  */
  bool startController();

  /**
  * stop motor controller 
  * @return true/false on success/failure
  */
  bool stopController();

  /**
  * Set joint pos
  * @joint joint id (0..3)
  * @pos   joint pos (0..90)
  * @return true/false on success/failure
  */
  bool setPose(1:i32 joint, 2:i32 pos);

  /**
  * get joint pos
  * @joint joint id (0..3)
  * @return joint pos
  */
  i32 getPose(1:i32 joint);

  /**
  * Move a joint to position with time duration
  * @joint joint id (0..3)
  * @pos   joint pos (0..90)
  * @t     time duration in ms
  * @return true/false on success/failure
  */
  bool gotoPose(1:i32 joint, 2:i32 pos, 3:i32 t=100);

  /**
  * Move a joint to position with time duration
  * and wait until it reaches the desired position
  * @joint joint id (0..3)
  * @pos   joint pos (0..90)
  * @t     time duration in ms
  * @return true/false on success/failure
  */
  bool gotoPoseSync(1:i32 joint, 2:i32 pos, 3:i32 t=100);

  /**
  * Set all joint pos
  * @poses a vector of joint poses (must have 4 elements)
  * @return true/false on success/failure
  */
  bool setPoseAll(1:list<i32> poses);

  /**
  * get all joint poses
  * @return joint poses
  */
  list<i32> getPoseAll();

  /**
  * move all joint pos with desired duration
  * @poses a vector of joint poses (must have 4 elements)
  * @times a vector of time duration (must have 4 elements)
  * @return true/false on success/failure
  */
  bool gotoPoseAll(1:list<i32> poses, 2:list<i32> times);

  /**
  * get range-finder value
  * @id the sensor id started from 0
  * @return true/false on success/failure
  */
  i32 getDistance(1:i32 id=0);

  /**
  * get digital compass values
  * @returna a vector of x,y,z
  */
  list<i32> getOrientation();

  /**
  * get absolute heading 
  * @return heading angle
  */
  i32 getHeading();

  /**
  * get battery voltage status
  * @return true/false on success/failure
  */
  i32 getBatteryVolt();

  /**
  * get battery current status
  * @return true/false on success/failure
  */
  i32 getBatteryCurrent();

  /**
  * get motion status
  * @return 1 if motion is detected otherwise 0
  */
  i32 getMotion();

  /**
  * get all important status
  * @returna a vector of j0, j1, j2, j3, dist, heading, volt
  */
  list<i32> getAll();

}
