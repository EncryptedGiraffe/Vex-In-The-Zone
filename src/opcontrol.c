#include "main.h"

 //Variables
int arm_target = 1500;
int secondArm_target = 0;
bool goingToPickupHeight = false;
bool goingToLoaderHeight = false;
bool goingToRestHeight = false;
int conesStacked = 0;
bool incrementToggle = false;
bool decrementToggle = false;
int *stackTemp;
int driveDir = 1;
bool driveToggle = false;
float lastErrorRight = 0;
float lastErrorLeft = 0;

void operatorControl()
{
	//setup
	SetupPins();

	//loop
	while (1)
	{
		//delay
		delay(20);
		//turn on autonomous lights
		AutonomousLights();
		//////////Automated Functions//////////
		//go to cone grabbing height
		if(joystickGetDigital(2, 7, JOY_LEFT))
		{
			//we are going to pickup height
			goingToPickupHeight = true;
		}
		//check if we should be moving
		if(goingToPickupHeight)
		{
			//set second arm to pickup height which just happens to be the outer limit
			secondArm_target = secondArmTargetOuterLimit;
			//check if we should set main arm height yet
			if(GetSecondArmPosition() > 100)
			{
				//set main arm target now
				arm_target = 2000;
				//done checking
				goingToPickupHeight = false;
			}
		}
		//go to loader grabbing height
		if(joystickGetDigital(2, 7, JOY_RIGHT))
		{
			//we are going to loader height
			goingToLoaderHeight = true;
		}
		//check if we should be moving
		if(goingToLoaderHeight)
		{
			//set second arm to loader pickup height
			secondArm_target = 150;
			//check if we should set main arm height yet
			if(GetSecondArmPosition() > 100)
			{
				//set main arm target now
				arm_target = 2400;
				//done checking
				goingToLoaderHeight = false;
			}
		}
		//go to cone stacking height
		if(joystickGetDigital(2, 8, JOY_DOWN))
		{
			//set the stack height to one above how many are currently stacked
			stackTemp = GetStackHeight(conesStacked + 1);
			arm_target = stackTemp[0];
			secondArm_target = stackTemp[1];
		}
		//check if cones stacked reset button has been hit
		if(joystickGetDigital(2, 8, JOY_RIGHT))
		{
			//reset the count
			conesStacked = 0;
		}
		//check for increment button
		if(joystickGetDigital(2, 8, JOY_UP) && incrementToggle == false)
		{
			//toggle
			incrementToggle = true;
			//increment counter
			conesStacked += 1;
		}
		else if(!joystickGetDigital(2, 8, JOY_UP))
		{
			//not toggled
			incrementToggle = false;
		}
		//should we switch drive direction
		if(joystickGetDigital(1, 8, JOY_DOWN) && driveToggle == false)
		{
			//switch drive direction
			if(driveDir == 1)
			{
				driveDir = -1;
			}
			else if(driveDir == -1)
			{
				driveDir = 1;
			}
			driveToggle = true;
		}
		else if(!joystickGetDigital(1, 8, JOY_DOWN))
		{
			driveToggle = false;
		}
		//should we go to rest position
		if(joystickGetDigital(2, 8, JOY_LEFT))
		{
			//go to rest height
			goingToRestHeight = true;
		}
		//are we going to rest height
		if(goingToRestHeight)
		{
			//set second arm target
			secondArm_target = 90;
			//check if arm is past 70
			if(GetSecondArmPosition() > 70 && GetSecondArmPosition() < 110)
			{
				//set main arm target
				arm_target = armTargetLowerLimit;
				//done
				goingToRestHeight = false;
			}
		}
		//////////Automated Functions//////////
		//wheel drive
		if(driveDir == 1)
		{
			TankDrive(joystickGetAnalog(1, 3), joystickGetAnalog(1, 2));
		}
		else
		{
			TankDrive(joystickGetAnalog(1, 2) * -1, joystickGetAnalog(1, 3) * -1);
		}
		//pincher controller
		if(joystickGetDigital(2, 7, JOY_UP))
		{
			PincherDrive(1);
		}
		else if(joystickGetDigital(2, 7, JOY_DOWN))
		{
			PincherDrive(-1);
		}
		else
		{
			PincherDrive(0);
		}
		//mobile base grabber
		if(joystickGetDigital(1, 6, JOY_UP))
		{
			//go up
			GrabberDrive(1);
		}
		else if(joystickGetDigital(1, 6, JOY_DOWN))
		{
			//go down
			GrabberDrive(-1);
		}
		else
		{
			GrabberDrive(0);
		}
		//mobile base pusher
		if(joystickGetDigital(1, 5, JOY_UP))
		{
			PusherDrive(1);
		}
		else if(joystickGetDigital(1, 5, JOY_DOWN))
		{
			PusherDrive(-1);
		}
		else
		{
			PusherDrive(0);
		}
		//arm control - get inputs
		if(joystickGetDigital(2, 6, JOY_UP))
		{
			arm_target += 20;
		}
		else if(joystickGetDigital(2, 6, JOY_DOWN))
		{
			arm_target -= 20;
		}
		//check range
		if(arm_target < armTargetLowerLimit)
		{
			arm_target = armTargetLowerLimit;
		}
		else if(arm_target > armTargetUpperLimit)
		{
			arm_target = armTargetUpperLimit;
		}
		lastErrorRight = MainArmDrive(arm_target, lastErrorRight, sensorArmRightPotentiometer, motorArmRight, -1);
		lastErrorLeft = MainArmDrive(arm_target, lastErrorLeft, sensorArmLeftPotentiometer, motorArmLeft, 1);
		//secondary rotating arms
		if(joystickGetDigital(2, 5, JOY_UP))
		{
			secondArm_target -= 1;
		}
		else if(joystickGetDigital(2, 5, JOY_DOWN))
		{
			secondArm_target += 1;
		}
		//check range
		if(secondArm_target < secondArmTargetInnerLimit)
		{
			secondArm_target = secondArmTargetInnerLimit;
		}
		else if(secondArm_target > secondArmTargetOuterLimit)
		{
			secondArm_target = secondArmTargetOuterLimit;
		}
		SecondArmDrive(secondArm_target);
		//////////AUTONOMOUS TRIGGER/////////////
		/*
		if(joystickGetDigital(1, 7, JOY_UP) && joystickGetDigital(1, 7, JOY_DOWN))
		{
			//enter autonomous
			autonomous();
		}
		*/
		/////////AUTONOMOUS TRIGGER//////////////
	}
}
