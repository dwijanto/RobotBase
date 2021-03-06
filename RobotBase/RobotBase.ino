#include <Arduino.h>
#include <SPI.h>

#define MPU_DEVICE
#define REMOTE_CONTROL
#define L9110S_DRIVER
//#define SERV0_DEVICE

//MPU6050
#ifdef MPU_DEVICE
#include "PID_V1.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
MPU6050 mpu;
#endif // MPU_DEVICE

// RC
#ifdef REMOTE_CONTROL
//#define TANK
#define BOAT
#include "V202_protocol.h"
v202Protocol protocol;
#include "RF24MJXRemoteControl.h"
nrf24l01p radio;
#define RF24_PIN 8,10
#endif // REMOTE_CONTROL

//Motor
#ifdef L9110S_DRIVER
#include "L9110SMotorDriver.h"
#define LEFT_MOTOR_INIT 3,5
#define RIGHT_MOTOR_INIT 6,9
#endif // L9110S_DRIVER

//Servo
#ifdef SERV0_DEVICE
#include <Servo.h>
#define RUDDER_PIN 4
Servo servo1;
#endif // SERV0_DEVICE

/*
  Turns on an LED on for one second, then off for one second, repeatedly.
*/
namespace dwijanto
{
class Robot
{
private:
public:
#ifdef REMOTE_CONTROL
    RemoteControlDriver::command_t remoteCmd;
    RF24MJXRemoteControl remoteControl;
#endif // REMOTE_CONTROL

    unsigned long int servoCycle;
    Motor _leftMotor;
    Motor _rightMotor;

    Robot():_leftMotor(LEFT_MOTOR_INIT),_rightMotor(RIGHT_MOTOR_INIT)
#ifdef REMOTE_CONTROL
        ,remoteControl()
#endif // REMOTE_CONTROL
    {
        initialize();
    }

    void initialize()
    {
        servoCycle = millis();
    }


    void run()
    {

    }

#ifdef REMOTE_CONTROL
    void runremote()
    {
        bool haveRemoteCmd = remoteControl.getRemoteCommand(remoteCmd);
        if(haveRemoteCmd)
        {
            switch(remoteCmd.key)
            {
            case RemoteControlDriver::command_t::keyF1:
                break;
            case RemoteControlDriver::command_t::keyNone:
                //Serial.print(remoteCmd.left);
                //Serial.println(remoteCmd.right);
                _leftMotor.setSpeed(remoteCmd.left);
                _rightMotor.setSpeed(remoteCmd.right);
                break;
            default:
                break;
            }
        }

        #ifdef SERV0_DEVICE
        if (millis() - servoCycle > 15)
        {
            servo1.writeMicroseconds(remoteCmd.rudder == 0 ? 1500:remoteCmd.rudder);
            servoCycle = millis();

        }
        #endif // SERV0_DEVICE





    }
#endif // REMOTE_CONTROL


};

}

dwijanto::Robot robot;

void setup()
{
    Serial.begin(9600);


    // initialize the digital pin as an output.
    // Pin 13 has an LED connected on most Arduino boards:
    pinMode(13, OUTPUT);

#ifdef REMOTE_CONTROL
    pinMode(SS, OUTPUT);
    radio.setPins(RF24_PIN);
    protocol.init(&radio);
#endif // REMOTE_CONTROL


    Serial.println("Start Robot");
    Serial.println("CodeBlock 13.12 Project");
    Serial.println("Arduino Nano (328)");
    Serial.println("Project Name: RobotBase.cbp");
    Serial.println("Location: C:/Users/dwijanto/Documents/GitHub/RobotBase");
    Serial.println("https://github.com/dwijanto/RobotBase");
    Serial.println("dwijanto@yahoo.com");
    #ifdef SERV0_DEVICE
    servo1.attach(RUDDER_PIN);
    #endif // SERV0_DEVICE




}

void loop()
{


    //delay(1000);              // wait for a second
    //digitalWrite(13, HIGH);   // set the LED on
    //delay(1000);              // wait for a second
    //digitalWrite(13, LOW);    // set the LED off
    //robot.runremote();

    robot.run();
    //servo1.writeMicroseconds(2000);
}
