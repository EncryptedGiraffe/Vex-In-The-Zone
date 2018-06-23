#ifndef DRIVERS_H
#define DRIVERS_H


//this file contains declarations for functions in drivers.c

//leftSpeed controls speed of left wheels, rightSpeed controls speed of right wheels
void TankDrive(int leftSpeed, int rightSpeed);

//target is the potentiometer value that we are aiming for
void ArmDrive(int target);

//target to the potentiometer value that we are aiming for
int MainArmDrive(int target, float last, int potentiometer, int motor, int motorScale);

//1 == intake, -1 = spit out
void PincherDrive(int dir);

//dir controls which direction to move the mobile base grabber in.  1 = up, -1 = down, any other value do nothing
void GrabberDrive(int dir);

//target is the encoder value that we are aiming for
void SecondArmDrive(int target);

//get the position of the secondary arm
int GetSecondArmPosition();

//set all pin modes
void SetupPins();

//get how high to put the arms for stacking. return [2]. 0 == main arm height, 1 == secondary arm height
int* GetStackHeight(int height);

//turns on light based on autonomous setting
void AutonomousLights();

//1 == extended, -1 = in
void PusherDrive(int dir);

#endif
