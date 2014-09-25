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
    Point center;
    vector<polarPoint> area;
    RotatedRect roughArea;

    Bleb(vector<Point> &bunch);
    ~Bleb();
};

#endif // BLEB_H
