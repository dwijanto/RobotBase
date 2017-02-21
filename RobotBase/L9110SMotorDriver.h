#ifndef L9110SMOTORDRIVER_H_INCLUDED
#define L9110SMOTORDRIVER_H_INCLUDED

#include "MotorDriver.h"

/**

Author : Lie Dwi Janto Johan
Date : 2015-May-9
1st motor
#define AIA 3 -> _pin1
#define AIB 5 -> _pin2

2nd motor
#define BIA 6 -> _pin1
#define BIB 9 -> _pin2
*/

namespace dwijanto{
class Motor:public MotorDriver{
private:
    int currentSpeed;
    int _pin1,_pin2;
    enum mStatus{_Forward,_Backward} _mStatus;
 public:

    Motor(int Pin1,int Pin2):MotorDriver(),_pin1(Pin1),_pin2(Pin2){
        pinMode(_pin1,OUTPUT);
        pinMode(_pin2,OUTPUT);
        currentSpeed = 0;
    }
    void run(mStatus status){
        if (status == _Forward){
            analogWrite(_pin1,currentSpeed);
            analogWrite(_pin2,0);
        }else if(status == _Backward){
            analogWrite(_pin1,0);
            analogWrite(_pin2,currentSpeed * -1);
        }
    }
    void setSpeed(int speed){
        currentSpeed = speed;
        if (currentSpeed >=0){
            //Serial.print("Forward");
            run(_Forward);

        }else {

            run(_Backward);
        }
    }
    int getSpeed()const{
        return currentSpeed;
    }


};
}
#endif // L9110SMOTORDRIVER_H_INCLUDED
