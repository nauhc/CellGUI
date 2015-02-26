#include "findcontour.h"
#include "imgproc/imgproc.hpp"
#include "qstring.h"

template <class T>
inline T sqre(T value){
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

void FindContour::setDilSize(int var)
{
    dilSize = var;
}

void FindContour::setblebSizeRatio(int var)
{
    blebSizeRatio = 1.0/double((11-var)*50);
    //cout << "blebSizeRatio changed to: 1/" << (11-var)*50 << endl;
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

Rect translateRect(Rect rect, Point2f vect){
    return Rect(rect.x+vect.x, rect.y+vect.y,
                rect.width, rect.height);
}

void CannyWithBlur(Mat &in, Mat &out){
    //GaussianBlur(in, out, Size(3, 3), 2, 2 );
    //imshow("blur", out);
    int lowThreshold = 8;
    Canny(in, out, lowThreshold, lowThreshold*6.0, 3, true);
    //imshow("canny", out);
}

void dilErod(Mat &in, Mat&dilerod, int dilation_size){
    // mophological dialate and erode
    // dilation_size = 3;
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
    unsigned int s_tmp = contours[0].size();
    for(unsigned int i = 0; i < contours.size(); i++){
        //drawContours( drawTarget, contours, i, Scalar(51,100,175), 1, 8, hierarchy, 0, Point() );
        if( contours[i].size() > s_tmp){
            s_tmp = contours[i].size();
            largest_contour_index = i;
        }
    }
    perimeter = contours[largest_contour_index].size();
    //draw only the largest contour
    drawContours( drawTarget, contours, largest_contour_index, Scalar(81,172,251), 1, 8, hierarchy, 0, Point() );
}

Mat createAlphaMat(Mat &mat)
{
    Mat mat_alpha(mat.rows, mat.cols, CV_8UC4);
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            int v = mat.at<uchar>(i,j);
            if(v == 0){
                Vec4b& rgba = mat_alpha.at<Vec4b>(i, j);
                rgba[0] = saturate_cast<uchar>(1.0 * UCHAR_MAX);
                rgba[1] = saturate_cast<uchar>(1.0 * UCHAR_MAX);
                rgba[2] = saturate_cast<uchar>(1.0 * UCHAR_MAX);
                rgba[3] = saturate_cast<uchar>(0.0 * UCHAR_MAX);
            }else{
                Vec4b& rgba = mat_alpha.at<Vec4b>(i, j);
                rgba[0] = saturate_cast<uchar>(v/255.0 * UCHAR_MAX);
                rgba[1] = saturate_cast<uchar>(v/255.0 * UCHAR_MAX);
                rgba[2] = saturate_cast<uchar>(v/255.0 * UCHAR_MAX);
                rgba[3] = saturate_cast<uchar>(1.0 * UCHAR_MAX);
            }
        }
    }
    return mat_alpha;
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

double dist_square(Point &p1, Point &p2){
    return (sqre(p1.x - p2.x)+sqre(p1.y -p2.y));
}


vector<Point> pointTransform(const vector<Point> &srcPoints, const Mat &H) {

    double h11 = H.at<double>(0, 0);
    double h12 = H.at<double>(0, 1);
    double h13 = H.at<double>(0, 2);
    double h21 = H.at<double>(1, 0);
    double h22 = H.at<double>(1, 1);
    double h23 = H.at<double>(1, 2);
    double h31 = H.at<double>(2, 0);
    double h32 = H.at<double>(2, 1);
    double h33 = H.at<double>(2, 2);

    double x_trans0 = (h11 * srcPoints[0].x + h12 * srcPoints[0].y + h13) /
      (h31 * srcPoints[0].x + h32 * srcPoints[0].y + h33);
    double y_trans0 = (h21 * srcPoints[0].x + h22 * srcPoints[0].y + h23) /
      (h31 * srcPoints[0].x + h32 * srcPoints[0].y + h33);
    double x_trans1 = (h11 * srcPoints[1].x + h12 * srcPoints[1].y + h13) /
      (h31 * srcPoints[1].x + h32 * srcPoints[1].y + h33);
    double y_trans1 = (h21 * srcPoints[1].x + h22 * srcPoints[1].y + h23) /
      (h31 * srcPoints[1].x + h32 * srcPoints[1].y + h33);
    double x_trans2 = (h11 * srcPoints[2].x + h12 * srcPoints[2].y + h13) /
      (h31 * srcPoints[2].x + h32 * srcPoints[2].y + h33);
    double y_trans2 = (h21 * srcPoints[2].x + h22 * srcPoints[2].y + h23) /
      (h31 * srcPoints[2].x + h32 * srcPoints[2].y + h33);
    double x_trans3 = (h11 * srcPoints[3].x + h12 * srcPoints[3].y + h13) /
      (h31 * srcPoints[3].x + h32 * srcPoints[3].y + h33);
    double y_trans3 = (h21 * srcPoints[3].x + h22 * srcPoints[3].y + h23) /
      (h31 * srcPoints[3].x + h32 * srcPoints[3].y + h33);

    vector<Point> dstPoints;
    dstPoints.push_back(Point(x_trans0, y_trans0));
    dstPoints.push_back(Point(x_trans1, y_trans1));
    dstPoints.push_back(Point(x_trans2, y_trans2));
    dstPoints.push_back(Point(x_trans3, y_trans3));

    return dstPoints;

}

void FindBlobs(const Mat &binary, vector<vector<Point2i> > &blobs){
    blobs.clear();

    // Fill the label_image with the blobs
    // 0  - background
    // 1  - unlabelled foreground
    // 2+ - labelled foreground

    Mat label_image;
    binary.convertTo(label_image, CV_32SC1);

    int label_count = 64; // starts at 2 because 0,1 are used already

    for(int y=0; y < label_image.rows; y++) {
        int *row = (int*)label_image.ptr(y);
        for(int x=0; x < label_image.cols; x++) {
            if(row[x] != 1) {
                continue;
            }

            Rect rect_b;
            floodFill(label_image, Point(x,y), label_count, &rect_b, 0, 0, 4);

            vector <Point2i> blob;

            for(int i=rect_b.y; i < (rect_b.y+rect_b.height); i++) {
                int *row2 = (int*)label_image.ptr(i);
                for(int j=rect_b.x; j < (rect_b.x+rect_b.width); j++) {
                    if(row2[j] != label_count) {
                        continue;
                    }

                    blob.push_back(Point2i(j,i));
                }
            }

            blobs.push_back(blob);

            label_count++;
        }
    }
}

double findShape(Point2f &ctr, vector<Point> contour){
    unsigned int cnt = contour.size();
    vector<double> dist;
    double mean = 0.0;
    for(unsigned int i = 0; i < cnt; i++){
        double d = sqrt(sqre(contour[i].x - ctr.x)+sqre(contour[i].y - ctr.y));
        dist.push_back(d);
        mean += d;
    }
    mean = mean/cnt;

    float standDev= 0.0;
    for(unsigned int i = 0; i < cnt; i++){
        standDev += sqre(dist[i]-mean);
    }
    standDev = sqrt(standDev/cnt);
    return standDev;
}

void FindContour::binaryImage(const Mat &img, Mat &binaryImg){
    Mat frameGray;
    cv::cvtColor(img, frameGray, CV_RGB2GRAY);

    Mat tmp = Mat::zeros(frameGray.rows, frameGray.cols, frameGray.type());
    adaptiveThreshold(frameGray, tmp, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                      CV_THRESH_BINARY_INV, blockSize, constValue);
    dilErod(tmp, binaryImg, dilSize);

    cvtColor(binaryImg, binaryImg, CV_GRAY2RGB);
}

// --- curve smooth begin ---
struct polarPoint{
    double r;
    double theta;
};

double dist(Point p1, Point p2){
    return ((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

polarPoint cartesianToPolar(Point &ctr, Point &pt){
    polarPoint p;
    p.r        = sqrt(dist(ctr, pt));
    double x   = pt.x - ctr.x;
    double y   = pt.y - ctr.y;
    p.theta    = atan2(y, x);
    return p;
}

Point polarToCartesian(Point &ctr, polarPoint p){
    return Point(p.r*cos(p.theta)+ctr.x, p.r*sin(p.theta)+ctr.y);
}

bool sortByTheta(const polarPoint &l, const polarPoint &r){
    return l.theta < r.theta;
}
Mat FindContour::curveSmooth(Mat &contourImg,
                int WIN, // half window size for laplacian smoothing
                vector<Point> &border,
                vector<Point> &smooth,
                /*Point cntoid*/vector<Point> &convHull )
{
//    if(contourImg.type() != CV_8UC1){
//        cvtColor( contourImg, contourImg, CV_BGR2GRAY );
//    }
    double width = contourImg.cols;
    double height = contourImg.rows;

    Moments mu = moments(convHull);
//    Moments mu = moments(border);
    Point cntoid = Point2f(mu.m10/mu.m00/* + rect.x*/, mu.m01/mu.m00/* +rect.y*/);

    double d_min = max(width, height)*max(width, height);
    vector<polarPoint> border_polar;
    for (unsigned int n = 0; n < border.size(); n++)
    {
        //find the polar coordinates of the border;
        border_polar.push_back(cartesianToPolar(cntoid, border[n]));

        //find the nearest point to the center on the border
        double d = dist(cntoid, border[n]);
        if(d < d_min){
            d_min = d;
        }
    }
    d_min = sqrt(d_min);

    // sort border_polar by theta
    sort(border_polar.begin(), border_polar.end(), sortByTheta);

    // Laplacian smoothing
    unsigned int border_size = border_polar.size();
    for(unsigned int n = 0; n < border_size; n++){
        //cout << border_polar[n].r << " " << border_polar[n].theta << "  ";

        double avg = 0;
        for(int w = -WIN; w < WIN; w++){
            unsigned int pos = std::fabs((w+n+border_size)%border_size);
            //cout << " pos " << pos << "  ";
            avg += border_polar[pos].r;
        }
        avg = avg/WIN/2;
        polarPoint polar;
        polar.r = avg;
        polar.theta = border_polar[n].theta;
        //cout << polar.r << " " << polar.theta << " ";
        Point p = polarToCartesian(cntoid, polar);
        //circle(color, p, 1, Scalar(255, 255, 0));
        smooth.push_back(p);
        //cout << p.x << " " << p.y << "\n";
    }

    Mat smoothCircle = Mat::zeros(height, width, CV_8UC1);
    fillConvexPoly(smoothCircle, smooth, Scalar(255));
//    fillPoly(smoothCircle, smooth, Scalar(255));
    //imshow("smoothCircle", smoothCircle);

    return smoothCircle;
}
// --- curve smooth end ---

bool is_noise(int i, int SIZE){
    if(i <= SIZE)
        return true;
    else
        return false;
}


// get ROI + edgeDectection
void FindContour::cellDetection(const Mat &img, vector<Point> &cir_org,
                                Mat &dispImg1, Mat &dispImg2,
                                vector<Point2f> &points1, vector<Point2f> &points2,
                                int &area,
                                int &perimeter,
                                Point2f &ctroid,
                                float &shape,
//                                vector<int> &blebs,
                                int &frameNum){
    frame = &img;
    //rect = boundingRect(Mat(cir));


    Mat frameGray;
    cv::cvtColor(*frame, frameGray, CV_RGB2GRAY);
/*
    QString cellFileName0 = "frame" + QString::number(frameNum) + ".png";
    imwrite(cellFileName0.toStdString(), frameGray);*/

    vector<Point> cir; //***global coordinates of circle***
    for(unsigned int i = 0; i < cir_org.size(); i++){
        cir.push_back(Point(cir_org[i].x / scale, cir_org[i].y / scale));
    }
    //cout << "original circle: " << cir_org << "\n" << " scaled circle: " << cir << endl;

    //enlarge the bounding rect by adding a margin (e) to it
    rect = enlargeRect(boundingRect(Mat(cir)), 5, img.cols, img.rows);

    //global circle mask
    Mat mask_cir_org = Mat::zeros(frame->size(), CV_8UC1);
    fillConvexPoly(mask_cir_org, cir, Scalar(255));

    // flow points
    vector<unsigned int> cell_pts_global;
    vector<Point2f> longOptflow_pt1, longOptflow_pt2;
    Point2f avrg_vec = Point2f(0,0);
    for(unsigned int i = 0; i < points1.size(); i++){
        Point p1 = Point(points1[i].x, points1[i].y);
        Point p2 = Point(points2[i].x, points2[i].y);
        if(mask_cir_org.at<uchar>(p1.y,p1.x) == 255 ){
            cell_pts_global.push_back(i);
            if(dist_square(p1, p2) > 2.0){
                longOptflow_pt1.push_back(Point2f(p1.x, p1.y));
                longOptflow_pt2.push_back(Point2f(p2.x, p2.y));
                avrg_vec.x += (p2.x-p1.x);
                avrg_vec.y += (p2.y-p1.y);
            }
        }
    }

//    if(longOptflow_pt1.size()!= 0){
//        avrg_vec.x = avrg_vec.x / longOptflow_pt1.size();
//        avrg_vec.y = avrg_vec.y / longOptflow_pt1.size();
//    }
    Rect trans_rect = translateRect(rect, avrg_vec);


    // ***
    // if (the homography is a good one) use the homography to update the rectangle
    // otherwise use the same rectangle
    // ***
    if (longOptflow_pt1.size() >= 4){
        Mat H = findHomography(Mat(longOptflow_pt1), Mat(longOptflow_pt2), CV_RANSAC, 2);
        //cout << "H: " << H << endl;

        if(determinant(H) >= 0){
            vector<Point> rect_corners;
            rect_corners.push_back(Point(rect.x, rect.y));
            rect_corners.push_back(Point(rect.x+rect.width, rect.y));
            rect_corners.push_back(Point(rect.x, rect.y+rect.height));
            rect_corners.push_back(Point(rect.x+rect.width, rect.y+rect.height));

            vector<Point> rect_update_corners = pointTransform(rect_corners, H);
            trans_rect = boundingRect(rect_update_corners);
        }
    }


    rectangle(frameGray, trans_rect, Scalar(255), 2);
    imshow("frameGray", frameGray);









    dispImg1 = (*frame)(rect).clone();
    //dispImg2 = Mat(dispImg1.rows, dispImg1.cols, CV_8UC3);

    Mat sub; //*** the rectangle region of ROI (Gray) ***
    cv::cvtColor(dispImg1, sub, CV_RGB2GRAY);
    int width = sub.cols;
    int height = sub.rows;

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
    dilErod(adapThreshImg1, dilerod, dilSize);

    //display image 2 -- dilerod of adaptive threshold image
    GaussianBlur(dilerod, dilerod, Size(3, 3), 2, 2 );

    //mask for filtering out the cell of interest
    Mat mask_conv = Mat::zeros(height, width, CV_8UC1);
    fillConvexPoly(mask_conv, circle_ROI, Scalar(255));
    //imshow("mask_before", mask_conv);

    //dilate the mask -> region grows
    Mat mask_conv_dil;
    Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 2*2+2, 2*2+1 ), Point(2,2) );
    dilate(mask_conv, mask_conv_dil, element);
    //imshow("mask_dil", mask_conv_dil);



    /*
    Mat mask_conv_ero;
    erode(mask_conv, mask_conv_ero, element);
    Mat ring_dil, ring_ero;
    bitwise_xor(mask_conv, mask_conv_dil, ring_dil);
    bitwise_xor(mask_conv, mask_conv_ero, ring_ero);
    Mat ring;
    bitwise_or(ring_dil, ring_ero, ring);
    imshow("ring", ring);

    vector<unsigned int> opt_onRing_index;
    // use optflow info set rectangle
    for(unsigned int i = 0; i < points2.size(); i++){
        Point p2 = Point(points2[i].x, points2[i].y);
        Point p1 = Point(points1[i].x, points1[i].y);
        if(ring.at<uchar>(p1.y,p1.x) != 255 &&
                ring.at<uchar>(p2.y,p2.x) != 255)
            continue;
        else{
            opt_onRing_index.push_back(i);
        }
    }*/

    /*
    // draw the optflow on dispImg1
    unsigned int size = opt_inside_cl_index.size();
    for(unsigned int i = 0; i < size; i++ ){
        Point p0( ceil( points1[i].x - rect.x ), ceil( points1[i].y - rect.y ) );
        Point p1( ceil( points2[i].x - rect.x ), ceil( points2[i].y - rect.y) );
        //std::cout << "(" << p0.x << "," << p0.y << ")" << "\n";
        //std::cout << "(" << p1.x << "," << p1.y << ")" << std::endl;

        //draw lines to visualize the flow
        double angle = atan2((double) p0.y - p1.y, (double) p0.x - p1.x);
        double arrowLen = 0.01 * (double) (width);
        line(dispImg1, p0, p1, CV_RGB(255,255,255), 1 );
        Point p;
        p.x = (int) (p1.x + arrowLen * cos(angle + 3.14/4));
        p.y = (int) (p1.y + arrowLen * sin(angle + 3.14/4));
        line(dispImg1, p, p1, CV_RGB(255,255,255), 1 );
        p.x = (int) (p1.x + arrowLen * cos(angle - 3.14/4));
        p.y = (int) (p1.y + arrowLen * sin(angle - 3.14/4));

        line(dispImg1, p, Point(2*p1.x - p0.x, 2*p1.y - p0.y), CV_RGB(255,255,255), 1 );
        //line(dispImg1, p, p1, CV_RGB(255,255,255), 1 );
    }*/

/*
    //stop growing when meeting with canny edges that outside the circle

    Mat canny;
    CannyWithBlur(sub, canny);
    Mat cannyColor;
    cvtColor(canny, cannyColor, CV_GRAY2RGB);
    imwrite("canny.png", canny);
    vector<Point> outsideCircle;
    vector<Point> onRing;
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            if(canny.at<uchar>(j,i) != 0 && mask_conv.at<uchar>(j,i) == 0){
                cannyColor.data[cannyColor.channels()*(cannyColor.cols*j + i)+0] = 81;
                cannyColor.data[cannyColor.channels()*(cannyColor.cols*j + i)+1] = 172;
                cannyColor.data[cannyColor.channels()*(cannyColor.cols*j + i)+2] = 251;
                outsideCircle.push_back(Point(i, j));
                if(ring.at<uchar>(j,i) != 0){
                    cannyColor.data[cannyColor.channels()*(cannyColor.cols*j + i)+0] = 255;
                    cannyColor.data[cannyColor.channels()*(cannyColor.cols*j + i)+1] = 255;
                    cannyColor.data[cannyColor.channels()*(cannyColor.cols*j + i)+2] = 0;
                    onRing.push_back(Point(i,j));
                }
            }
        }
    } */

//    QString cannyFileName = "canny" + QString::number(frameNum) + ".png";
//    imwrite(cannyFileName.toStdString(), cannyColor);



    //bitwise AND on mask and dilerod
    bitwise_and(mask_conv/*_dil*/, dilerod, dispImg2);

    // findcontours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    unsigned int largest_contour_index;
    dilErodContours(dispImg2, contours, hierarchy, largest_contour_index, perimeter, dispImg1);

    // find the area of the cell by counting the white area inside the largest contour
    Mat cellArea = Mat::zeros(height, width, CV_8UC1);
    drawContours(cellArea, contours, largest_contour_index, Scalar(255), -1, 8, hierarchy, 0, Point() );
    //imshow("cell", cell);
    area = countNonZero(cellArea);

    //cout << "frame " << frameNum << "\n";
    //cout << contours[largest_contour_index] << endl;


    //change dispImg2 from gray to rgb for displaying
    cvtColor(dispImg2, dispImg2, CV_GRAY2RGB);

    //renew circle points as the convex hull
    vector<Point> convHull;
    convexHull(contours[largest_contour_index], convHull);


    // find the centroid of the contour
    Moments mu = moments(contours[largest_contour_index]);
    ctroid = Point2f(mu.m10/mu.m00 + rect.x, mu.m01/mu.m00 + rect.y);

    // find the shape of the cell by the largest contour and centroid
    shape = findShape(ctroid, contours[largest_contour_index]);

    ////---- draw largest contour start ----
    //draw the largest contour
    Mat borderImg = Mat::zeros(height, width, CV_8UC1);
    drawContours(borderImg, contours, largest_contour_index, Scalar(255), 1, 8, hierarchy, 0, Point());
    //QString cellFileName0 = "border" + QString::number(frameNum) + ".png";
    //imwrite(cellFileName0.toStdString(), borderImg);
    ////---- draw largest contour end ----

    // find the number and the sizes of blebs of the cell
    Mat smooth;
    vector<Point> smoothCurve;
    int WIN = 25;
    vector< vector<Point> > tmp;
    smooth = curveSmooth(borderImg, WIN, contours[largest_contour_index], smoothCurve, convHull/*ctroid*/);
    tmp.push_back(smoothCurve);
    drawContours(dispImg1, tmp, 0, Scalar(255, 0, 0));

    bitwise_not(smooth, smooth);
    Mat blebsImg;
    bitwise_and(smooth, cellArea, blebsImg);
    //imshow("blebs", blebs);
    //QString cellFileName2 = "blebs" + QString::number(frameNum) + ".png";
    //imwrite(cellFileName2.toStdString(), blebs);

//    vector<Point> blebCtrs;
//    recursive_connected_components(blebsImg, blebs, blebCtrs);
//    for(unsigned int i = 0; i < blebCtrs.size(); i++){
//        circle(dispImg1, blebCtrs[i], 2, Scalar(255, 255, 0));
//    }


    cir_org.clear();
    for(unsigned int i = 0; i < convHull.size(); i++)
        cir_org.push_back(Point((convHull[i].x + rect.x)*scale, (convHull[i].y + rect.y)*scale));

}

void drawPointVectors(Mat &img, vector<Point> &vec, int r, const Scalar& color){
    for(unsigned int i = 0; i < vec.size(); i++){
        circle(img, vec[i], r, color, -1);
    }
}

void FindContour::singleCellDetection(const Mat &img, vector<Point> &cir_org,
                                      Mat &dispImg1, Mat &dispImg2,
                                      int &area, int &perimeter,
                                      Point2f &ctroid, float &shape,
                                      Mat &cell_alpha, // only the area inside cell (without background)
                                      vector<Point> &smooth_contour_curve,
                                      Mat &blebsImg,
                                      Rect &rectangle,
                                      //vector<int> &blebs,
                                      int &frameNum)
{
    frame = &img;

    vector<Point> cir; //***global coordinates of circle***
    for(unsigned int i = 0; i < cir_org.size(); i++){
        cir.push_back(Point(cir_org[i].x / scale, cir_org[i].y / scale));
    }

    //enlarge the bounding rect by adding a margin (e) to it
    rect = enlargeRect(boundingRect(Mat(cir)), 5, img.cols, img.rows);
    //cout << "rect_roi " << boundingRect(Mat(cir)) << "\n";
    //cout << "enlarged rect " << rect << endl;

    dispImg1 = (*frame)(rect).clone();

    Mat sub; //*** the rectangle region of ROI (Gray) ***
    cv::cvtColor(dispImg1, sub, CV_RGB2GRAY);
    int width = sub.cols;
    int height = sub.rows;

    rectangle = rect;

//    Mat canny;
//    CannyWithBlur(sub, canny);
//    imshow("canny", canny);

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
    dilErod(adapThreshImg1, dilerod, dilSize);

    //display image 2 -- dilerod of adaptive threshold image
    GaussianBlur(dilerod, dilerod, Size(3, 3), 2, 2 );

    //mask for filtering out the cell of interest
    Mat mask_conv = Mat::zeros(height, width, CV_8UC1);
    fillConvexPoly(mask_conv, circle_ROI, Scalar(255));
    //imshow("mask_before", mask_conv);

    //dilate the mask -> region grows
    Mat mask_conv_dil;
    Mat element = getStructuringElement( MORPH_ELLIPSE,
                                         Size( 2*dilSize+1, 2*dilSize+1 ),
                                         Point(dilSize,dilSize) );
    dilate(mask_conv, mask_conv_dil, element);
    //imshow("mask_dil", mask_conv_dil);

    //bitwise AND on mask and dilerod
    bitwise_and(mask_conv_dil, dilerod, dispImg2);

    // findcontours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    unsigned int largest_contour_index;
    dilErodContours(dispImg2, contours, hierarchy, largest_contour_index, perimeter, dispImg1);

    // find the area of the cell by counting the white area inside the largest contour
    Mat cellArea = Mat::zeros(height, width, CV_8UC1);
    drawContours(cellArea, contours, largest_contour_index, Scalar(255), -1, 8, hierarchy, 0, Point() );
    //imshow("cellArea", cellArea);
    area = countNonZero(cellArea);

    //cout << "frame " << frameNum << "\n";
    //cout << contours[largest_contour_index] << endl;

    //renew circle points as the convex hull
    vector<Point> convHull;
    convexHull(contours[largest_contour_index], convHull);

    // find the centroid of the contour
    Moments mu = moments(contours[largest_contour_index]);
    ctroid = Point2f(mu.m10/mu.m00 + rect.x, mu.m01/mu.m00 + rect.y);

    // find the shape of the cell by the largest contour and centroid
    shape = findShape(ctroid, contours[largest_contour_index]);

    ////---- draw largest contour start ----
    //draw the largest contour
    Mat borderImg = Mat::zeros(height, width, CV_8UC1);
    drawContours(borderImg, contours, largest_contour_index, Scalar(255), 1, 8, hierarchy, 0, Point());
    //QString cellFileName0 = "border" + QString::number(frameNum) + ".png";
    //imwrite(cellFileName0.toStdString(), borderImg);


    Mat cell;
    bitwise_and(cellArea, sub, cell);
    //cell_alpha = createAlphaMat(cell);  // cell image with exactly the contour detected
    //vector<int> compression_params;
    //compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    //compression_params.push_back(9);
//    QString cellFileName1 = "cell" + QString::number(frameNum) + ".png";
//    imwrite(cellFileName1.toStdString(), cell_alpha, compression_params);
    ////---- draw largest contour end ----

    // find the number and the sizes of blebs of the cell
    Mat smooth;
    vector<Point> smoothCurve;
    int WIN = 25;
    smooth = curveSmooth(borderImg, WIN, contours[largest_contour_index], smoothCurve, convHull);
    //smooth = curveSmooth(borderImg, WIN, contours[largest_contour_index], smoothCurve, ctroid/*Point(ctroid.x, ctroid.y)*/);
    //drawPointVectors(dispImg1, smoothCurve, 1, Scalar(159, 120, 28));


    Mat smooth_contour;
    int w = 10;
    smooth_contour = curveSmooth(borderImg, w, contours[largest_contour_index], smooth_contour_curve, convHull);
    //smooth_contour = curveSmooth(borderImg, w, contours[largest_contour_index], smooth_contour_curve, ctroid/*Point(ctroid.x, ctroid.y)*/);
    //imshow("smooth_contour", smooth_contour);

    //cout << mask_conv_dil.type() << " " << sub.type() << endl;
    Mat cell_convex;
    bitwise_and(smooth_contour, sub, cell_convex);
    cell_alpha = createAlphaMat(cell_convex);
//    imshow("cell_convex_contour", cell_alpha);
    dispImg2 = cell_convex.clone();

    //change dispImg2 from gray to rgb for displaying
    cvtColor(dispImg2, dispImg2, CV_GRAY2RGB);

    bitwise_not(smooth, smooth);
    //Mat blebsImg;
    bitwise_and(smooth, cellArea, blebsImg);
//    imshow("blebs", blebsImg);
//    QString cellFileName2 = "blebs" + QString::number(frameNum) + ".png";
//    imwrite(cellFileName2.toStdString(), blebs);

    //QString cellFileName2 = "dispImg1" + QString::number(frameNum) + ".png";
    //imwrite(cellFileName2.toStdString(), dispImg1);

    cir_org.clear();
    for(unsigned int i = 0; i < convHull.size(); i++)
        cir_org.push_back(Point((convHull[i].x + rect.x)*scale, (convHull[i].y + rect.y)*scale));
}


void FindContour::boundingBox(Mat &img)
{
    img = frame->clone();
    //Rect rect = boundingRect(Mat(circle));
    //Scalar color(49, 204, 152); // draw a green rectangle on the image
    rectangle(img, rect, Scalar(49, 204, 152), 2);
}

