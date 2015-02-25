#include "bleb.h"

polarPoint cartesianToPolar1(Point &ctr, Point2f &pt){
    polarPoint p;
    p.r        = sqrt((ctr.x-pt.x)*(ctr.x-pt.x) + (ctr.y-pt.y)*(ctr.y-pt.y));
    double x   = pt.x - ctr.x;
    double y   = pt.y - ctr.y;
    p.theta    = atan2(y, x);
    return p;
}

Bleb::Bleb(vector<Point> &bunch, Point2f &centroid, int &BIN)
{
    //bleb size
    size = bunch.size();

    //bleb center
    vector<Point> hull;
    convexHull(bunch, hull);
    Moments mu = moments(hull);
    Point2f blebCtr = Point2f(mu.m10/mu.m00, mu.m01/mu.m00);
    center.x = blebCtr.x;
    center.y = blebCtr.y;

    // bleb bunch in polar coordinates
    double theta = 0;
    for(unsigned int n = 0; n < bunch.size(); n++){
        polarPoint pt = cartesianToPolar1(bunch[n], centroid);
        bunch_polar.push_back(pt);
        theta += pt.theta;
    }
    theta /= bunch.size();
    bin = theta/(3.1415926*2/BIN)+BIN/2;


    // bleb bounding ellipse
    roughArea =  fitEllipse(Mat(bunch));

}

Bleb::Bleb()
{
    size = 0;
    center = Point(0, 0);
    bunch_polar.clear();
    roughArea = RotatedRect();
    bin = 0;
}

Bleb::~Bleb(){

}
