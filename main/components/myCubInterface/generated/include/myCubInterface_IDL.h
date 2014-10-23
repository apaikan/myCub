// This is an automatically-generated file.
// It could get re-generated if the ALLOW_IDL_GENERATION flag is on.

#ifndef YARP_THRIFT_GENERATOR_myCubInterface_IDL
#define YARP_THRIFT_GENERATOR_myCubInterface_IDL

#include <yarp/os/Wire.h>
#include <yarp/os/idl/WireTypes.h>

class myCubInterface_IDL;


/**
 * myCubInterface_IDL
 * Interface.
 */
class myCubInterface_IDL : public yarp::os::Wire {
public:
  myCubInterface_IDL() { yarp().setOwner(*this); }
/**
 * ping the control board
 * @return true/false on success/failure
 */
  virtual bool ping();
/**
 * move all joints to the home position
 * @return true/false on success/failure
 */
  virtual bool homeAll();
/**
 * start motor controller
 * @return true/false on success/failure
 */
  virtual bool startController();
/**
 * stop motor controller
 * @return true/false on success/failure
 */
  virtual bool stopController();
/**
 * Set joint pos
 * @joint joint id (0..3)
 * @pos   joint pos (0..90)
 * @return true/false on success/failure
 */
  virtual bool setPose(const int32_t joint, const int32_t pos);
/**
 * get joint pos
 * @joint joint id (0..3)
 * @return joint pos
 */
  virtual int32_t getPose(const int32_t joint);
/**
 * Move a joint to position with time duration
 * @joint joint id (0..3)
 * @pos   joint pos (0..90)
 * @t     time duration in ms
 * @return true/false on success/failure
 */
  virtual bool gotoPose(const int32_t joint, const int32_t pos, const int32_t t = 100);
/**
 * Move a joint to position with time duration
 * and wait until it reaches the desired position
 * @joint joint id (0..3)
 * @pos   joint pos (0..90)
 * @t     time duration in ms
 * @return true/false on success/failure
 */
  virtual bool gotoPoseSync(const int32_t joint, const int32_t pos, const int32_t t = 100);
/**
 * Set all joint pos
 * @poses a vector of joint poses (must have 4 elements)
 * @return true/false on success/failure
 */
  virtual bool setPoseAll(const std::vector<int32_t> & poses);
/**
 * get all joint poses
 * @return joint poses
 */
  virtual std::vector<int32_t>  getPoseAll();
/**
 * move all joint pos with desired duration
 * @poses a vector of joint poses (must have 4 elements)
 * @times a vector of time duration (must have 4 elements)
 * @return true/false on success/failure
 */
  virtual bool gotoPoseAll(const std::vector<int32_t> & poses, const std::vector<int32_t> & times);
/**
 * get range-finder value
 * @id the sensor id started from 0
 * @return true/false on success/failure
 */
  virtual int32_t getDistance(const int32_t id = 0);
/**
 * get digital compass values
 * @returna a vector of x,y,z
 */
  virtual std::vector<int32_t>  getOrientation();
/**
 * get absolute heading
 * @return heading angle
 */
  virtual int32_t getHeading();
/**
 * get battery voltage status
 * @return true/false on success/failure
 */
  virtual int32_t getBatteryVolt();
/**
 * get battery current status
 * @return true/false on success/failure
 */
  virtual int32_t getBatteryCurrent();
  virtual bool read(yarp::os::ConnectionReader& connection);
  virtual std::vector<std::string> help(const std::string& functionName="--all");
};

#endif

