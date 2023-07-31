#include <opencv2/opencv.hpp>
using namespace cv;

#include <webots/Robot.hpp>
#include <webots/Camera.hpp>
#include <webots/LightSensor.hpp>

using namespace webots;
using namespace std;

#include "opencv1.h"


char letter_cam(void) { //returns victim U,H,S or N for no visual victim
  
  const int width = cam->getWidth(); //cam width
  const int height = cam->getHeight(); //cam height
  
  RNG rng;
  
  bool found;
  
    char vict = 'N'; //N=None, ony here for checking/debugging
    int contCount[3] = {0};
    found = false;
    /*
     * Convert the camera image into an openCV Mat. You must pass in the height
     * and width of the camera in pixels, as well as specify CV_8UC4, meaning that
     * it is in RGBA format.
     */
    
    //frame is 128*128
    Mat frame( height, width, CV_8UC4, (void*)cam->getImage());
    Mat drawing = frame.clone();
    //imshow("frame", frame);
    
    cvtColor(frame,frame,COLOR_BGR2GRAY); //grayscale
    //imshow("grayScale", frame);
    
    GaussianBlur(frame, frame, cv::Size(3,3), 0 ,0); //Gaussian Blur
   // imshow("Gaussian Blur", frame);
    
    threshold(frame,frame,80,255,THRESH_BINARY); //threshold
    //imshow("thresh", frame);
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Rect> boundRect( contours.size() );
    
    findContours( frame, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );
    
    for( size_t i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( 255, 0, 0 );
        drawContours( drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0 );
    }
    imshow("all contours",drawing);
    cout << "All contours: " << contours.size() << endl;//check how many contours there are
    
    Rect ROI;
    for(int i=0;i<contours.size();i++)
    {
      ROI = boundingRect(contours[i]);
      //set thresholds for bounding box creation here
      if(ROI.height<=height/2 && ROI.width<=width/2 && ROI.height>=height/4 && ROI.width>=width/4)
      {
         found = true;
         break;
      }
    }
    if(found) // this checks if a letter was found in the frame
    {
      Mat cropped = frame(ROI);
      bitwise_not ( cropped, cropped );
      imshow("bounding box", cropped);
      
      cout << "Cropped : " << "Rows: " << cropped.rows << " Cols: " << cropped.cols <<endl;
      
      Rect ROI1(0, 0, cropped.cols, cropped.rows/3);
      Mat cropped1 = cropped(ROI1);
      findContours( cropped1, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );
      cout << contours.size() << endl;
      contCount[0] = contours.size();
      imshow("contours top",cropped1);
      
      Rect ROI2(0, cropped.rows/3.0, cropped.cols, cropped.rows/3);
      Mat cropped2 = cropped(ROI2);
      findContours( cropped2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );
      cout << contours.size() << endl; 
      contCount[1] = contours.size();
      imshow("contours middle",cropped2);
      
      Rect ROI3(0, 2*cropped.rows/3.0, cropped.cols, cropped.rows/3);
      Mat cropped3 = cropped(ROI3);
      findContours( cropped3, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );
      cout << contours.size() << endl;
      contCount[2] = contours.size();
      imshow("contours bottom",cropped3); 
      
      if(contCount[0]==2)
      {
        if(contCount[2] == 1)
          vict = 'U';
        else
          vict = 'H';
      }
      else
      {
        vict = 'S';
      }
    
      return vict;
    }
   return 'N';
}
int heat_cam(void) {
  int left = leftHeat->getValue();
  int right = rightHeat->getValue();
  
  if (left > 30 || right > 30)
    return 1;
  else
    return 0;
}