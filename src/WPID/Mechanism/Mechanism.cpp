#include "WPID/Mechanism/Mechanism.h"
using namespace vex;

Mechanism::Mechanism(motor_group* motors, float gear_ratio){
    this->motors = motors;
    this->gear_ratio = gear_ratio;
}

void Mechanism::spin(int velocity){
    float position = this->getPosition(rotationUnits::deg);
    if((velocity > 0 && position < upper_bound)
    || (velocity < 0 && position > lower_bound)
    || (upper_bound == lower_bound)){
        motors->spin(directionType::fwd, velocity, velocityUnits::pct);
    } else {
        motors->stop();
    }
}

void Mechanism::stop(){
    motors->stop();
}

void Mechanism::setPosition(float position, float max_speed){
    float target = (position + offset) * gear_ratio;
    target += this->getPosition(rotationUnits::deg); // retains state
    
    float state = 0;
    float error = 999;
    int calc = 0;
    float ramp = 0;

    int startTime = (int)vex::timer::system();

    while(pid.cont(error)){
        if((int)vex::timer::system() >= startTime + timeout) {break;}
        state = this->getPosition(rotationUnits::deg); // get the state of the motors
        error = target - state; // difference between target and state
        calc = pid.calculateSpeed(error, max_speed); // calculate PID speed for the left side
        
        if(error > target*(1 - MAX_RAMP_DURATION) && ramp <= max_speed && ramp > 0) {
            calc = target < 0 ? 0.0 - ramp : ramp;
            ramp += max_acceleration;
        }

        this->spin(calc); // spin the motors at speed
        wait(pid.delay_time, msec); // delay by pid.delay_time milliseconds
    }
    pid.reset();
    this->stop();
}

float Mechanism::getPosition(rotationUnits units){
    return motors->position(units) * gear_ratio;
}

void Mechanism::resetPosition(){
    motors->resetPosition();
}

void Mechanism::setBrakeType(brakeType type){
    motors->setStopping(type);
}

void Mechanism::setPID(PID pid){
    this->pid = pid;
}

void Mechanism::setOffset(float offset){
    this->offset = offset;
}

void Mechanism::setMaxAcceleration(float max_accel){
    this->max_acceleration = max_accel;
}

void Mechanism::setBounds(float upper_bound, float lower_bound){
    this->upper_bound = upper_bound;
    this->lower_bound = lower_bound;
}

void Mechanism::setTimeout(int timeout){
    this->timeout = timeout;
}

void Mechanism::setName(char* name){
    std::string t = std::string(name);
    this->name = t;
}