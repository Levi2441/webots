
#include <webots/Robot.hpp>
#include <webots/Camera.hpp>
#include <webots/Motor.hpp>

// All the webots classes are defined in the "webots" namespace
using namespace webots;

int main(int argc, char **argv) {
  // create the Robot instance.
  Robot *robot = new Robot();
   Motor *leftMotor = robot->getMotor("left wheel motor"); 
  Motor *rightMotor = robot->getMotor("right wheel motor");
  
  Camera *colorCam = robot->getCamera("camera");
  
  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);
  
  leftMotor->setVelocity(1);
  rightMotor->setVelocity(1);

  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();
  colorCam->enable(timeStep);
  
  const unsigned char* img = colorCam->getImage();
  
  printf("got here\n");
  printf("%d\n", (int)(img[0]));

  while (robot->step(timeStep) != -1) {
    printf("%d\n", (int)(img[0]));
    break;
  };

  delete robot;
  return 0;
}
