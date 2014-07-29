#include "controller.h"
#include "qdebug.h"

Controller::Controller(QObject *parent) : QThread(parent),
   inputVideo(new VideoCapture()),
   frame(new Mat()), roiFrame(new Mat()),
   contour(new FindContour())
{
    cout << "controller initialzed." << endl;
    pause = true;
    encircled = false;
}
Controller::~Controller(){
    if(inputVideo->isOpened())
        inputVideo->release();
        delete inputVideo;
    delete roiFrame;
//    delete frame; // Do not delete frame here. It has been released in FindContour class!
    delete contour;
    cout << "controller deleted" << endl;
}


bool Controller::loadVideo(string filename){
    inputVideo->open(filename);
    if(!inputVideo->isOpened()){
        cout << "Could not open the input video:" << filename << endl;
        return false;
    }
    else{
        cout << "Input video file:" << filename << " opened." << endl;
        frameCnt    = (int) inputVideo->get(CV_CAP_PROP_FRAME_COUNT);
        fps         = inputVideo->get(CV_CAP_PROP_FPS);
        videoSize   = Size((int) inputVideo->get(CV_CAP_PROP_FRAME_WIDTH),
                         (int) inputVideo->get(CV_CAP_PROP_FRAME_HEIGHT));
        cout << "video size: " << videoSize << "\n";
        cout << "frame count: " << frameCnt << "\n";
        cout << "fps:" << fps << endl;

        return true;
    }
}

void Controller::playVideo(){
    if((!isRunning())){
        if(videoIsPaused()){
            pause = false;
        }
        start(LowPriority);
    }
}

void Controller::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

bool Controller::videoIsNull(){
    if(inputVideo->isOpened())
        return false;
    else
        return true;
}

void Controller::releaseVideo(){
    // release video
    inputVideo->release();
    //clear contour
    encircled = false;
    hull.clear();
}


void Controller::getVideoSize(int &width, int &height)
{
    width = videoSize.width;
    height = videoSize.height;
}

void Controller::setCircle(QVector<QPoint> points)
{
    encircled = true;
    for(int i = 0; i < points.size(); i++){
        Point p =  Point(points[i].x(), points[i].y());
        hull.push_back(p);
    }
}

void Controller::setAdaptThresh(int var){
    contour->setAdaptThresh(double(var));
}

void Controller::setBlkSize(int var){
    contour->setBlkSize(2*var+1);
}

inline QImage cvMatToQImage(const cv::Mat &inMat){
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );
        return image;
    }
    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );
        return image.rgbSwapped();
    }
    case CV_32FC3:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );
        return image.rgbSwapped();
    }
    // 8-bit, 1 channel
    case CV_8UC1:
    {
        //cout << "cv_8uc1 to qimage" << endl;
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );
        return image;
    }
    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }
    return QImage();
}

void Controller::run(){
    int delay = (1500/fps);
    while(!pause){
            if(!inputVideo->read(*frame)){
                //cout << inputVideo->get(CV_CAP_PROP_POS_FRAMES) << endl;
                cout << "Unable to retrieve frame from video stream." << endl;
                pause = true;
                continue;
            }

            int frameIdx = inputVideo->get(CV_CAP_PROP_POS_FRAMES);
            cout << "frame " << frameIdx << endl;

            QImage          roiImg1; // QImage for ROI for displaying
            QImage          roiImg2; // QImage for ROI for displaying
            if(!encircled){
                img = cvMatToQImage(*frame);
                roiImg1 = img;
                roiImg2 = img;
            }
            else{
                //draw bounding box on ROI and show in original video player

                Mat boxedImg = Mat(frame->rows, frame->cols, CV_8UC3);
                contour->boundingBox(boxedImg);
                img = cvMatToQImage(boxedImg);

                Mat contourImg;
                Mat edgeImg;
                int area;
                int perimeter;
                contour->cellDetection(*frame, hull, contourImg, edgeImg, area, perimeter);
                emit detectedArea(area, perimeter);
                //cout << "frame " << frameIdx << " cell area: " << area << endl;
                roiImg1 = cvMatToQImage(contourImg);
                roiImg2 = cvMatToQImage(edgeImg);
            }

            /*
            //ROI
            int x = videoSize.width/2 - 5;
            int y = videoSize.height/2 + 10;
            int width = 150;
            int height = 100;

            contour->getROI(*frame, x, y, width, height);
            //adaptive threshold for getting edges from current image
            Mat edgeImg;
            contour->edgeDetection(edgeImg);
            roiImg = cvMatToQImage(edgeImg); //Mat to QImage for display

            if (frameIdx <= 80)
                roiImg = img;
            else{
                //bounding box
                Mat boxImg;
                contour->boundingBox(boxImg);
                img = cvMatToQImage(boxImg);

            }
            */

            //emit the singnals
            emit processedImage(img, roiImg1, roiImg2);
            this->msleep(delay);
        }
}

void Controller::pauseVideo(){
    pause = true;
}

bool Controller::videoIsPaused(){
    return this->pause;
}

double Controller::getNumberOfFrames(){
    return frameCnt;
}

double Controller::getCurrentFrame(){
    return inputVideo->get(CV_CAP_PROP_POS_FRAMES);
}
