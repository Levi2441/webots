#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Gyro.hpp>
#include <webots/GPS.hpp>
#include <webots/LightSensor.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Camera.hpp>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <queue>
#include <stack>

#include "mazemotors1.h" 
#include "bfs.h"
#include "opencv1.h"

#include <opencv2/opencv.hpp>

using namespace webots;
using namespace std;

#define GRID_SIZE 0.5

#define row 5
#define column 5
#define MAZE_SIZE row*column*4

enum State { forward, turn} state;
enum Facing { North, South, East, West} dir;

Robot *robot = new Robot();
Motor *leftMotor = robot->getMotor("left wheel motor"); 
Motor *rightMotor = robot->getMotor("right wheel motor");

PositionSensor *leftEncoder = leftMotor->getPositionSensor();
 PositionSensor *rightEncoder = rightMotor->getPositionSensor();

Gyro *gyro = robot->getGyro("gyro");

Camera *cam = robot->getCamera("camera");

LightSensor* rightHeat = robot->getLightSensor("right_heat_sensor");
LightSensor* leftHeat = robot->getLightSensor("left_heat_sensor");

int victim_locations[row * column * 4];

struct cell {
    unsigned int N: 1;
    unsigned int E: 1;
    unsigned int S: 1;
    unsigned int W: 1;
    unsigned int V: 1; //Victim
    unsigned int T: 1; //Traverse
    unsigned int filler: 2; //Filler
};

struct cell maze[MAZE_SIZE];

double angle = 0;
 const double rPM = 50; //should be 40
 const double rPD = 0.02485; //should be 0.044 -- used to be 0.02485

double targetL = 0;
double targetR = 0;
double targetG = 0;

void display_cell(struct cell a) {
    printf("%d%d%d%d%d%d%d \n", a.filler, a.T, a.V, a.W, a.S, a.E, a.N );

}

DistanceSensor *distances[4];
double ds[4];
void getShortDist() {
  for (int i = 0; i < 4; i++) {
  ds[i] = distances[i]->getValue();
  }
    
}


stack<int> pathStack;
int done = 0;

int straight(int Section, int Orientation) {
  /*printf("Direction: %c \n", pOrientation(Orientation));
  getSurroundings(Section, Orientation);
  printf("New cell: %d : ", Section);
  display_cell(maze[Section]);
  */
  
  //change curCell
   Section = getNewCell(Section, Orientation);
   //move forward
   setForward(GRID_SIZE);
   
   return Section;
}

int main(int argc, char **argv) {
  // create the Robot instance.

  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();
  
   char buf[4];
  for(int i = 0; i < 4; i++) {
    if (i == 0) snprintf(buf, 4, "dsn");
    else if (i == 1) snprintf(buf, 4, "dse");
    else if (i == 2) snprintf(buf, 4, "dss");
    else  snprintf(buf, 4, "dsw");
    
    distances[i] = robot->getDistanceSensor(buf);
    distances[i]->enable(timeStep);  //lookup table: 0-0.5m, 0-2000
  }
  

  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);
  
  leftMotor->setVelocity(0);
  rightMotor->setVelocity(0);
      
  leftEncoder->enable(timeStep);
  rightEncoder->enable(timeStep);
  
  cam->enable(timeStep);
 

  int startCell = 24;
  int Orientation = 0;
  int curCell = startCell;
  int goalCell = 0;
  int process = 1; //tracks turn + forward //change to 0 to include visual
  int control = 0; //tracks state  
  int goBack = 0;
  int vturn = 0; //keeps track of victim turning
    
  while (robot->step(timeStep) != -1) { 
    //printf("rep\n"); 
        
      if (done == 1) {
       printf("done searching\n");
        if (goBack == 0) {
          maze[startCell].T = 0;
          done = 0;
          goBack++;
         printf("going back to original");
          continue;
        }
        else {
         printf("back at starting pos\n");
          break;
        }
      }

      if (doneTurnR() == false && control == 1) {
        leftMotor->setVelocity(1);
        rightMotor->setVelocity(-1);
        continue;
      }
      

      if (doneTurnL() == false && control == 3) {
        leftMotor->setVelocity(-1);
        rightMotor->setVelocity(1);
        continue;
      }
      
      if (doneForward() == false && control == 0) {
        leftMotor->setVelocity(1);
        rightMotor->setVelocity(1);
        continue;
      }
      if (pathStack.empty() == true) {
         printf("Direction: %c \n", pOrientation(Orientation));
          getSurroundings(curCell, Orientation);
         printf("New cell: %d : ", curCell);
          display_cell(maze[curCell]);
                   
        BFS(curCell);
        
       if (done == 1 && goBack == 0) { //when done intially, and go back to beginning
         continue;
       }
       if (done == 1 && goBack == 1) {
         continue;
       }
       
        
       goalCell = pathStack.top();
      printf("goal cell: %d\n", goalCell);
       
       continue;
      
      }
      else { 
        if (process == 0) { //checking for visual victim
          printf("---------------------------\nChecking For Victim\n--------------------\n");
          
          if (letter_cam() == 'U' || letter_cam() == 'H' || letter_cam() || 'S') {
            printf("Victim Found\n");
            victim_locations[curCell] = 1;
           }
          
          setTurnR(90);
          control = 1;
          vturn++;
          
          if (vturn == 4) {
            //reset
            process++;
            vturn = 0;
          }
        }
        
        
        if (process == 1) { //determining the degree of turn
          printf("Stage 1: \n");
         printf("turn: %d \n", getTurn(curCell, Orientation, goalCell));

          if (getTurn(curCell, Orientation, goalCell) == 0) {
            process++;
          }
          else if (getTurn(curCell, Orientation, goalCell) == 1) {
            Orientation = (Orientation + 1) % 4;
            setTurnR(90);
            process++;
            control = 1;
          }
          else if (getTurn(curCell, Orientation, goalCell) == 3) {
            if (Orientation == 0) {
              Orientation = 3;
            }
            else  {
              Orientation--;
            }
            setTurnL(90);
            process++;
            control = 3;
          }
          else { //if it's 180, rotate robot 90, update settings, reveluate (should come up to do another 90)
            Orientation = (Orientation + 1) % 4;
            //setReverseTurn();
            setTurnR(90);
            control = 1;
          }
        }
        else if (process == 2) { //going forward after orienting robot
         printf("Stage 2: \n");
          printf("currCell: %d, Orientation: %d\n", curCell, Orientation);
          curCell = straight(curCell, Orientation);
          process++;
          control = 0;
        }
        
        if (process == 3) {
          printf("Stage 3: \n");
          pathStack.pop();
          if (pathStack.empty() == false) {
            goalCell = pathStack.top();
          }
          else {
            goalCell = -1;
          }
          
          if (pathStack.empty() == true) {
           printf("!Empty Stack!\n");
          }
           
           process = 1; //change this to 0 to include visual
          printf("curCell: %d Orientation: %d Goal Cell: %d\n", curCell, Orientation, goalCell);
           printf("----------------------------------\n");
           continue;
        }
        
      }
          
  }
  printf("I'm FREE!\n");
  
  
   //lookup table: 0-0.5m, 0-2000
  //getSurroundings(0,0); //remmeber to set orietation
  //display_cell(maze[0]);
    
  delete robot;
  return 0;
}
