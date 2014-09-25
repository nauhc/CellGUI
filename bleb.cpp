#include "bleb.h"

polarPoint cartesianToPolar1(Point &ctr, Point &pt){
    polarPoint p;
    p.r        = sqrt((ctr.x-pt.x)*(ctr.x-pt.x) + (ctr.y-pt.y)*(ctr.y-pt.y));
    double x   = pt.x - ctr.x;
    double y   = pt.y - ctr.y;
    p.theta    = atan2(y, x);
    return p;
}

Bleb::Bleb(vector<Point> &bunch)
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

    // bleb area in polar coordinates
    for(unsigned int n = 0; n < bunch.size(); n++){
        area.push_back(cartesianToPolar1(bunch[n], center));
    }

    // bleb bounding ellipse
    roughArea =  fitEllipse(Mat(bunch));

}

Bleb::~Bleb(){

}
