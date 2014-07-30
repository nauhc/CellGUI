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

// get ROI + edgeDectection
void FindContour::cellDetection(const Mat &img, vector<Point> &cir,
                                Mat &dispImg1, Mat &dispImg2,
                                int &area, int &perimeter){
    frame = &img;
//    rect = boundingRect(Mat(cir));

    //enlarge the bounding rect by adding a margin (e) to it
    Rect rect_roi = boundingRect(Mat(cir));
    int e = 2;
    int x = rect_roi.x > e? rect_roi.x - e : 0;
    int y = rect_roi.y > e? rect_roi.y - e : 0;
    int w = x+rect_roi.width+2*e < img.cols? rect_roi.width+2*e : img.cols-x;
    int h = y+rect_roi.height+2*e < img.rows? rect_roi.height+2*e : img.rows-y;
    rect = Rect(x, y, w, h);

    dispImg1 = (*frame)(rect).clone();
    //dispImg2 = Mat(dispImg1.rows, dispImg1.cols, CV_8UC3);

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

    Mat adapThreshImg = Mat::zeros(height, width, sub.type());
    //image edge detection for the sub region (roi rect)
    adaptiveThreshold(sub, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                          CV_THRESH_BINARY_INV, blockSize, constValue);

    //imshow("adapThreshImg", adapThreshImg);

    // mophological dialate and erode
    //Mat element1 = getStructuringElement( MORPH_ELLIPSE, Size( 2*1+1, 2*1+1 ), Point(1, s1));
    Mat element3 = getStructuringElement( MORPH_ELLIPSE, Size( 2*3+1, 2*3+1 ), Point(3, 3));

    Mat dil;
    dilate(adapThreshImg, dil, element3);
    Mat dilerod;
    erode(dil, dilerod, element3);

    /*
//    Mat ero;
//    erode(adapThreshImg, ero, element1);
//    imshow("erosion", ero);
//    Mat bg;
//    threshold(dil, bg, 1, 128, 1);
//    imshow("bg", bg);
//    Mat markers;
//    add(ero, bg, markers);
//    WatershedSegmenter segmenter;
//    segmenter.setMarkers(markers);
//    Mat result = segmenter.process(dispImg1);
//    //result.convertTo(dispImg1,CV_8UC3);
//    //imshow("final_result", dispImg1);
//    for(int j = 0; j < height; j++){
//        for(int i = 0; i < width; i++){
//            if(markers.at<uchar>(j,i) == 0){
//                dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+0] = 81;
//                dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+1] = 172;
//                dispImg1.data[dispImg1.channels()*(dispImg1.cols*j + i)+2] = 251;
//            }
//        }
//    } */

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    // Find contours
    Mat openclose_clone = dilerod.clone();
    findContours( openclose_clone, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

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
/*
//    perimeter = 0;
//    for(unsigned int i = 0; i < contours.size(); i++){
//        drawContours( dispImg1, contours, i, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
//        if(i == largest_contour_index){
//            ellipse( dispImg1, minEllipse[i], Scalar(81,172,251), 2, 8 );
//        }
//        perimeter += contours[i].size();
//    }*/
    //drawContours( dispImg1, contours, largest_contour_index, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
    RotatedRect elps = minEllipse[largest_contour_index];
    //ellipse( dispImg1, elps, Scalar(81,172,251), 2, 8 );
    //perimeter = contours[largest_contour_index].size();


    //** remove all the points outside the ellipse to be used as the circle in the next frame **
    int wth = elps.size.width, hgt = elps.size.height;
    Point2f cntr = elps.center;

    //mask for filtering out the cell of interest
    Mat mask = Mat::zeros(height, width, CV_8UC1);
    fillConvexPoly(mask, circle_ROI, Scalar(255));

    for(unsigned int c = 0; c < circle_ROI.size(); c++){
        Point p = circle_ROI[c];
        // if circle point is outside the ellipse, then dont consider as the mask
        if( (square(p.x-cntr.x)/square(wth/2)+ square(p.y-cntr.y)/square(hgt/2)) > 1 )
            continue;

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
//    imshow("mask", mask); //good

    vector<Point> whiteArea;
    //let roi only display the region inside the circle
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            if(mask.at<uchar>(j,i) == 0)
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
        if(contours_update[i].size() > 100)
            drawContours( dispImg1, contours_update, i, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
        perimeter += contours[i].size();
    }


    GaussianBlur(dilerod, dilerod, Size(3, 3), 2, 2 );
    cvtColor(dilerod, dispImg2, CV_GRAY2RGB);

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

