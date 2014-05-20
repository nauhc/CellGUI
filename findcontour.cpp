#include "findcontour.h"

FindContour::FindContour(): /*currFrame(new Mat()), prevFrame(new Mat()), nextFrame(new Mat()),*/
                            currROI(new Mat()), prevROI(new Mat()), nextROI(new Mat()){
}

FindContour::~FindContour(){
//    delete prevFrame;
//    delete currFrame;
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

void FindContour::getInitialROI(const Mat &currImg, const Mat &prevImg, const Mat &nextImg,
                                int x, int y, int width, int height){
//    x_start = x > 0 ? x : 0;
//    x_stop  = currImg.cols > (x_start + width) ? (x_start + width) : currImg.cols;
//    y_start = y > 0 ? y : 0;
//    y_stop  = currImg.rows > (y_start + height) ? (y_start + height) : currImg.rows;

    Rect roi_rect = Rect(x, y, width, height);
    Mat currSub = currImg(roi_rect);
    Mat prevSub = prevImg(roi_rect);
    Mat nextSub = nextImg(roi_rect);
    cv::cvtColor(currSub, *currROI, CV_RGB2GRAY); // convert color image to grayscale image
    cv::cvtColor(prevSub, *prevROI, CV_RGB2GRAY); // convert color image to grayscale image
    cv::cvtColor(nextSub, *nextROI, CV_RGB2GRAY); // convert color image to grayscale image
    x_start = 0;
    x_stop  = currSub.cols;
    y_start = 0;
    y_stop  = currSub.rows;
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
        for(int j = y_start; j < y_stop; j+=2){ // height
            for(int i = x_start; i < x_stop; i+=2){ // width
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
        cout << "check02" << endl;
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
    bitwise_and(d1, d2, motion);
    threshold(motion, motion, 35, 255, CV_THRESH_BINARY);
    cout << "check1" << endl;
//    Mat kernel_ero = getStructuringElement(MORPH_RECT, Size(2,2));
//    erode(motion, motion, kernel_ero);
    Scalar color(0,255,255);
    cout << "check2" << endl;
    int max_deviation = 20;
    int num_of_changes = detectMotion(motion, result, result_cropped,  x_start, x_stop, y_start, y_stop, max_deviation, color);
    cout << "num_of_changes " << num_of_changes << endl;
    imwrite("../../../video/result_cropped.tiff", result_cropped);
    int there_is_motion = 5;
    if(num_of_changes >= there_is_motion){
        cout << "there is motion!" << endl;
    }
}


void FindContour::edgeDetection(Mat &adapThreshImg){
//    int blockSize = 17;
//    double constValue = 7;
//    adaptiveThreshold(*roi, adapThreshImg, 255.0, CV_ADAPTIVE_THRESH_MEAN_C,
//                      CV_THRESH_BINARY_INV, blockSize, constValue);
    adaptiveThreshold(*currROI, adapThreshImg, 255.0, ADAPTIVE_THRESH_GAUSSIAN_C,
                      CV_THRESH_BINARY_INV, blockSize, constValue);

}
