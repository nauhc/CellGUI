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

void FindContour::edgeDetection(Mat &adapThreshImg){
//    int blockSize = 17;
//    double constValue = 7;
//    adaptiveThreshold(*roi, adapThreshImg, 255.0, CV_ADAPTIVE_THRESH_MEAN_C,
//                      CV_THRESH_BINARY_INV, blockSize, constValue);
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


void FindContour::getROI(const Mat &img, vector<Point> &circle, Mat &mask)
{
    frame = &img;
    roi_rect = boundingRect(Mat(circle));

    Mat sub; // the rectangle region of ROI
    cv::cvtColor((*frame)(roi_rect), sub, CV_RGB2GRAY);

    //filter the image inside the circle
    vector<Point> circle_ROI;
    for (unsigned int i = 0; i < circle.size(); i++){
        Point p = Point(circle[i].x - roi_rect.x, circle[i].y - roi_rect.y);
        circle_ROI.push_back(p);
    }
    mask = Mat::zeros(sub.rows, sub.cols, CV_8UC1);
    fillConvexPoly(mask, circle_ROI, Scalar(255));
    *roi = sub.clone();
    //convert color image to grayscale image
    //cv::cvtColor(sub_filter, *roi, CV_RGB2GRAY);
}

void FindContour::edgeDetection(Mat &adapThreshImg, Mat &mask, vector<Point> &circle)
{
    //adaptive threshold method to detect edges in the roi
    adaptiveThreshold(*roi, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                          CV_THRESH_BINARY_INV, blockSize, constValue);

    vector<Point> whiteArea;
    //let roi only display the region inside the circle
    for(int j = 0; j < roi->rows; j++){
        for(int i = 0; i < roi->cols; i++){
            if(mask.at<uchar>(j,i) == 0)
                adapThreshImg.at<uchar>(j,i) = 0;
            if(adapThreshImg.at<uchar>(j,i) != 0)
                whiteArea.push_back(Point(i,j));
        }
    }

    //renew circle
    vector<Point> convHull;
    convexHull(whiteArea, convHull);

    /*for(unsigned int i = 0; i < circle.size(); i++)
        //std::cout << circle[i].x << ", " << circle[i].y << "   ";
    cout << "\n";*/

    circle.clear();
    for(unsigned int i = 0; i < convHull.size(); i++)
        circle.push_back(Point(convHull[i].x + roi_rect.x, convHull[i].y + roi_rect.y));

    /*for(unsigned int i = 0; i < circle.size(); i++)
        //std::cout << circle[i].x << ", " << circle[i].y << "   ";
    cout << "\n\n" << endl;*/

    //Rect rect_temp = boundingRect(Mat(convHull));
    //rectangle(adapThreshImg, rect_temp, Scalar(128), 1);

}


void FindContour::boundingBox(Mat &img, vector<Point> &circle)
{
    img = frame->clone();
    Rect rect = boundingRect(Mat(circle));
    Scalar color(0,255,255); // draw a yellow rectangle on the image
    rectangle(img, rect, color, 1);
}
