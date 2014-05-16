#include "findcontour.h"

FindContour::FindContour(): frame(new Mat()), roi(new Mat()){
}

FindContour::~FindContour(){
    delete frame;
    delete roi;
}

void FindContour::setAdaptThresh(double para1){

    constValue = para1;
}

void FindContour::setBlkSize(int para2){
    blockSize = para2;
}

void FindContour::getROI(const Mat &img, int x, int y, int width, int height){
    frame = &img;
    Rect roi_rect = Rect(x, y, width, height);
    Mat sub = (*frame)(roi_rect);
    cv::cvtColor(sub, *roi, CV_RGB2GRAY); // convert color image to grayscale image
    //imwrite("../../../video/roigray.tiff", *roi);
}

void FindContour::edgeDetection(Mat &adapThreshImg){
//    int blockSize = 17;
//    double constValue = 7;
//    adaptiveThreshold(*roi, adapThreshImg, 255.0, CV_ADAPTIVE_THRESH_MEAN_C,
//                      CV_THRESH_BINARY_INV, blockSize, constValue);
    adaptiveThreshold(*roi, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                      CV_THRESH_BINARY_INV, blockSize, constValue);

}
