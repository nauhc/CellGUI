#ifndef FINDCONTOUR_H
#define FINDCONTOUR_H
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class FindContour
{
private:
    const Mat *currFrame;
//    const Mat *prevFrame;
//    const Mat *nextFrame;
    Mat *currROI;
    Mat *prevROI;
    Mat *nextROI;

    int x_start;
    int x_stop;
    int y_start;
    int y_stop;

    // adaptive thresholding parameters
    int blockSize;
    double constValue;

public:

    FindContour();
    ~FindContour();

    // functions for setting up threshold for adaptive thresholding parameters
    void setAdaptThresh(double para1);
    void setBlkSize(int para2);
    // get region of interest
    void getROI(const Mat &img, int x, int y, int width, int height);
    void getInitialROI(const Mat &currImg,
                       const Mat &prevImg,
                       const Mat &nextImg,
                       int x, int y, int width, int height);
    // detect motion in given (initial) area and update roi for each frame
    void traceMotionROI();
    // edge detection - adaptive thresholding for roi
    void edgeDetection(Mat &adapThreshImg);
    void boundingBox(Mat &img);
};

#endif // FINDCONTOUR_H
