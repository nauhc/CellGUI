#include "findcontour.h"

FindContour::FindContour(): currFrame(new Mat()), /*prevFrame(new Mat()), nextFrame(new Mat()),*/
                            currROI(new Mat()), prevROI(new Mat()), nextROI(new Mat()){
}

FindContour::~FindContour(){
    delete currFrame;
//    delete prevFrame;
//    delete nextFrame;
    delete currROI;
    delete prevROI;
    delete nextROI;
}

void FindContour::setAdaptThresh(double para1){
    constValue = para1;
}

void FindContour::setBlkSize(int para2){
    blockSize = para2;
}

void FindContour::getROI(const Mat &img, int x, int y, int width, int height){
    currFrame = &img;
    //cv::cvtColor(img, *currFrame, CV_RGB2GRAY);

    x_start = x;
    y_start = y;
    x_stop  = (x + width) > img.cols ? img.cols : (x + width);
    y_stop  = (y + height) > img.rows ? img.rows :(x + width);

    Rect roi_rect = Rect(x, y, width, height);
    //Rect roi_rect = Rect( Point(x_start, y_start), Point(x_stop, y_stop) );
    Mat sub = (*currFrame)(roi_rect);
    cv::cvtColor(sub, *currROI, CV_RGB2GRAY); // convert color image to grayscale image
    //imwrite("../../../video/roigray.tiff", *roi);
}

void FindContour::getInitialROI(const Mat &currImg,
                                const Mat &prevImg,
                                const Mat &nextImg,
                                int x, int y, int width, int height){
    x_start = x;
    y_start = y;
    x_stop  = x + width;
    y_stop  = y + height;


    currFrame = &currImg;

    Rect roi_rect = Rect(x, y, width, height);
    Mat currSub = currImg(roi_rect);
    Mat prevSub = prevImg(roi_rect);
    Mat nextSub = nextImg(roi_rect);
    cv::cvtColor(currSub, *currROI, CV_RGB2GRAY); // convert color image to grayscale image
    cv::cvtColor(prevSub, *prevROI, CV_RGB2GRAY); // convert color image to grayscale image
    cv::cvtColor(nextSub, *nextROI, CV_RGB2GRAY); // convert color image to grayscale image

}

inline int detectMotion(const Mat & motion, Mat & result, Mat & result_cropped,
                 int x_start, int x_stop, int y_start, int y_stop,
                 int max_deviation,
                 Scalar & color)
{
    // calculate the standard deviation
    Scalar mean, stddev;
    meanStdDev(motion, mean, stddev);
    cout << "mean: " << mean << " stddev: " << stddev << endl;
    // if not to much changes then the motion is real (neglect agressive snow, temporary sunlight)
    if(stddev[0] < max_deviation)
    {
        cout << "check00" << endl;
        int number_of_changes = 0;
        int min_x = motion.cols, max_x = 0;
        int min_y = motion.rows, max_y = 0;
        // loop over image and detect changes
        cout << "check01" << endl;
        for(int j = y_start; j < y_stop; j++){ // height
            for(int i = x_start; i < x_stop; i++){ // width
                // check if at pixel (j,i) intensity is equal to 255
                // this means that the pixel is different in the sequence
                // of images (prev_frame, current_frame, next_frame)
                if(static_cast<int>(motion.at<uchar>(j,i)) == 255)
                {
                    number_of_changes++;
                    if(min_x>i) min_x = i;
                    if(max_x<i) max_x = i;
                    if(min_y>j) min_y = j;
                    if(max_y<j) max_y = j;
                }
            }
        }
        cout << "number_of_changes " << number_of_changes<< endl;
        if(number_of_changes){
            //check if not out of bounds
            if(min_x-10 > 0) min_x -= 10;
            if(min_y-10 > 0) min_y -= 10;
            if(max_x+10 < result.cols-1) max_x += 10;
            if(max_y+10 < result.rows-1) max_y += 10;
            // draw rectangle round the changed pixel
            Point x(min_x,min_y);
            Point y(max_x,max_y);
            Rect rect(x,y);
            Mat cropped = result(rect);
            cropped.copyTo(result_cropped);
            rectangle(result,rect,color,1);
        }
        return number_of_changes;
    }
    return 0;
}


void FindContour::traceMotionROI(){
    imwrite("../../../video/prevROI.tiff", *prevROI);
    imwrite("../../../video/currROI.tiff", *currROI);
    imwrite("../../../video/nextROI.tiff", *nextROI);
    Mat d1, d2, motion;
    Mat result, result_cropped= *nextROI;
    absdiff(*prevROI, *nextROI, d1);
    absdiff(*nextROI, *currROI, d2);
    int num_of_changes_d1 = 0;
    int num_of_changes_d2 = 0;
    for(int j = y_start; j < y_stop; j++){ // height
        for(int i = x_start; i < x_stop; i++){ // width
            // check if at pixel (j,i) intensity is equal to 255
            // this means that the pixel is different in the sequence
            // of images (prev_frame, current_frame, next_frame)
            if(static_cast<int>(d1.at<uchar>(j,i)) == 255)
            {
                num_of_changes_d1++;
                num_of_changes_d2++;
            }
        }
    }
    cout << "num_of_changes_d1 " << num_of_changes_d1 << endl;
    cout << "num_of_changes_d2 " << num_of_changes_d2 << endl;
    bitwise_and(d1, d2, motion);
    threshold(motion, motion, /*35*/5, 255, CV_THRESH_BINARY);
    cout << "check1" << endl;
//    Mat kernel_ero = getStructuringElement(MORPH_RECT, Size(2,2));
//    erode(motion, motion, kernel_ero);
    Scalar color(0,255,255);
    cout << "check2" << endl;
    int max_deviation = 20;
    int num_of_changes = detectMotion(motion, result, result_cropped,  x_start, x_stop, y_start, y_stop, max_deviation, color);
    imwrite("../../../video/result_cropped.tiff", result_cropped);
    int there_is_motion = 5;
    if(num_of_changes >= there_is_motion){
        cout << "there is motion!" << endl;
    }
}

void FindContour::edgeDetection(Mat &adaptThreshImg){
    adaptiveThreshold(*currROI, adaptThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                      CV_THRESH_BINARY_INV, blockSize, constValue);
}

void FindContour::boundingBox(Mat &img){

    Mat thresholdImg;
    adaptiveThreshold(*currROI, thresholdImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                      CV_THRESH_BINARY_INV, blockSize, constValue);
    //cv::cvtColor(*currFrame, img, CV_RGB2GRAY);
    img = (*currFrame).clone();
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
        Scalar color(0,255,255);
        rectangle(img, rect, color, 1);

    }

}
