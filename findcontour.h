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
    const Mat *frame;
    //Mat *roi;

    int blockSize;
    double constValue;

//    int x_start; // offset of roi rect - x
//    int y_start; // offset of roi rect - y
    Rect rect;

public:

    FindContour();
    ~FindContour();

    void setAdaptThresh(double para1);
    void setBlkSize(int para2);

//    void getROI(const Mat &img, int x, int y, int width, int height);
//    void edgeDetection(Mat &adapThreshImg); // detect the edges in the image and remove the noises
//    void boundingBox(Mat &img);

//    // get the region of interest by the updated circle
//    void getROI(const Mat &img, vector<Point> &circle, Mat &mask);
//    // detect the edges in the image and remove the noises
//    void edgeDetection(Mat &adapThreshImg, Mat &mask, vector<Point> &circle);

    // getROI+edgeDetection
    void cellDetection(const Mat &img, vector<Point> &circle, Mat &adapThreshImg);
    // draw a bounding box of roi rect
    void boundingBox(Mat &img/*, vector<Point> &circle*/);
};

#endif // FINDCONTOUR_H
