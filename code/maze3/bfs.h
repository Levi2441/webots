#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Gyro.hpp>
#include <webots/DistanceSensor.hpp>
#include <queue>
#include <stack>

using namespace webots;
using namespace std;

extern stack<int> pathStack;
extern int done;
extern DistanceSensor *distances[4];

void getSurroundings(int Section, int Orientation);
bool ifWall(int Section, int Orientation);
int getNewCell(int Section, int i);
char pOrientation(int i);
bool is_clear(int Section, int Orientation);
void BFS(int starting_node);
int getTurn(int Section, int Orientation, int goalCell);