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
    Mat *roi;
    int blockSize;
    double constValue;

    int x_start;
    int y_start;

public:

    FindContour();
    ~FindContour();

    void setAdaptThresh(double para1);
    void setBlkSize(int para2);
    void getROI(const Mat &img, int x, int y, int width, int height);
    void getROI(const Mat &img, vector<Point> circle);
    void edgeDetection(Mat &adapThreshImg); // detect the edges in the image and remove the noises
    void boundingBox(Mat &img);
    void boundingBox(Mat &img, vector<Point> circle);
};

#endif // FINDCONTOUR_H
