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
    Mat element3 = getStructuringElement( MORPH_ELLIPSE, Size( 2*3+1, 2*3+1 ), Point(3, 3));
    Mat dil;
    dilate(in, dil, element3);
    erode(dil, dilerod, element3);
}

void dilErodContours(Mat &dilerod,
                     vector<vector<Point> > &contours,
                     vector<Vec4i> &hierarchy,
                     RotatedRect &elps,
                     int &perimeter,
                     Mat &drawTarget){ //drawTarget-> dispImg1
    // Find contours
    findContours( dilerod.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //find the min enclosing ellipse of the contours
    vector<RotatedRect> minEllipse( contours.size() );
    unsigned int largest_contour_index = 0;
    int s_tmp = contours[0].size();
    for(unsigned int i = 0; i < contours.size(); i++ ){
        if( contours[i].size() > 50 ){
            minEllipse[i] = fitEllipse( Mat(contours[i]) );
        }
        if( contours[i].size() > s_tmp){
            s_tmp = contours[i].size();
            largest_contour_index = i;
        }
    }


    // draw contours and the enclosing ellipse
    perimeter = 0;
    for(unsigned int i = 0; i < contours.size(); i++){
        drawContours( drawTarget, contours, i, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
        if(i == largest_contour_index){
            ellipse( drawTarget, minEllipse[i], Scalar(153, 204, 49), 1, 8 );
        }else{
            //ellipse( drawTarget, minEllipse[i], Scalar(0,172,251), 1, 8 );
        }
        perimeter += contours[i].size();
    }
    //drawContours( drawTarget, contours, largest_contour_index, Scalar(255,255,255), 1, 8, hierarchy, 0, Point() );
    elps = minEllipse[largest_contour_index];
    ellipse( drawTarget, elps, Scalar(255,172,251), 1, 8 );
    //perimeter = contours[largest_contour_index].size();
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
    //    for(int j = 0; j < height; j++){
    //        for(int i = 0; i < width; i++){
    //            if(markers.at<uchar>(j,i) == 0){
    //                dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+0] = 81;
    //                dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+1] = 172;
    //                dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+2] = 251;
    //            }
    //        }
    //    }
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

    Mat canny;
    CannyWithBlur(sub, canny);

    vector<Point> circle_ROI; //***local coordinates of circle***
    for (unsigned int i = 0; i < cir.size(); i++){
        Point p = Point(cir[i].x - rect.x, cir[i].y - rect.y);
        circle_ROI.push_back(p);
    }

    Mat adapThreshImg1 = Mat::zeros(height, width, sub.type());
    //image edge detection for the sub region (roi rect)
    adaptiveThreshold(sub, adapThreshImg1, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                          CV_THRESH_BINARY_INV, blockSize, constValue);
    imshow("adapThreshImg1", adapThreshImg1);


    Mat dilerod;
    dilErod(adapThreshImg1, dilerod);

    //display image 2 -- dilerod of adaptive threshold image
    GaussianBlur(dilerod, dilerod, Size(3, 3), 2, 2 );
    cvtColor(dilerod, dispImg2, CV_GRAY2RGB);

    // findcontours and constrain shape
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RotatedRect elps;
    dilErodContours(adapThreshImg1, contours, hierarchy, elps, perimeter, dispImg1);

    //** remove all the points outside the ellipse to be used as the circle in the next frame **
    int wth = elps.size.width, hgt = elps.size.height;
    Point2f cntr = elps.center;

    //mask for filtering out the cell of interest
    Mat mask_conv = Mat::zeros(height, width, CV_8UC1);
    fillConvexPoly(mask_conv, circle_ROI, Scalar(255));
    imshow("mask_conv", mask_conv);

    Mat mask_elps = Mat::zeros(height, width, CV_8UC1);
    ellipse(mask_elps, elps, Scalar(255), -1);
    imshow("mask_elps", mask_elps);

    Mat mask_and;
    bitwise_and(mask_conv, mask_elps, mask_and);
    imshow("AND", mask_and);

    for(unsigned int c = 0; c < circle_ROI.size(); c++){
        Point p = circle_ROI[c];
        // if circle point is outside the ellipse, then dont consider as the mask
        if( (square(p.x-cntr.x)/square(wth/2) + square(p.y-cntr.y)/square(hgt/2)) > 1 ){
            circle(dispImg1, p, 2, Scalar(255, 255, 0), -1);
            continue;
        }
        if(abs((square(p.x-cntr.x)/square(wth/2) + square(p.y-cntr.y)/square(hgt/2))-1) > 5)
            circle(dispImg1, p, 1, Scalar(0, 255, 0), -1);
        for(int j = -2; j < 2; j++){
            int jj = circle_ROI[c].y+j;
            for(int i = -2; i < 2; i++){
                int ii = circle_ROI[c].x+i;
                if(ii < 0 || jj < 0 || ii >= width || jj >= height)
                    continue;
                mask_conv.at<uchar>(jj, ii) = 255;
            }
        }
    }
    //imshow("mask_after", mask); //good

    vector<Point> whiteArea;
    //let roi only display the region inside the circle
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            if(mask_conv.at<uchar>(j,i) == 0)
                dilerod.at<uchar>(j,i) = 0;
            if(dilerod.at<uchar>(j,i) != 0)
                whiteArea.push_back(Point(i,j));
        }
    }
    area = whiteArea.size();

    vector<vector<Point> > contours_update;
    vector<Vec4i> hierarchy_update;
    // Find contours again
    Mat dilerod_clone = dilerod.clone();
    findContours( dilerod_clone, contours_update, hierarchy_update, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    perimeter = 0;
    for(unsigned int i = 0; i < contours_update.size(); i++){
        if(contours_update[i].size() > 200)
            drawContours( dispImg1, contours_update, i, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
        perimeter += contours_update[i].size();
    }



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

