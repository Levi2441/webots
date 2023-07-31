#include <webots/Robot.hpp>
#include <webots/Camera.hpp>

using namespace webots;
using namespace std;

extern Robot *robot;
extern Camera *cam;

extern LightSensor *leftHeat;
extern LightSensor *rightHeat;

extern int height;
extern int width;

char letter_cam(void);
int heat_cam(void);

