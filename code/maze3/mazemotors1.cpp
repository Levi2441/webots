#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Gyro.hpp>
#include <stdlib.h>


using namespace webots;

#include "mazemotors1.h"

#define TURN_SPEED 1
#define MOTOR_SPEED 3


void updateGyro(int timeStep) {
    angle += (timeStep / 1000.0) * (gyro->getValues())[1];
}
double getAngle() {
    return angle * 180 / 3.1415;
}
void setGyroR() {
  targetG = getAngle();

  leftMotor->setVelocity(1);
  rightMotor->setVelocity(-1);
}
bool doneGyroR(int timeStep) {
  updateGyro(timeStep);
  if(getAngle() <= targetG-90)
  {
      leftMotor->setVelocity(0);
       rightMotor->setVelocity(0);
       return true;
  }
  return false;
}
void setMotors(double lef, double rig) {
  leftMotor->setVelocity(lef);
  rightMotor->setVelocity(rig);
}
void setReverseTurn() {
  targetL = leftEncoder->getValue();
  targetR = rightEncoder->getValue();
  
  targetL -= 180 * rPD;
  targetR += 180 * rPD;
  
  leftMotor->setVelocity(-1);
  rightMotor->setVelocity(1);
}
bool doneReverseTurn() {
if (rightEncoder->getValue() >= targetR) {
       leftMotor->setVelocity(0);
       rightMotor->setVelocity(0);
       return true;
    }
    return false;
}
void setTurnL(int angle) {
   //targetL = leftEncoder->getValue();
   //targetR = rightEncoder->getValue();
  
  
  targetL -= angle * rPD;
  targetR += angle * rPD;
  
  leftMotor->setVelocity(-1 * TURN_SPEED);
  rightMotor->setVelocity(TURN_SPEED);
}
bool doneTurnL() {
if (rightEncoder->getValue() > targetR) {
       leftMotor->setVelocity(0);
       rightMotor->setVelocity(0);
       return true;
    }
    return false;
}
void setTurnR(int angle) {
  
   //targetL = leftEncoder->getValue();
   //targetR = rightEncoder->getValue();
 
 
  targetL += angle * rPD;
  targetR -= angle * rPD;
  

  
  leftMotor->setVelocity(TURN_SPEED);
  rightMotor->setVelocity(TURN_SPEED * -1);

  //printf("got here too\n");
  //printf("target s3: %f\n", targetL);
}

bool doneTurnR() {

   // printf("Target: %f Curr: %f\n-----------\n", targetL, leftEncoder->getValue());
    
    if (leftEncoder->getValue() > targetL) {
       leftMotor->setVelocity(0);
       rightMotor->setVelocity(0);
       return true;
    }
    return false;
  
}
void setForward(double meters) {
   targetL = leftEncoder->getValue();
   targetR = rightEncoder->getValue();
 
  
  targetL += (meters * rPM);
  targetR += (meters * rPM);
  
  leftMotor->setVelocity(MOTOR_SPEED);
  rightMotor->setVelocity(MOTOR_SPEED);
}
bool doneForward() {
  if (leftEncoder->getValue() >= targetL) {
      leftMotor->setVelocity(0);
      rightMotor->setVelocity(0);
      return true;
    }
    return false;
}
