#pragma once
#include "v5.h"
#include "v5_vcs.h"
#include "WPID/PID.h"

using namespace vex;

class Mechanism {
private:
    motor_group* motors;
    float gear_ratio;
    PID pid;

    float max_acceleration = 0;
public:
    /**
     * @brief Construct a new Mechanism object.
     * 
     * @param motors the motors used on the mechanism
     * @param gear_ratio the external gear ratio between motor group and mechanism
     */
    Mechanism(motor_group* motors, float gear_ratio);
    Mechanism() = default;

    /**
     * @brief Spins the motor group at the specified speed.
     *  
     */
    void spin(int velocity);

    /**
     * @brief stops the motors
     */
    void stop();

    /**
     * @brief Set the angle of the mechanism. The angle is the relative angle of your arm, in degrees.
     * 
     * 
     * @param angle 
     * @param max_speed 
     */
    void setAngle(float angle, float max_speed);

    /**
     * @brief Gets the position of the mechanism
     * 
     * @return float
     */
    float getPosition(rotationUnits units);

    /**
     * @brief resets the motor position
     * 
     */
    void resetPosition();
};
