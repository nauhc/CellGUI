#include "findcontour.h"
#include "imgproc/imgproc.hpp"

template <class T>
inline T square(T value){
    return value*value;
}

class WatershedSegmenter{
private:
    cv::Mat markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(markers, CV_32S);
    }

    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, markers);
        markers.convertTo(markers,CV_8U);
        return markers;
    }
};

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

void FindContour::setScale(double scl){
        scale = scl;
}

Rect enlargeRect(Rect rect_roi, int e, int cols, int rows){
    int x = rect_roi.x > e? rect_roi.x - e : 0;
    int y = rect_roi.y > e? rect_roi.y - e : 0;
    return Rect(x, y,
                x+rect_roi.width+2*e < cols? rect_roi.width+2*e : cols-x, //w
                y+rect_roi.height+2*e < rows? rect_roi.height+2*e : rows-y); //h
}

void CannyWithBlur(Mat &in, Mat &out){
    GaussianBlur(in, out, Size(3, 3), 2, 2 );
    //imshow("blur", out);
    int lowThreshold = 30;
    Canny(out, out, lowThreshold, lowThreshold*4.0, 3 );
    //imshow("canny", out);
}

void dilErod(Mat &in, Mat&dilerod){
    // mophological dialate and erode
    int dilation_size = 3;
    Mat element = getStructuringElement( MORPH_ELLIPSE,
                                         Size( 2*dilation_size+1, 2*dilation_size+1 ),
                                         Point( dilation_size, dilation_size ) );
    Mat dil;
    dilate(in, dil, element);
    erode(dil, dilerod, element);
}

void dilErodContours(Mat &dilerod,
                     vector<vector<Point> > &contours,
                     vector<Vec4i> &hierarchy,
                     unsigned int &largest_contour_index,
                     int &perimeter,
                     Mat &drawTarget){ //drawTarget-> dispImg1
    // Find contours
    findContours( dilerod.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    largest_contour_index = 0;
    int s_tmp = contours[0].size();
    for(unsigned int i = 0; i < contours.size(); i++){
        drawContours( drawTarget, contours, i, Scalar(51,100,175), 1, 8, hierarchy, 0, Point() );
        if( contours[i].size() > s_tmp){
            s_tmp = contours[i].size();
            largest_contour_index = i;
        }
    }
    perimeter = contours[largest_contour_index].size();
    //draw only the largest contour
    drawContours( drawTarget, contours, largest_contour_index, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
}


void watershedWithErosion(Mat &in, Mat &dispImg1, Mat &out){ //in->adapThreshImg1
    Mat element1 = getStructuringElement( MORPH_ELLIPSE, Size( 2*1+1, 2*1+1 ), Point(1, 1));
    Mat element3 = getStructuringElement( MORPH_ELLIPSE, Size( 2*3+1, 2*3+1 ), Point(3, 3));
    Mat ero;
    erode(in, ero, element1);
    imshow("erosion", ero);
    Mat dil;
    dilate(in, dil, element3);
    Mat bg;
    threshold(dil, bg, 1, 128, 1);
    imshow("bg", bg);
    Mat markers;
    add(ero, bg, markers);
    WatershedSegmenter segmenter;
    segmenter.setMarkers(markers);
    Mat dispImg1Clone = dispImg1.clone();
    out = segmenter.process(dispImg1Clone);
    out.convertTo(out,CV_8UC3);
    imshow("final_result", out);
    /*
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                if(markers.at<uchar>(j,i) == 0){
                    dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+0] = 81;
                    dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+1] = 172;
                    dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+2] = 251;
                }
            }
        }*/
}

// get ROI + edgeDectection
void FindContour::cellDetection(const Mat &img, vector<Point> &cir_org,
                                Mat &dispImg1, Mat &dispImg2,
                                int &area, int &perimeter){
    frame = &img;
    //rect = boundingRect(Mat(cir));

    vector<Point> cir; //***global coordinates of circle***
    for(unsigned int i = 0; i < cir_org.size(); i++){
        cir.push_back(Point(cir_org[i].x* scale, cir_org[i].y * scale));
    }
    //cout << "original circle: " << cir_org << "\n" << " scaled circle: " << cir << endl;

    //enlarge the bounding rect by adding a margin (e) to it
    rect = enlargeRect(boundingRect(Mat(cir)), 2, img.cols, img.rows);

    dispImg1 = (*frame)(rect).clone();
    //dispImg2 = Mat(dispImg1.rows, dispImg1.cols, CV_8UC3);

    Mat sub; //*** the rectangle region of ROI (Gray) ***
    cv::cvtColor(dispImg1, sub, CV_RGB2GRAY);
    int width = sub.cols;
    int height = sub.rows;

    //Mat canny;
    //CannyWithBlur(sub, canny);

    vector<Point> circle_ROI; //***local coordinates of circle***
    for (unsigned int i = 0; i < cir.size(); i++){
        Point p = Point(cir[i].x - rect.x, cir[i].y - rect.y);
        circle_ROI.push_back(p);
    }

    Mat adapThreshImg1 = Mat::zeros(height, width, sub.type());
    //image edge detection for the sub region (roi rect)
    adaptiveThreshold(sub, adapThreshImg1, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                          CV_THRESH_BINARY_INV, blockSize, constValue);
    //imshow("adapThreshImg1", adapThreshImg1);

    // dilation and erosion
    Mat dilerod;
    dilErod(adapThreshImg1, dilerod);

    //display image 2 -- dilerod of adaptive threshold image
    GaussianBlur(dilerod, dilerod, Size(3, 3), 2, 2 );

    //mask for filtering out the cell of interest
    Mat mask_conv = Mat::zeros(height, width, CV_8UC1);
    fillConvexPoly(mask_conv, circle_ROI, Scalar(255));
    //imshow("mask_conv", mask_conv);

    //bitwise AND on mask and dilerod
    bitwise_and(mask_conv, dilerod, dispImg2);

    // findcontours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    unsigned int largest_contour_index;
    dilErodContours(dispImg2, contours, hierarchy, largest_contour_index, perimeter, dispImg1);

    // find the area of the cell by counting the white area inside the largest contour
    Mat cell = Mat::zeros(height, width, CV_8UC1);
    drawContours(cell, contours, largest_contour_index, Scalar(255), -1, 8, hierarchy, 0, Point() );
    //imshow("cell", cell);
    area = countNonZero(cell);


    //change dispImg2 from gray to rgb for displaying
    cvtColor(dispImg2, dispImg2, CV_GRAY2RGB);


    //renew circle points as the convex hull
    vector<Point> convHull;
    convexHull(contours[largest_contour_index], convHull);
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

