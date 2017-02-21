#ifndef REMOTECONTROLDRIVER_H_INCLUDED
#define REMOTECONTROLDRIVER_H_INCLUDED
namespace dwijanto{
class RemoteControlDriver{
public:
    struct command_t{
            enum key_t {keyNone,
                        keyPower,
                        keyTurnLeft,
                        keyTurnRight,
                        keyF1,
                        keyF2,
                        keyF3,
                        keyF4}key;
            int left;
            int right;
			int throttle; //speed;
			int rudder; //steer left or right

            command_t():left(0),right(0),throttle(0),key(keyNone){}
			
            void goForward(){
				left = throttle;
				right = throttle;
            }
            void goBack(){
                left =-255;
                right = -255;
            }
            void goForward(int speed){
                left = speed;
                right = speed;
            }
            void goBack(int speed){
                left = speed * -1;
                right = speed * -1;
            }


            void turnLeft(){

				left = throttle * -1;
				right = throttle;
                //Serial.print("Speed");
                //Serial.println(speed);

            }
            void turnRight(){
				left = throttle;
				right = throttle * -1;
            }
            void turnLeft(int l){
				left = -throttle - l;
				right = throttle;

            }
            void turnRight(int r){
				left = throttle;
				right = throttle - r;
            }

            void stop(){
                left = 0;
                right = 0;
            }

            void leftAndRightSlider(int l,int r){
                left =l;
                right = r;
            }

            void forwardBackAndLeftRightSliders(int fb, int lr){
                left = fb - lr;
                right = fb + lr;
                if (left < -255) left = -255;
                else if (left > 255) left = 255;

                if (right < -255) right = -255;
                else if (right > 255) right = 255;
            }

            void joystick(int x,int y){
                forwardBackAndLeftRightSliders(x,y);
            }

        };

        RemoteControlDriver(){}

        virtual bool getRemoteCommand(command_t& cmd) =0;
};
}


#endif // REMOTECONTROLDRIVER_H_INCLUDED
