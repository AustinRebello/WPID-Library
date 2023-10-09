#pragma once
#include "v5.h"
#include "PID.h"
using namespace vex;

class Chassis {
    private:
        bool debug = true;
        float wheel_base;
        float track_width;
        float wheel_circumference;
        motor_group* left;
        motor_group* right;
        PID pidStraight;
        PID pidTurn;
        encoder* leftEnc = nullptr;
        encoder* rightEnc = nullptr;

        /**
         * @brief Sets the target position of each side of the chassis.
         * This uses an open loop algorithm to move the robot with PID to its target.
         * Does not use odometry to calculate error. Cannot adjust for sideways error.
         * @param left_target the left side's target in SOME UNITS
         * @param right_target the right side's target in SOME UNITS
         * @param l_max_spd the max speed the left side should spin
         * @param r_max_spd the max speed the right side should spin
         * @param PID PID object to rely on
         */
        void setTarget(float left_target, float right_target, int l_max_spd, int r_max_spd, PID pid);

    public:
        double turn_offset = 0;
        /**
         * @brief Construct a new Chassis object. 
         * All distance units are assumed to be in inches.
         * 
         * @param track_width the width between left and right
         * @param wheel_base the distance from front to back of the wheel
         * @param wheel_radius radius of the wheel 
         * @param left motor group
         * @param right motor group
         */
        Chassis(float track_width, float wheel_base, float wheel_radius, motor_group* left, motor_group* right);
        Chassis() = default;

        /**
         * @brief Sets the straight line PID constants 
         * @param pid 
         */
        void setStraightPID(PID pid);

        /**
         * @brief Sets the turning PID constants
         * @param pid 
         */
        void setTurnPID(PID pid);

        /**
         * @brief Spin the entire chassis by specified velocities for each
         * side of the chassis
         * 
         * @param left_speed 
         * @param right_speed 
         */
        void spin(int left_speed, int right_speed);

        /**
         * @brief Spin the entire chassis by specified velocities for both sides
         * @param velocity 
         */
        void spin(int velocity);

        /**
         * @brief Stops the chassis from moving
         */
        void stop();

        /**
         * @brief Reset the left and right encoders
         */
        void resetEncoders();

        /**
         * @brief Move the chassis forward with the specified PID constants.
         * Chassis will always stay at or below the maximum speed.
         * @param distance 
         * @param max_speed 
         */
        void forward(float distance, int max_speed);

        /**
         * @brief Turn the chassis on the spot with the specified PID constants.
         * Chassis will always stay at or below the maximum speed.
         * @param target_angle 
         * @param max_speed 
         */
        void turn(int target_angle, int max_speed);

        /**
         * @brief Drive the chassis on an arc
         * 
         * @param x 
         * @param y 
         * @param max_speed 
         * @return float 
         */
        float arc(float x, float y, int max_speed);
        void arc(float r, int theta, int max_speed);

        /**
         * @brief Gets the current position of the left side of the chassis
         * 
         * @param units typically useing rotationUnits::deg
         * @return float 
         */
        float leftEncoder(rotationUnits units);

        /**
         * @brief Gets the current position of the right side of the chassis
         * 
         * @param units typically useing rotationUnits::deg
         * @return float 
         */
        float rightEncoder(rotationUnits units);

        /**
         * @brief Sets the encoders
         * 
         * @param left The left encoder
         * @param right The right encoder
         */
        void setEncoders(encoder* left, encoder* right);

        /**
         * @brief Sets the brake type of the chassis by passing a brake type as a parameter.
         * @param type The brake type can be set to coast, brake, or hold.  
         */
        void setBrakeType(brakeType type);

        /**
         * @brief Engages the motors to reduce dead zone between gears.
         * Use this when you need to reset the precesision of the robot.
         * @attention WIP
         */
        void engage(int l, int r);
};      
