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

public:

    FindContour();
    ~FindContour();
    void setAdaptThresh(double para1);
    void setBlkSize(int para2);
    void getROI(const Mat &img, int x, int y, int width, int height);
    void edgeDetection(Mat &adapThreshImg);
};

#endif // FINDCONTOUR_H
