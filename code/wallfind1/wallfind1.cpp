#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Gyro.hpp>
#include <webots/GPS.hpp>
#include <webots/LightSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace webots;


Robot *robot = new Robot();
Motor *leftMotor = robot->getMotor("left wheel motor"); 
Motor *rightMotor = robot->getMotor("right wheel motor");
 
PositionSensor *leftEncoder = leftMotor->getPositionSensor();
PositionSensor *rightEncoder = rightMotor->getPositionSensor();

LightSensor *leftHeat = robot->getLightSensor("ls1");
LightSensor *rightHeat = robot->getLightSensor("ls2");

Gyro *gyro = robot->getGyro("gyro");

bool checkTurnR();

double angle = 0;
const double rPM = 50; //should be 40
bool moving;
double target;

void updateGyro(int timeStep) {
    angle += (timeStep / 1000.0) * (gyro->getValues())[1];
}
double getAngle() {
    return angle * 180 / 3.1415;
}

GPS *gps = robot->getGPS("gps");

DistanceSensor *distances[8];
double ds[8];

// All the webots classes are defined in the "webots" namespace

void getShortDist() { //return pointer
  for (int i = 0; i < 8; i++) 
    ds[i] = distances[i]->getValue();
}
void setMotors(double lef, double rig) {
  leftMotor->setVelocity(lef);
  rightMotor->setVelocity(rig);
}
double light[2];
void getColor() {
  light[0] = leftHeat->getValue();
  light[1] = rightHeat->getValue();
}

double getLHeat() {
  return leftHeat->getValue();
}
double getRHeat() {
  return rightHeat->getValue();
}
double getG() {
  return gyro->getValues()[1];
}
double left = 0;
bool check_movement() {
  if (abs(leftEncoder->getValue() - target) < 0.1) {
    leftMotor->setVelocity(0);
    rightMotor->setVelocity(0);
    return true;
   }
   return false;
}
void movement(int meters) {
  int timeStep = (int)robot->getBasicTimeStep();
  target += (double)meters * rPM;

  leftMotor->setVelocity(1);
  rightMotor->setVelocity(1);
  
  while (robot->step(timeStep) != -1) {
    if (check_movement() == true)
      break;
   }
}
int turnTar;
void turnR() {
  int timeStep = (int)robot->getBasicTimeStep();
  updateGyro(timeStep);

  double moduI = fmod(getAngle(), 360);
  turnTar = moduI;
  
  leftMotor->setVelocity(1);
  rightMotor->setVelocity(-1);
  
  double moduT;
  while (robot->step(timeStep) != -1) {
    updateGyro(timeStep);
    moduT = fmod(getAngle(), 360);
    if (abs(moduT - turnTar) >= 90) {
      leftMotor->setVelocity(0);
      rightMotor->setVelocity(0);
      break;
     }
    }
 }

bool checkTurnL() {
  if (abs(getAngle() - left) > 455) { //425 was good L for 1,-1
		
        leftMotor->setVelocity(0);
        rightMotor->setVelocity(0);
        return true;
    }

    return false;
}
void turnL() {
  int timeStep = (int)robot->getBasicTimeStep();
  left = getAngle();
  
  leftMotor->setVelocity(-1);
  rightMotor->setVelocity(1);
  
  while (robot->step(timeStep) != -1) {
    updateGyro(timeStep);
    if (checkTurnL() == true) {
      break;
     }
    }
}

int main(int argc, char **argv) {
  // create the Robot instance.

  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();
  leftEncoder->enable(timeStep);
  leftHeat->enable(timeStep);
  rightHeat->enable(timeStep);
  gyro->enable(timeStep);
  gps->enable(timeStep);

  
  char buf[4];
  for(int i = 0; i < 8; i++) {
    snprintf(buf, 4, "ps%d", i);
    distances[i] = robot->getDistanceSensor(buf);
    distances[i]->enable(timeStep);
  }

  DistanceSensor *dsn = robot->getDistanceSensor("dsn");
  dsn->enable(timeStep);
  
  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);
  
  leftMotor->setVelocity(0);
  rightMotor->setVelocity(0);
  
  moving = true;
  target = 0;
  
  
  setMotors(-1,1);
  double modu;
 while (robot->step(timeStep) != -1) {
    updateGyro(timeStep);
    modu = fmod(getAngle(), 360);
    printf("%f \n", modu);
  } 
 

   /*double modu;
 while (robot->step(timeStep) != -1) {
    updateGyro(timeStep);
    modu = fmod(getAngle(), 360);
    printf("%f \n", modu);
  }
  */
  
  
  /*while (robot->step(timeStep) != -1) {
    if (dsn->getValue() >= 2000) {
    
      leftMotor->setVelocity(0);
      rightMotor->setVelocity(0);
      break;
     }
  }
  */
  
 /* bool turn = false;
  while (robot->step(timeStep) != -1) {
    updateGyro(timeStep);
    
    if (turn == false) {
      turnR();
      turn = true;
      break;
    }

  }
  /*
  
  
   /*while (robot->step(timeStep) != -1) {
     turnR();
  }
  /*
  
  
  
  
  
  
   
  //Wall Turn
  /*
  while (robot->step(timeStep) != -1) {
    
    if (doneTurn() == false) {
       continue;
    }
     
    if (doneForward() == false) {
      continue;
    }
    
    if (dsf->getValue() < 2000) {
      setTurn(90);
    }
    else {
      setForward(GRID_SIZE);
      
    }
  }
  */




  delete robot;
  return 0;
}
