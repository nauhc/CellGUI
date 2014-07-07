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
    x_start = x;
    y_start = y;

    frame = &img;
    Rect roi_rect = Rect(x, y, width, height);
    Mat sub = (*frame)(roi_rect);
    cv::cvtColor(sub, *roi, CV_RGB2GRAY); // convert color image to grayscale image
    //imwrite("../../../video/roigray.tiff", *roi);
}

void FindContour::getROI(const Mat &img, vector<Point> circle)
{
    frame = &img;
    Rect roi_rect = boundingRect(Mat(circle));
    Mat sub = (*frame)(roi_rect);
    cv::cvtColor(sub, *roi, CV_RGB2GRAY); // convert color image to grayscale image
}

void FindContour::edgeDetection(Mat &adapThreshImg){
//    int blockSize = 17;
//    double constValue = 7;
//    adaptiveThreshold(*roi, adapThreshImg, 255.0, CV_ADAPTIVE_THRESH_MEAN_C,
//                      CV_THRESH_BINARY_INV, blockSize, constValue);
    Mat roi_temp;
    adaptiveThreshold(*roi, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                      CV_THRESH_BINARY_INV, blockSize, constValue);

    //remove noise from roi image
    //medianBlur(adapThreshImg, adapThreshImg, 5);

}



void FindContour::boundingBox(Mat &img){


    Mat thresholdImg;
    edgeDetection(thresholdImg);

    //cv::cvtColor(*currFrame, img, CV_RGB2GRAY);
    img = frame->clone();
    vector<Point> points;
    int min_x = thresholdImg.cols, max_x = 0;
    int min_y = thresholdImg.rows, max_y = 0;
    for (int j = 0; j < thresholdImg.rows; j++){
        for (int i = 0; i < thresholdImg.cols; i++){
            if(thresholdImg.at<uchar>(j, i) >= 128){
                points.push_back(Point(i, j));
                if(min_x>i) min_x = i;
                if(max_x<i) max_x = i;
                if(min_y>j) min_y = j;
                if(max_y<j) max_y = j;
            }
        }
    }
    int point_cnt = points.size();
    cout << point_cnt << " points" << endl;
    if(point_cnt > 30){
        int x1 = min_x+x_start;
        int y1 = min_y+y_start;
        int x2 = (max_x+x_start) > img.cols ? img.cols : (max_x+x_start);
        int y2 = (max_y+y_start) > img.rows ? img.rows : (max_y+y_start);
        Point x(x1, y1);
        Point y(x2, y2);
        Rect rect(x,y);
//        Scalar color(0,255,255); // draw a yellow rectangle on the image
//        rectangle(img, rect, color, 1);

    }




}

void FindContour::boundingBox(Mat &img, vector<Point> circle)
{
    img = frame->clone();
    Rect rect = boundingRect(Mat(circle));
    Scalar color(0,255,255); // draw a yellow rectangle on the image
    rectangle(img, rect, color, 1);
}
