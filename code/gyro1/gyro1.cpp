#include <iostream>
#include <Math.h>
using namespace std;

// Webots include statements 
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/Gyro.hpp>
#include <webots/PositionSensor.hpp>
using namespace webots;

// Instantiate new Robot object
Robot* robot = new Robot();

// Gets pointers to the motor and sensors from the robot
Motor* leftMotor = robot->getMotor("left wheel motor"), *rightMotor = robot->getMotor("right wheel motor");
Gyro* gyro = robot->getGyro("gyro");
PositionSensor* leftEnc = leftMotor->getPositionSensor(), *rightEnc = rightMotor->getPositionSensor();

// Get the max velocity the motors are allowed to go at, in rad/s.
const int MAX_VELOCITY = leftMotor->getMaxVelocity();

/*
 * Number of radians the motors must turn to move the robot forward 1 meter.
 * This is found by taking 100cm (1m) and dividing it by pi * 19cm, the 
 * cirumference of the wheel of the pioneer3DX robot found at
 * https://cyberbotics.com/doc/guide/pioneer-3dx which yields wheel revolutions per meter.
 * Then multiply by 2pi to convert to radians per meter.
 */
const double radPerMeter = 10.5;

/* Variable declaration */

// Variable "angle" stores our current angle heading. "initAngle" is used for keeping track of how
// much we have turned.
double angle = 0, initAngle;

// Use an enum called "state" in order to store our state. This state alternates between moving forward
// and turning.
enum State { movingForward, turn } state;

// numEdgesTravel keeps track of how many sides of the box we have currently traveled. Once numEdgesTravel reaches 4,
// then we know that we have completely gone around in a square pattern. targetEnc stores our desired encoder value
// when we want to move forward a certain amount of meters.
int numEdgesTravel = 0, targetEnc = 0;

/*
 * Webots gyros return angular velocity along the 3 axes. To find angle, we need to integrate
 * angular velocity by multiplying by the timeStep. We only care about rotation around the y-axis AKA yaw.
 * The result is stored in the variable "angle" in radians.
 */
void updateGyro(int timeStep) {
    angle += (timeStep / 1000.0) * (gyro->getValues())[1];
}

// Converts the current angle from radians into degrees.
// 180 degrees = pi radians
double getAngle() {
    return angle * 180 / 3.1415;
}

// Makes the robot go forward for a setpoint of "dist" meters.
void goForward(double dist) {

	// Sets the robot state to moving forward
    state = movingForward;

	// Calculates the target encoder value by converting "dist" into radians
	// and adding it onto the current encoder value.
    targetEnc = leftEnc->getValue() + radPerMeter * dist;

	// Sets the left and right motors to move forward at half the max velocity
    leftMotor->setVelocity(MAX_VELOCITY * .5);
    rightMotor->setVelocity(MAX_VELOCITY * .5);
}

// Makes the robot do a clockwise 90 degrees point turn
void doTurn() {

	// Sets the robot state to turning
    state = turn;

	// Updates initAngle to be the current angle (in degrees).
    initAngle = getAngle();

	// Sets the left motor to move forward at 10% of max velocity.
	// Sets the right motor to move backward at 10% of max velocity.
    leftMotor->setVelocity(MAX_VELOCITY * .1);
    rightMotor->setVelocity(MAX_VELOCITY * -.1);
}

// Function that checks if the robot is in the process of moving forward.
// true -> still moving forward
// false -> robot has finished
bool ifForward() {

	// If the current encoder value is within .1 radians of the target encoder value
    if (abs(leftEnc->getValue() - targetEnc) < .1) {

		// Stop the robot and return false
        leftMotor->setVelocity(0);
        rightMotor->setVelocity(0);
        return false;
    }

	// Otherwise, return true
    return true;
}


bool ifTurn() {

	
    if (abs(getAngle() - initAngle) >= 860.0) {
		
		// Stop the robot and return false
        leftMotor->setVelocity(0);
        rightMotor->setVelocity(0);
        return false;
    }

	// Otherwise, return true
    return true;
}


int main() {
 
    leftMotor->setPosition(INFINITY);
    rightMotor->setPosition(INFINITY);

    // setVelocity sets the speed in rad/s.  0 means stop.
    leftMotor->setVelocity(0);
    rightMotor->setVelocity(0);

    // Retrieves the basic world time step and stores it in variable "timeStep". 
    int timeStep = (int)robot->getBasicTimeStep();

    // we enable the gyro sensor and the encoders
    // we need to pass the timeStep as the refresh rate of the sensor
    gyro->enable(timeStep);
    leftEnc->enable(timeStep);
    rightEnc->enable(timeStep);

    /* End of Initialization */

    /* Loop - where we do work*/

    // begin moving forward 2 meters 
    goForward(2.0);

	
    while (robot->step(timeStep) != -1) {

        // Update the angle heading by integrating the gyro's current reading
        updateGyro(timeStep);

        // If the robot state is set to moving forward, and ifForward() tells us that the robot has finished
		// moving forward, then transition into turning 90 degrees.
        if (state == movingForward && !ifForward()) {
            printf("turning\n");
			// Switch to turning
            doTurn();
        }
		// Otherwise, check if the state is set to turning and ifTurn() tells us that the robot has finished turning.
        else if (state == turn && !ifTurn()) {

			// If numEdgesTravel is less than 3, meaning that we have not gone fully around in a box yet, then
			// increment numEdgesTravel and start moving forward 2 meters again.
            if (numEdgesTravel < 3) {
                printf("forward\n");
                numEdgesTravel++;
                goForward(2.0);
            }
        }
    }

    return 0;
}