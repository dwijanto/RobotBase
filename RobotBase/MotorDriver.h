#ifndef MOTORDRIVER_H_INCLUDED
#define MOTORDRIVER_H_INCLUDED

namespace dwijanto{
class MotorDriver{
public:
      /*
      *@brief Change the speed of the motor.
      *@param speed, the new speed of the motor.
      * valid value are between -255 and 255
      * use positive values to run the motor forward.
      * negative values to run it backward and zeor to stop the motor.
      */
  virtual void setSpeed(int speed) = 0;
  virtual int getSpeed() const = 0;
};
};

#endif // MOTORDRIVER_H_INCLUDED
