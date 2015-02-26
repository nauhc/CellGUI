#ifndef BLEB_H
#define BLEB_H
#include <cv.h>
#include <cxcore.h>

using namespace std;
using namespace cv;

struct polarPoint{
    double r;
    double theta;
};

class Bleb
{
public:
    int size;
    Point center; // bleb center
    vector<polarPoint> bunch_polar;
    RotatedRect roughArea;
    int bin;

    Bleb(vector<Point> &bunch, Point2f &centroid, int &BIN);
    Bleb();
    ~Bleb();
};

#endif // BLEB_H
