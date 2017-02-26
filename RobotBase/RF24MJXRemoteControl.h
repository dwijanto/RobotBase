
#ifndef RF24MJXREMOTECONTROL_H_INCLUDED
#define RF24MJXREMOTECONTROL_H_INCLUDED

#include "RemoteControlDriver.h"

namespace dwijanto
{
class RF24MJXRemoteControl:public RemoteControlDriver
{
private:
    unsigned long lastRecvTime;
    command_t::key_t lastkey;
    unsigned long time;
    unsigned long newTime;
    rx_values_t rxValues;
    bool bind_in_progress;

    void setPPMValuesFromData()
    {
        time = micros();
        ppm[0] = rxValues.throttle;
        ppm[1] = map(rxValues.yaw, -114, 114, 1000, 2000);// rxValues.yaw; // map(rxValues.yaw, -114, 114, 1000, 2000); // Mapping for servo, min 600 ppm max 2500 ppm
        ppm[2] = map(rxValues.pitch,    -114, 114, 1000, 2000);
        ppm[3] = map(rxValues.roll,     -114, 114, 1000, 2000);
        ppm[4] = rxValues.flags ==0?1:-1;
        ppm[5] = rxValues.trim_yaw;
        ppm[6] = rxValues.trim_pitch;
        ppm[7] = rxValues.trim_roll;
        ppm[8] = rxValues.crc;

        Serial.print("Throttle (Speed) : ");
        Serial.print(ppm[0]);
        Serial.print(" Yaw : ");
        Serial.print(ppm[1]);
        Serial.print(" Pitch : ");
        Serial.print(ppm[2]);
        Serial.print(" Roll : ");
        Serial.print(ppm[3]);
        Serial.print(" Flag : ");
        Serial.print(ppm[4]);
        Serial.print(" Trim Yaw : ");
        Serial.print(ppm[5]);
        Serial.print(" Trim pitch : ");
        Serial.print(ppm[6]);
        Serial.print(" Trim Roll : ");
        Serial.print(ppm[7]);
        Serial.print(" Crc : ");
        Serial.println(ppm[8]);
        time = newTime;

    }
    void resetData()
    {
        rxValues.throttle = 0;
        //rxValues.yaw = 128;
        rxValues.yaw = 1500;
        rxValues.pitch = 128;
        rxValues.roll = 128;
        rxValues.flags = 0;
        rxValues.trim_yaw = 0;
        rxValues.trim_pitch = 0;
        rxValues.trim_roll = 0;
        setPPMValuesFromData();
    }


public:
    int ppm[8];
    RF24MJXRemoteControl()
    {
    }

    virtual bool getRemoteCommand(command_t& cmd)
    {
        cmd.stop();
        cmd.key = command_t::keyNone;

        time = micros();
        uint8_t value = protocol.run(&rxValues);
        //Serial.print("\t"); Serial.print(value);
        //Serial.print("\t"); Serial.println(rxValues.flags);

        newTime = micros();
        switch( value )
        {
        case  BIND_IN_PROGRESS:
            if(!bind_in_progress)
            {
                bind_in_progress = true;
                Serial.println("Bind in progress");
            }
            break;

        case BOUND_NEW_VALUES:
            setPPMValuesFromData();
            /*
            newTime = micros();

            Serial.print(newTime - time); //120 ms for 16 Mhz
            Serial.print(" :\t");Serial.print(rxValues.throttle);
            Serial.print("\t"); Serial.print(rxValues.yaw);
            Serial.print("\t"); Serial.print(rxValues.pitch);
            Serial.print("\t"); Serial.print(rxValues.roll);
            Serial.print("\t"); Serial.print(rxValues.trim_yaw);
            Serial.print("\t"); Serial.print(rxValues.trim_pitch);
            Serial.print("\t"); Serial.print(rxValues.trim_roll);
            Serial.print("\t"); Serial.println(rxValues.flags);

            time = newTime;
            */
            break;

        case BOUND_NO_VALUES:
            //Serial.print(newTime - time); Serial.println(" : ----"); // 32ms for 16Mhz
            break;

        default:
            break;
        }


        /*
        while ( radio.available() ) {
            radio.read(&data, sizeof(MyData));
            lastRecvTime = millis();
        }


        unsigned long now = millis();
        if ( now - lastRecvTime > 1000 ) {
               // signal lost?
            resetData();
        }
        */
        //setPPMValuesFromData();
        cmd.throttle = ppm[0] * ppm[4]; //throttle * flag
        cmd.rudder = ppm[1]; //map(ppm[1], 1000, 2000, 0, 180);

        #ifdef TANK
        if (ppm[1] > 1500){
            //Serial.println("Turn Right");
            cmd.turnRight();
        }else if(ppm[1] < 1500){
           // Serial.println("Turn Left");
            cmd.turnLeft();
        }else if(ppm[1] = 1500){
            cmd.goForward();
        }
        #endif



        #ifdef BOAT
        cmd.goForward();
        #endif // BOAT

        if (cmd.key !=command_t::keyNone && cmd.key == lastkey)
        {

            return false;
        }

        lastkey = cmd.key;

        delay(2);
        return true;
    }
};
}

#endif // RF24MJXREMOTECONTROL_H_INCLUDED
