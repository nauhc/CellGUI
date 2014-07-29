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

// get ROI + edgeDectection
void FindContour::cellDetection(const Mat &img, vector<Point> &cir,
                                Mat &dispImg1, Mat &dispImg2,
                                int &area, int &perimeter){
    frame = &img;
    rect = boundingRect(Mat(cir));
    dispImg1 = (*frame)(rect).clone();
    //dispImg2 = Mat(dispImg1.rows, dispImg1.cols, CV_8UC3);;

/*
    Rect rect_roi = boundingRect(Mat(cir));
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
    cv::cvtColor(dispImg1, sub, CV_RGB2GRAY);
    int width = sub.cols;
    int height = sub.rows;

    //filter the image inside the circle
    vector<Point> circle_ROI;
    for (unsigned int i = 0; i < cir.size(); i++){
        Point p = Point(cir[i].x - rect.x, cir[i].y - rect.y);
        circle_ROI.push_back(p);
    }

    //mask for filtering out the cell of interest
    Mat mask = Mat::zeros(height, width, CV_8UC1);
    fillConvexPoly(mask, circle_ROI, Scalar(255));

    for(unsigned int c = 0; c < circle_ROI.size(); c++){
        for(int j = -2; j < 2; j++){
            int jj = circle_ROI[c].y+j;
            for(int i = -2; i < 2; i++){
                int ii = circle_ROI[c].x+i;
                if(ii < 0 || jj < 0 || ii >= width || jj >= height)
                    continue;
                mask.at<uchar>(jj, ii) = 255;
            }
        }
    }
    Mat adapThreshImg = Mat::zeros(height, width, sub.type());
    //image edge detection for the sub region (roi rect)
    adaptiveThreshold(sub, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                          CV_THRESH_BINARY_INV, blockSize, constValue);

    vector<Point> whiteArea;
    //let roi only display the region inside the circle
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
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
    Mat openclose;
    dilate(adapThreshImg, open, element);
    erode(open, openclose, element);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // Find contours
    Mat openclose_clone = openclose.clone();
    findContours( openclose_clone, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    perimeter = 0;
    for(unsigned int i = 0; i < contours.size(); i++){
        drawContours( dispImg1, contours, i, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
        perimeter += contours[i].size();
    }

    GaussianBlur(openclose, openclose, Size(3, 3), 2, 2 );
    cvtColor(openclose, dispImg2, CV_GRAY2RGB);

    /*
    vector<Vec3f> circles;
    HoughCircles(openclose, circles, CV_HOUGH_GRADIENT, 3, width/20,
                 50, 90, width/20, width);

    cout << "circle found:" << circles.size();
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        if(pow((center.x - width/2), 2.0) +
                pow((center.y - height/2), 2.0) >=
                pow( min(width/3, height/3), 2.0)
                && radius > width/6){
            circle( openclose, center, 2, Scalar(255,0,255), -1, 8, 0 );
            continue;
        }
        // circle center
        //circle(openclose, center, 2, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle(openclose, center, radius+3, Scalar(255,255,255), 1, 8, 0 );
    }
    cout<< endl;
    //imshow("blurred_openclose.tiff", openclose);*/

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
    Scalar color(49, 204, 152); // draw a green rectangle on the image
    rectangle(img, rect, color, 2);
}

