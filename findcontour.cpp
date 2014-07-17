#include "findcontour.h"
#include "imgproc/imgproc.hpp"

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
void FindContour::cellDetection(const Mat &img, vector<Point> &cir,
                                Mat &openclose, int &area){
    frame = &img;
    rect = boundingRect(Mat(cir));

/*
    Rect rect_roi = boundingRect(Mat(circle));
    int e = 1;
    int x = rect_roi.x > e? rect_roi.x - e : 0;
    int y = rect_roi.y > e? rect_roi.y - e : 0;
    int w = x+rect_roi.width+2*e < img.cols? rect_roi.width+2*e : img.cols-x;
    int h = y+rect_roi.height+2*e < img.rows? rect_roi.height+2*e : img.rows-y;
    rect = Rect(x, y, w, h);

//    Mat sub_0; // the rectangle region of ROI
//    cv::cvtColor((*frame)(rect_roi), sub_0, CV_RGB2GRAY);

//    //filter the image inside the circle
//    vector<Point> circle_ROI_0;
//    for (unsigned int i = 0; i < circle.size(); i++){
//        Point p = Point(circle[i].x - rect_roi.x, circle[i].y - rect_roi.y);
//        circle_ROI_0.push_back(p);
//    }

//    //mask for filtering out the cell of interest
//    Mat mask_0 = Mat::zeros(sub_0.rows, sub_0.cols, CV_8UC1);
//    fillConvexPoly(mask_0, circle_ROI_0, Scalar(255));

//    imshow("mask_0", mask_0); */

    Mat sub; // the rectangle region of ROI
    cv::cvtColor((*frame)(rect), sub, CV_RGB2GRAY);

    //filter the image inside the circle
    vector<Point> circle_ROI;
    for (unsigned int i = 0; i < cir.size(); i++){
        Point p = Point(cir[i].x - rect.x, cir[i].y - rect.y);
        circle_ROI.push_back(p);
    }

    //mask for filtering out the cell of interest
    Mat mask = Mat::zeros(sub.rows, sub.cols, CV_8UC1);
    fillConvexPoly(mask, circle_ROI, Scalar(255));

//    vector<Point> concaveHull;
//    approxPolyDP(circle, concaveHull, 0.01*arcLength(circle,true), true);
//    fillConvexPoly(mask, concaveHull, Scalar(128));
//    imshow("mask", mask);

    for(unsigned int c = 0; c < circle_ROI.size(); c++){
        for(int j = -2; j < 2; j++){
            int jj = circle_ROI[c].y+j;
            for(int i = -2; i < 2; i++){
                int ii = circle_ROI[c].x+i;
                if(ii < 0 || jj < 0 || ii >= mask.cols || jj >= mask.rows)
                    continue;
                mask.at<uchar>(jj, ii) = 255;
            }
        }
    }
    Mat adapThreshImg;
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
    area = whiteArea.size();

    // mophological opening and closing
    Mat open;
    int dilation_size = 3;
    Mat element = getStructuringElement( MORPH_ELLIPSE,
                                         Size( 2*dilation_size+1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
    dilate(adapThreshImg, open, element);
    //Mat openclose;
    erode(open, openclose, element);

    GaussianBlur( openclose, openclose, Size(5, 5), 2, 2 );
    imshow("blured openclose", openclose);
//    vector<Vec3f> circles;
//    HoughCircles(sub, circles, CV_HOUGH_GRADIENT, 1, adapThreshImg.rows/10, 100, 100, 0, 0 );

//    cout << "circle found:" << circles.size();
//    for( size_t i = 0; i < circles.size(); i++ )
//    {
//       Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//       int radius = cvRound(circles[i][2]);
//       cout << "center " << center << " radius " << radius << "\n";
//       // circle center
//       circle(openclose, center, 3, Scalar(0,255,0), -1, 8, 0 );
//       // circle outline
//       circle(openclose, center, radius, Scalar(0,0,255), 3, 8, 0 );
//     }
//    cout<< endl;

    //renew circle points as the convex hull
    vector<Point> convHull;
    convexHull(whiteArea, convHull);
    cir.clear();
    for(unsigned int i = 0; i < convHull.size(); i++)
        cir.push_back(Point(convHull[i].x + rect.x, convHull[i].y + rect.y));

}


void FindContour::boundingBox(Mat &img)
{
    img = frame->clone();
    //Rect rect = boundingRect(Mat(circle));
    Scalar color(128, 255, 0); // draw a green rectangle on the image
    rectangle(img, rect, color, 2);
}

