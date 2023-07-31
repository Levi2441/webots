#include <webots/Motor.hpp>
#include <webots/Robot.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Gyro.hpp>
#include <queue>
#include <stack>

using namespace webots;
using namespace std;

#include "bfs.h"

struct cell {
    unsigned int N: 1;
    unsigned int E: 1;
    unsigned int S: 1;
    unsigned int W: 1;
    unsigned int V: 1; //Victim
    unsigned int T: 1; //Traverse
    unsigned int filler: 2; //Filler
};


#define GRID_SIZE 0.5

#define row 5
#define column 5
#define MAZE_SIZE row*column*4


extern struct cell maze[MAZE_SIZE];

void getSurroundings(int Section, int Orientation) {
  if (maze[Section].T == 0) {
    maze[Section].T = 1;
    for (int i = 0; i < 4; i++) {
    if (distances[i]->getValue() < 2000) {
      if (((i + Orientation) % 4) == 0)
        maze[Section].N = 1;
      else if (((i + Orientation) % 4) == 1)
        maze[Section].E = 1;
      else if (((i + Orientation) % 4) == 2)
        maze[Section].S = 1;
      else 
        maze[Section].W = 1;
      }
  }
}
}
bool ifWall(int Section, int Orientation) {
  if (Orientation == 0) {
    if (maze[Section].N == 1)
      return true;
    else 
      return false;
  }
  else if (Orientation == 1) {
    if (maze[Section].E == 1)
      return true;
    else 
      return false;
  }
  else if (Orientation == 2) {
    if (maze[Section].S == 1)
      return true;
    else
      return false;
  }
  else {
    if (maze[Section].W == 1)
      return true;
     else
       return false;
  }
  return false;
}
int getNewCell(int Section, int i) {
  if (i == 0)
      return Section - column;
   else if (i == 1)
       return Section + 1;
   else if (i == 2)
        return Section + column;
    else
        return Section - 1;
}
char pOrientation(int i) {
  if (i  == 0) 
    return 'N';
  else if (i == 1)
    return 'E';
  else if (i == 2)
    return 'S';
  else
    return 'W';
}

bool is_clear(int Section, int Orientation) {
  if (distances[0]->getValue() < 2000)
    return false;
  if (maze[getNewCell(Section, Orientation)].T == 1)
    return false;
  
  return true;
}
void BFS(int starting_node)
{
    queue<int> myqueue;
    int currentNode;
    int pc[MAZE_SIZE];
    for (int i = 0; i < MAZE_SIZE; i++)
        pc[i] = -1;
    int goal = -1;
    int possible[4];
    
    pc[starting_node] = starting_node;
       
    myqueue.push(starting_node);
    
   while(myqueue.empty() != true) {
       currentNode = myqueue.front();
       
       //printf("Current Node: %d\n", currentNode);
       //get index adjacent, traversible tiles
       for (int i = 0; i < 4; i++) {
           possible[i] = getNewCell(currentNode, i);
          //printf("%d ", possible[i]);
       }
//       printf("\n");
       
       //check if they are traversible
       for (int i = 0; i < 4; i++) {
           if (ifWall(currentNode, i) == true ) {
               possible[i] = -1;
           }
       }
       //check if they already have parents
       for (int i = 0; i < 4; i++) {
           if (possible[i] != -1) {
               if (pc[possible[i]] != -1)
                   possible[i] = -1;
           }
       }
       for (int i = 0; i < 4; i++) {
          // printf("%d ", possible[i]);
       }
       
      // printf("\n");
       //enqueu remaining children
       for (int i = 0; i < 4; i++) {
           if (possible[i] != -1)
               myqueue.push(possible[i]);
       }
       myqueue.pop();
       
       //update pc
       for (int i = 0; i < 4; i++) {
           if (possible[i] != -1)
               pc[possible[i]] = currentNode;
       }

       //this checking shoudl go before check if they are traversible
       for (int i = 0; i < 4; i++) {
           if (possible[i] != -1) {
               if (maze[possible[i]].T == 0) {
                   goal = possible[i];
                 // printf("goal: %d \n", goal);
               }
           }
       }
       if (goal != -1) {
           //printf("goal found\n");
           break;
        }
        /*
        else  {
          printf("goal not found, searching children: ");
        }
        
        for (int i = 0; i < 4; i++) {
          if (possible[i] != -1)
            printf("%d ", possible[i]);
        }
        printf("\n");
        for (int i = 0; i < MAZE_SIZE; i++)
          printf("%d : %d\n", i, pc[i]);
       */    
       
   }
   // printf("done\n");
   //find the path to break
   
   if (goal == -1 && myqueue.empty() == true) {
         
         printf("No More Tiles\n");
         done = 1;
         return;
        } 
   
     
    int path = pc[goal];
    pathStack.push(goal);
    while (path != starting_node) {
        pathStack.push(path);
        path = pc[path];
    }
    
    printf("path: ");
    stack<int> display = pathStack;
    while (display.empty() == false) {
      printf("%d ", display.top());
      display.pop();
    }
   // printf("\n");
    
}
int getTurn(int Section, int Orientation, int goalCell) {
  int relation; //which directino do you want the robot to face
  if (Section + 1 == goalCell) 
    relation = 1; //go east
  else if (Section - column == goalCell) {
    relation = 0; //go nort
  }
  else if (Section - 1 == goalCell) {
    relation = 3; //west
  }
  else {
    relation = 2; //south
  }
  
  //determine which turn to take
  if (relation == Orientation)
    return 0; //straight
  else if (abs(relation-Orientation) == 2)
    return 2; //180
  else if (((relation + 1) % 4) == Orientation)
    return 3; //90 left;
  else
    return 1; //90 right
}

