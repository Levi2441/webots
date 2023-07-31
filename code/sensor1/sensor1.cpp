// File:          sensor1.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <webots/Gyro.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// All the webots classes are defined in the "webots" namespace
using namespace webots;

const double rPM = 50; //should be 40
const double rPD = 0.0419; //should be 0.044 -- used to be 0.0738

Robot *robot = new Robot();
 
Motor *leftMotor = robot->getMotor("left wheel motor");
Motor *rightMotor = robot->getMotor("right wheel motor");
 
void movement(int meters) {
  double fr = (double)meters * rPM;
  leftMotor->setPosition(fr);
  rightMotor->setPosition(fr);
  
  leftMotor->setVelocity(5);
  rightMotor->setVelocity(5);
}
void turnL(int degrees) {
  double t = degrees * rPD;
  leftMotor->setPosition(-t);
  rightMotor->setPosition(t);
  
  leftMotor->setVelocity(5);
  rightMotor->setVelocity(5);
}
int main(int argc, char **argv) {
  // create the Robot instance.
  
  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();
  
  DistanceSensor *ds1 = robot->getDistanceSensor("dsw");
  ds1->enable(timeStep);
  

  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
  //  Motor *motor = robot->getMotor("motorname");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);

  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  //x is physical distance -- y is perceived sensor reading
  
  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);
  
  leftMotor->setVelocity(4);
  rightMotor->setVelocity(4);
 
  
  while (robot->step(timeStep) != -1) {
    printf("%f\n", ds1->getValue());
    
    if (ds1->getValue() < 1500) {
      printf("hi\n");
      turnL(90);
      break;
    }
    else { 
      leftMotor->setVelocity(2);
      rightMotor->setVelocity(2);
    }
    
  }
  
  /*
    leftMotor->setPosition(INFINITY);
    rightMotor->setPosition(INFINITY);
    
    while (robot->step(timeStep) != -1) {
      leftMotor->setVelocity(4);
      rightMotor->setVelocity(4);
    }
    
   */
      
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();

    // Process sensor data here.

    // Enter here functions to send actuator commands, like:
    //  motor->setPosition(10.0);
  
      
  
  

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}
