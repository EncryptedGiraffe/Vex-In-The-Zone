#ifndef VARS_H
#define VARS_H

#include "main.h"

//motors
#define motorWheelsRight 2 //right side wheels
#define motorWheelsLeft 3 //left side wheels
#define motorArmLeft 4 //left arm tower motors
#define motorArmRight 5 //right arm tower motors
#define motorPincher 6 //pincher motor
#define motorGrabber 7 //mobile base grabber motors
#define motorSecondArm 8 //secondary rotating arm motor
#define motorPusher 9 //mobile base pusher motor

//speeds
#define speedDrive 1.0 //driving speed
#define speedArm 1.0 //arm speed
#define speedPincher 0.5 //pincher speed
#define speedGrabber 0.5 //grabber speed
#define speedSecondArm 1.0 //secondary rotating arm speed
#define speedPusher 0.7 //mobile base pusher speed

//sensors
#define sensorArmLeftPotentiometer 1 //left arm potentiometer into analog 1
#define sensorArmRightPotentiometer 2 //right arm potentiometer into analog 2
#define sensorSecondArmEncoderTop 4 //secondary rotating arm optical shaft encoder top in digital 4
#define sensorSecondArmEncoderBottom 5 //secondary rotating arm optical shaft encoder bottom in digital 5
Encoder secondArmEncoder; //secondary rotating arm optical shaft encoder
#define sensorAutoPotentiometer 3 //autonomous selector potentiometer into analog 3
#define sensorPincherButton 6 //cone detector button into digital 6
#define sensorPusherEncoderTop 7 //MOGO pusher optical shaft encoder top in digital 7
#define sensorPusherEncoderBottom 8 //MOGO pusher optical shaft encoder in digital 8
Encoder pusherEncoder; //shaft encoder for MOGO pusher
#define sensorMobileUp 9 //limit switch for mobile base upper limit
#define sensorMobileDown 10 //limit switch for mobile base lower limit

//other variables
#define driveThreshold 15 //the threshold for reading joystick input for driving
#define armBasePower 15 //the base power of the arm when stopped
#define armTargetUpperLimit 3000 //the highest the arm can go
#define armTargetLowerLimit 1500 //the lowest the arm can go
#define secondArmTargetInnerLimit -45 //the farthest the arm can move in towards the robot
#define secondArmTargetOuterLimit 195 //the farthest the arm can move out. This is where we will go to pick up cones
#define pusherRange 1400 //the target for the MOGO pusher
#define PID_MOTOR_SCALE     -1
#define PID_DRIVE_MAX       127
#define PID_DRIVE_MIN     (-127)
#define PID_INTEGRAL_LIMIT  50
#define pid_Kp 0.25
#define pid_Ki 0.1
#define pid_Kd 0.01

#endif
