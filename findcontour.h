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
    double scale;

public:

    FindContour();
    ~FindContour();

    void setAdaptThresh(double para1);
    void setBlkSize(int para2);

    void setScale(double scl);

    // getROI+edgeDetection
    void cellDetection(const Mat &img, vector<Point> &circle,
                       Mat &dispImg1, Mat &dispImg2,
                       int &area, int &perimeter, Point2f &ctroid);

    // draw a bounding box of roi rect
    void boundingBox(Mat &img/*, vector<Point> &circle*/);

};

#endif // FINDCONTOUR_H
