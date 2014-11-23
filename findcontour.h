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
    int dilSize;
    double blebSizeRatio;

//    int x_start; // offset of roi rect - x
//    int y_start; // offset of roi rect - y
    Rect rect;
    double scale;

public:

    FindContour();
    ~FindContour();

    void setAdaptThresh(double para1);
    void setBlkSize(int para2);
    void setDilSize(int var);
    void setblebSizeRatio(int var);

    void setScale(double scl);

    void binaryImage(const Mat &img, Mat &binaryImg);

    // getROI+edgeDetection
    void cellDetection(const Mat &img, vector<Point> &cir_org,
                       Mat &dispImg1, Mat &dispImg2,
                       vector<Point2f> &points1, vector<Point2f> &points2,
                       int &area,
                       int &perimeter,
                       Point2f &ctroid,
                       float &shape,
//                       vector<int> &blebs,
                       int &frameNum);

    void singleCellDetection(const Mat &img, vector<Point> &cir_org,
                             Mat &dispImg1, Mat &dispImg2,
                             int &area,
                             int &perimeter,
                             Point2f &ctroid,
                             float &shape,
                             Mat &cell_alpha,
                             Mat &blebsImg,
                             Rect &rectangle,
//                             vector<int> &blebs,
                             int &frameNum);

    // draw a bounding box of roi rect
    void boundingBox(Mat &img/*, vector<Point> &circle*/);

};

#endif // FINDCONTOUR_H
