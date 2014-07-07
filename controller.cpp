#include "controller.h"
#include "QDebug"

Controller::Controller(QObject *parent) : QThread(parent),
   inputVideo(new VideoCapture()),
   frame(new Mat()), roiFrame(new Mat()), /* RGBframe(new Mat()),*/
   contour(new FindContour())
{
    stop = true;
    circled = false;
}
Controller::~Controller(){
    if(inputVideo->isOpened())
        inputVideo->release();
        delete inputVideo;
    delete roiFrame;
//    delete frame; // Do not delete frame here. It has been released in FindContour class!
    //delete RGBframe;
    delete contour;
}


bool Controller::loadVideo(string filename){
//    const string source = "/Users/chuanwang/Sourcecode/Cell/video/movie.avi";
//    Video *inputVideo = new Video();
//    if(inputVideo->readVideo(source)){
//        inputVideo->edgeDetection();
//    }
    inputVideo->open(filename);
    if(!inputVideo->isOpened()){
        cout << "Could not open the input video:" << filename << endl;
        return false;
    }
    else{
        cout << "Input video file:" << filename << " opened." << endl;
        frameCnt    = (int) inputVideo->get(CV_CAP_PROP_FRAME_COUNT);
        fps         = inputVideo->get(CV_CAP_PROP_FPS);
        videoSize = Size((int) inputVideo->get(CV_CAP_PROP_FRAME_WIDTH),
                         (int) inputVideo->get(CV_CAP_PROP_FRAME_HEIGHT));
        cout << "video size: " << videoSize << "\n";
        cout << "frame count: " << frameCnt << "\n";
        cout << "fps:" << fps << endl;

        return true;
    }
}

void Controller::playVideo(){
    if((!isRunning())){
        if(videoIsStopped()){
            stop = false;
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
    inputVideo->release();
}

void Controller::getVideoSize(int &width, int &height)
{
    width = videoSize.width;
    height = videoSize.height;
}

void Controller::setCircle(QVector<QPoint> points)
{
    circled = true;
    for(int i = 0; i < points.size(); i++){
        Point p =  Point(points[i].x(), points[i].y());
        circle.push_back(p);
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
    // 8-bit, 1 channel
    case CV_8UC1:
    {
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
    int delay = (1000/fps);
    while(!stop){
            if(!inputVideo->read(*frame)){
                //cout << inputVideo->get(CV_CAP_PROP_POS_FRAMES) << endl;
                cout << "Unable to retrieve frame from video stream." << endl;
                stop = true;
                continue;
            }

            int frameIdx = inputVideo->get(CV_CAP_PROP_POS_FRAMES);

            //Mat to QImage for display
            img = cvMatToQImage(*frame);
            //roiImg = cvMatToQImage(Mat::zeros(roiFrame->size(), CV_8UC1));

            if(!circled)
                roiImg = img;
            else{
                contour->getROI(*frame, circle);
                Mat edgeImg;
                contour->edgeDetection(edgeImg);
                roiImg = cvMatToQImage(edgeImg);
//                Mat boxImg;
//                contour->boundingBox(boxImg, circle);
//                img = cvMatToQImage(boxImg);

            }


//            //ROI
//            int x = videoSize.width/2 - 5;
//            int y = videoSize.height/2 + 10;
//            int width = 150;
//            int height = 100;

//            contour->getROI(*frame, x, y, width, height);
//            //adaptive threshold for getting edges from current image
//            Mat edgeImg;
//            contour->edgeDetection(edgeImg);
//            roiImg = cvMatToQImage(edgeImg); //Mat to QImage for display

//            if (!circled)
//                roiImg = img;
//            else{
//                //bounding box
//                Mat boxImg;
//                contour->boundingBox(boxImg);
//                img = cvMatToQImage(boxImg);

//            }


            //emit the singnals
            emit processedImage(img, roiImg);
            this->msleep(delay);
        }
}

void Controller::stopVideo(){
    stop = true;
}

bool Controller::videoIsStopped(){
    return this->stop;
}

double Controller::getNumberOfFrames(){
    return frameCnt;
}

double Controller::getCurrentFrame(){
    return inputVideo->get(CV_CAP_PROP_POS_FRAMES);
}
