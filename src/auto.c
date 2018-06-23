/** @file auto.c
 * @brief File for autonomous code
 *
 * This file should contain the user autonomous() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user autonomous code. This function will be started in its own task with the default
 * priority and stack size whenever the robot is enabled via the Field Management System or the
 * VEX Competition Switch in the autonomous mode. If the robot is disabled or communications is
 * lost, the autonomous task will be stopped by the kernel. Re-enabling the robot will restart
 * the task, not re-start it from where it left off.
 *
 * Code running in the autonomous task cannot access information from the VEX Joystick. However,
 * the autonomous function can be invoked from another task if a VEX Competition Switch is not
 * available, and it can access joystick information if called in this way.
 *
 * The autonomous task may exit, unlike operatorControl() which should never exit. If it does
 * so, the robot will await a switch to another mode or disable/enable cycle.
 */
 //variables
 int target = 1600;
 int secondTarget = 30.0;
 int leftSpeed = 0;
 int rightSpeed = 0;
 int pinch = 0;
 int grabDir = 0;
 int autoTime = 0;
 bool autoGoingToPickupHeight = false;
 int* stackTmp;
 int autoAnalog = 0;
 int side = 0; //0 = right, 1 = left
 int autoMode = 0;
 /* autoMode:
 0 = mobile base right side
 1 = mobile base left side
 2 = stationary base right side
 3 = stationary base left side
 4 = stationary base middle
 */
 //functions
 void AutonomousMobile()
 {
   //mobile base grabber right side start
   if(autoTime == 20)
   {
     //move forward
     leftSpeed = 127;
     rightSpeed = 127;
     //raise second arm
     secondTarget = 90;
     //move mobile base grabber into position
     grabDir = -1;
   }
   if(autoTime == 1500)
   {
     //stop moving mobile base grabber
     grabDir = 0;
   }
   if(autoTime == 3200)
   {
     //stop
     leftSpeed = 0;
     rightSpeed = 0;
     //lift base
     grabDir = 1;
   }
   if(autoTime == 4000)
   {
     //stop base grabber
     grabDir = 0;
     //go backwards
     leftSpeed = -127;
     rightSpeed = -127;
   }
   if(autoTime == 6000)
   {
     //turn
     leftSpeed = -127;
     rightSpeed = 127;
   }
   if(autoTime == 7500)
   {
     //forward
     leftSpeed = 127;
     //lower base
     grabDir = -1;
   }
   if(autoTime == 8000)
   {
     //backwards
     leftSpeed = -127;
     rightSpeed = -127;
   }
   if(autoTime == 9000)
   {
     //stop
     leftSpeed = 0;
     rightSpeed = 0;
   }
 }
void autonomous()
{
  int lastErrorRight = 0;
  int lastErrorLeft = 0;

  //set pins
	SetupPins();

  //loop
  while(true)
  {
    //wait
    delay(20);

    //turn on autonomous lights
    AutonomousLights();

    //get which autonomous to do
    int value = analogRead(sensorAutoPotentiometer);

    if(value > 2000)
    {
      side = 0;
    }
    else
    {
      side = 1;
    }

    //add to autonomous time
    autoTime += 20;

    //drivers
    lastErrorRight = MainArmDrive(target, lastErrorRight, sensorArmRightPotentiometer, motorArmLeft, -1);
    lastErrorLeft = MainArmDrive(target, lastErrorLeft, sensorArmLeftPotentiometer, motorArmLeft, 1);
    GrabberDrive(grabDir);
    PincherDrive(pinch);
    TankDrive(leftSpeed, rightSpeed);
		SecondArmDrive(secondTarget);

		//check if we should be moving
		if(autoGoingToPickupHeight)
		{
			//set second arm to pickup height which just happens to be the outer limit
			secondTarget = secondArmTargetOuterLimit;
			//check if we should set main arm height yet
			if(GetSecondArmPosition() > 100)
			{
				//set main arm target now
				target = 1600;
				//done checking
				autoGoingToPickupHeight = false;
			}
		}

		//autonomous checks
		switch(autoMode)
		{
			case 0:
      //right
      side = 0;
			AutonomousMobile();
			break;
			case 1:
      //left
      side = 1;
			AutonomousMobile();
			break;
      //do nothing
			break;
		}
	}
}
