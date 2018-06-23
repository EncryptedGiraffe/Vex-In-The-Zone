#include <main.h>

//variables
int armTarget = 2048;
int armLeftAnalog = 0;
int armRightAnalog = 0;
int armLeftDir = 0;
int armRightDir = 0;
bool armGrounded = false;
int secondArmPosition = 0;
int secondArmTarget = 0;
int secondArmSpeed = 0;
int secondArmDir = 0;
int pincherTimeout = 0;
bool canGrabberUp = true;
bool canGrabberDown = true;
int pusherTarget = 0;

//functions
//1 == first cone, 2 == second height
int* GetStackHeight(int height)
{
  static int r[2];

  switch (height) {
    case 1:
    //set arm targets
    r[0] = 2500;
    r[1] = -30;
    break;
    case 2:
    //set arm targets
    r[0] = 1850;
    r[1] = 10;
    break;
    case 3:
    //set arm targets
    r[0] = 2150;
    r[1] = 10;
    break;
    case 4:
    //set arm targets
    r[0] = 2200;
    r[1] = 10;
    break;
    case 5:
    //set arm targets
    r[0] = 2300;
    r[1] = 20;
    break;
    case 6:
    //set arm targets
    r[0] = 2400;
    r[1] = 30;
    break;
    case 7:
    //set arm targets
    r[0] = 2500;
    r[1] = 40;
    break;
    case 8:
    //set arm targets
    r[0] = 2600;
    r[1] = 40;
    break;
    case 9:
    //set arm targets
    r[0] = 2700;
    r[1] = 40;
    break;
    default:
    //set arm targets
    r[0] = 2600;
    r[1] = 90;
    break;
 }
 return r;
}
void TankDrive(int leftSpeed, int rightSpeed)
{
    //adjust right speed
    leftSpeed *= 0.9;
    //check threshold on inputs
    if(abs(leftSpeed) < driveThreshold)
    {
      //speed is smaller than threshold so don't give any power to this side
      leftSpeed = 0;
    }
    if(abs(rightSpeed) < driveThreshold)
    {
      //speed is smaller than threshold so don't give any power to this side
      rightSpeed = 0;
    }
    //set motor speeds
    motorSet(motorWheelsLeft, leftSpeed * speedDrive * -1);
    motorSet(motorWheelsRight, rightSpeed * speedDrive);
}

void PincherDrive(int dir)
{
  //check if release triggered
  if(dir == -1)
    //set timeout
    pincherTimeout = 40;

  //motor speed
  int motor = 0;

  //check if we are opening
  if(pincherTimeout == 0)
  {
    //check if cone button has been hit
    if(digitalRead(sensorPincherButton) == LOW)
    {
      //cone is loaded - set motor to keep the cone in
      motor = -30;
    }
    else
    {
      //check if intake
      if(dir == 1)
      {
        //we still need to grab a cone - motor to full power
        motor = -127;
      }
    }
  }
  else
  {
    //motor to release power
    motor = 127;
    //decrease timeout
    pincherTimeout -= 1;
  }
  //set motors
  motorSet(motorPincher, motor);
}

void GrabberDrive(int dir)
{
  //motor speed
  int motor = 0;

  //if we want to go up
  if(dir == 1)
  {
    //set motor to full power up
    motor = -127;
  }
  else if(dir == -1)
  {
    //set motor to full power down
    motor = 127;
  }
  else
  {
    //stop
    motor = 0;
  }
  //set motors
  motorSet(motorGrabber, motor * speedGrabber);
}

void PusherDrive(int dir)
{
  //check input to set target
  if(dir == 1)
  {
    //set target to out
    pusherTarget = pusherRange;
  }
  else if(dir == -1)
  {
    //set target to in
    pusherTarget = 0;
  }

  //get encoder input
  int value = encoderGet(pusherEncoder);

  //motor power
  int motor = 0;

  //check if we need to go forward
  if(value < (pusherTarget - 100))
  {
    //motor on
    motor = 127 * speedPusher;
  }
  else if(value > (pusherTarget + 100))
  {
    //motor reverse
    motor = -127 * speedPusher;
  }
  else
  {
    //motor off
    motor = 0;
  }

  //set motor
  motorSet(motorPusher, motor);
}

int MainArmDrive(int target, float lastError, int potentiometer, int motor, int motorScale)
{
  float  pidSensorCurrentValue;

   float  pidError;
   float  pidLastError;
   float  pidIntegral;
   float  pidDerivative;
   float  pidDrive;

   // Init the variables - thanks Glenn :)
   pidLastError  =  lastError;
   pidIntegral   = 0;
   // Is PID control active ?
   if( true )
       {
       // Read the sensor value and scale
       pidSensorCurrentValue = analogRead(potentiometer);

       // calculate error
       pidError = pidSensorCurrentValue - target;

       // integral - if Ki is not 0
       if( pid_Ki != 0 )
           {
           // If we are inside controlable window then integrate the error
           if( abs((int)pidError) < PID_INTEGRAL_LIMIT )
               pidIntegral = pidIntegral + pidError;
           else
               pidIntegral = 0;
           }
       else
           pidIntegral = 0;

       // calculate the derivative
       pidDerivative = pidError - pidLastError;
       pidLastError  = pidError;

       //calculate window
       if(abs((int)pidError) < 50)
       {
         pidError = 0;
       }

       // calculate drive
       pidDrive = (pid_Kp * pidError) + (pid_Ki * pidIntegral) + (pid_Kd * pidDerivative);

       // limit drive
       if( pidDrive > PID_DRIVE_MAX )
           pidDrive = PID_DRIVE_MAX;
       if( pidDrive < PID_DRIVE_MIN )
           pidDrive = PID_DRIVE_MIN;

       // send to motor
       motorSet(motor, pidDrive * motorScale);
       return pidLastError;
       }
}

/*
void ArmDrive(int target)
{
  //update target
  armTarget = target;
  //get potentiometer values
  armLeftAnalog = analogRead(sensorArmLeftPotentiometer);
  armRightAnalog = analogRead(sensorArmRightPotentiometer);

  //check for edges
  if(armTarget < armTargetLowerLimit)
  {
    armTarget = armTargetLowerLimit;
  }
  else if(armTarget > armTargetUpperLimit)
  {
    armTarget = armTargetUpperLimit;
  }

  //set direction - if the analog is greater than target, move up. Else, move down
  //left
  if(armLeftAnalog > armTarget)
  {
    armLeftDir = 1;
  }
  else
  {
    armLeftDir = -1;
  }
  //right
  if(armRightAnalog > armTarget)
  {
    armRightDir = 1;
  }
  else
  {
    armRightDir = -1;
  }
  //if the absolute of analog minus target is greater than 300, motors to full power
  //left arm tower
  if(abs(armLeftAnalog - armTarget) > 200)
  {
    //motors to full power times direction times armSpeed
    motorSet(motorArmLeft, 127 * armLeftDir * speedArm);
  }
  //our window for acceptable variation on target
  else if(abs(armLeftAnalog - armTarget) < 20)
  {
    //is the arm grounded
    if(armGrounded)
    {
      //turn motors to zero power
      motorSet(motorArmLeft, 0.0F);
    }
    else
    {
      //turn motors to base power
      motorSet(motorArmLeft, armBasePower);
    }
  }
  //if the absolute of analog - target is smaller than 300, motors to decreasing power
  else
  {
    //motors to full power time direction times armSpeed times the absolute of analog minus target divided by 200.
    //This will give decreasing speed as we get close to target
    motorSet(motorArmLeft, 127 * armLeftDir * speedArm * abs(armLeftAnalog - armTarget) / 200);
  }

  //right arm tower
  if(abs(armRightAnalog - armTarget) > 200)
  {
    //motors to full power times direction times armSpeed
    motorSet(motorArmRight, -127 * armRightDir * speedArm);
  }
  //our window for acceptable variation on target
  else if(abs(armRightAnalog - armTarget) < 20)
  {
    //is the arm grounded
    if(armGrounded)
    {
      //turn motors to zero power
      motorSet(motorArmRight, 0.0F);
    }
    else
    {
      //turn motors to base power
      motorSet(motorArmRight, armBasePower);
    }
  }
  //if the absolute of analog - target is smaller than 300, motors to decreasing power
  else
  {
    //motors to full power time direction times armSpeed times the absolute of analog minus target divided by 200.
    //This will give decreasing speed as we get close to target
    motorSet(motorArmRight, -127 * armRightDir * speedArm * abs(armRightAnalog - armTarget) / 200);
  }
}
*/

void SecondArmDrive(int target)
{
  //calculate position
  secondArmPosition = encoderGet(secondArmEncoder);

  //adjust target
  secondArmTarget = target;

  //check range
  if(secondArmTarget < secondArmTargetInnerLimit)
  {
    secondArmTarget = secondArmTargetInnerLimit;
  }
  else if(secondArmTarget > secondArmTargetOuterLimit)
  {
    secondArmTarget = secondArmTargetOuterLimit;
  }

  //check dir
  if(secondArmPosition > secondArmTarget)
  {
    secondArmDir = 1;
  }
  else
  {
    secondArmDir = -1;
  }

  if(abs(secondArmPosition - secondArmTarget) > 30)
  {
    //motors to full power times direction times speed
    secondArmSpeed = 127 * secondArmDir * speedSecondArm;
  }
  //our window for acceptable variation on target
  else if(abs(secondArmPosition - secondArmTarget) < 2)
  {
    secondArmSpeed = 0.0F;
  }
  //if the absolute of position - target is smaller than 30, motors to decreasing power
  else
  {
    //This will give decreasing speed as we get close to target
    secondArmSpeed = 127 * secondArmDir * speedSecondArm * abs(secondArmPosition - secondArmTarget) / 30;
  }
  //set motors
  motorSet(motorSecondArm, -secondArmSpeed);
}

int GetSecondArmPosition()
{
  //spit out the position of the second arm
  return secondArmPosition;
}

void AutonomousLights()
{
  int value = analogRead(sensorAutoPotentiometer);

  if(value > 2000)
  {
    //turn light 11 on
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
  }
  else
  {
    //turn light 12 on
    digitalWrite(11, HIGH);
    digitalWrite(12, LOW);
  }
}

void SetupPins()
{
  //set pinmodes
  pinMode(sensorArmLeftPotentiometer, INPUT_ANALOG);
  pinMode(sensorArmRightPotentiometer, INPUT_ANALOG);
  pinMode(sensorAutoPotentiometer, INPUT_ANALOG);
  pinMode(sensorPincherButton, INPUT);
  pinMode(sensorMobileUp, INPUT);
  pinMode(sensorMobileDown, INPUT);
  //set lights
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}
