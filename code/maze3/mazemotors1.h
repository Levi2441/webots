#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Gyro.hpp>

using namespace webots;


extern const double rPM; //should be 40
extern const double rPD; //should be 0.044 -- used to be 0.0738
extern double angle;

extern double targetL;
extern double targetR;
extern double targetG;

extern Robot *robot;
extern Motor *leftMotor; 
extern Motor *rightMotor;

extern PositionSensor *leftEncoder;
extern PositionSensor *rightEncoder;

extern Gyro *gyro;

extern int timeStep;


void setMotors(double lef, double rig);
void setTurnR(int angle);
bool doneTurnR();
void setTurnL(int angle);
bool doneTurnL();
void setReverseTurn();
bool doneReverseTurn();
void setForward(double meters);
bool doneForward();
void updateGyro(int timeStep) ;
double getAngle();
void setGyroR();
bool doneGyroR(int timeStep);

