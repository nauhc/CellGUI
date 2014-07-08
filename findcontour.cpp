#include "findcontour.h"

FindContour::FindContour(): frame(new Mat())/*, roi(new Mat())*/{
}

FindContour::~FindContour(){
    delete frame;
//    delete roi;
}

void FindContour::setAdaptThresh(double para1){
    constValue = para1;
}

void FindContour::setBlkSize(int para2){
    blockSize = para2;
}


/*
//void FindContour::getROI(const Mat &img, vector<Point> &circle, Mat &mask)
//{
//    frame = &img;
//    roi_rect = boundingRect(Mat(circle));

//    Mat sub; // the rectangle region of ROI
//    cv::cvtColor((*frame)(roi_rect), sub, CV_RGB2GRAY);

//    //filter the image inside the circle
//    vector<Point> circle_ROI;
//    for (unsigned int i = 0; i < circle.size(); i++){
//        Point p = Point(circle[i].x - roi_rect.x, circle[i].y - roi_rect.y);
//        circle_ROI.push_back(p);
//    }
//    mask = Mat::zeros(sub.rows, sub.cols, CV_8UC1);
//    fillConvexPoly(mask, circle_ROI, Scalar(255));
//    *roi = sub.clone();
//    //convert color image to grayscale image
//    //cv::cvtColor(sub_filter, *roi, CV_RGB2GRAY);
//}

//void FindContour::edgeDetection(Mat &adapThreshImg, Mat &mask, vector<Point> &circle)
//{
//    //adaptive threshold method to detect edges in the roi
//    adaptiveThreshold(*roi, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
//                          CV_THRESH_BINARY_INV, blockSize, constValue);

//    vector<Point> whiteArea;
//    //let roi only display the region inside the circle
//    for(int j = 0; j < roi->rows; j++){
//        for(int i = 0; i < roi->cols; i++){
//            if(mask.at<uchar>(j,i) == 0)
//                adapThreshImg.at<uchar>(j,i) = 0;
//            if(adapThreshImg.at<uchar>(j,i) != 0)
//                whiteArea.push_back(Point(i,j));
//        }
//    }

//    //renew circle
//    vector<Point> convHull;
//    convexHull(whiteArea, convHull);

//    circle.clear();
//    for(unsigned int i = 0; i < convHull.size(); i++)
//        circle.push_back(Point(convHull[i].x + roi_rect.x, convHull[i].y + roi_rect.y));


//    //Rect rect_temp = boundingRect(Mat(convHull));
//    //rectangle(adapThreshImg, rect_temp, Scalar(128), 1);

//}*/

// get ROI + edgeDectection
void FindContour::cellDetection(const Mat &img, vector<Point> &circle, Mat &adapThreshImg){
    frame = &img;
    rect = boundingRect(Mat(circle));

    Mat sub; // the rectangle region of ROI
    cv::cvtColor((*frame)(rect), sub, CV_RGB2GRAY);

    //filter the image inside the circle
    vector<Point> circle_ROI;
    for (unsigned int i = 0; i < circle.size(); i++){
        Point p = Point(circle[i].x - rect.x, circle[i].y - rect.y);
        circle_ROI.push_back(p);
    }

    //mask for filtering out the cell of interest
    Mat mask = Mat::zeros(sub.rows, sub.cols, CV_8UC1);
    fillConvexPoly(mask, circle_ROI, Scalar(255));

    //image edge detection for the sub region (roi rect)
    adaptiveThreshold(sub, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                          CV_THRESH_BINARY_INV, blockSize, constValue);

    vector<Point> whiteArea;
    //let roi only display the region inside the circle
    for(int j = 0; j < sub.rows; j++){
        for(int i = 0; i < sub.cols; i++){
            if(mask.at<uchar>(j,i) == 0)
                adapThreshImg.at<uchar>(j,i) = 0;
            if(adapThreshImg.at<uchar>(j,i) != 0)
                whiteArea.push_back(Point(i,j));
        }
    }

    //renew circle
    vector<Point> convHull;
    convexHull(whiteArea, convHull);
    circle.clear();
    for(unsigned int i = 0; i < convHull.size(); i++)
        circle.push_back(Point(convHull[i].x + rect.x, convHull[i].y + rect.y));

}


void FindContour::boundingBox(Mat &img)
{
    img = frame->clone();
    //Rect rect = boundingRect(Mat(circle));
    Scalar color(0,255,255); // draw a yellow rectangle on the image
    rectangle(img, rect, color, 1);
}
