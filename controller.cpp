#include "controller.h"
int cnt = 0;

Controller::Controller(QObject *parent) : QThread(parent),
   inputVideo(new VideoCapture()),
   prevFrame(new Mat()), currFrame(new Mat()), nextFrame(new Mat()),
   RGBframe(new Mat()), roiFrame(new Mat()),
   contour(new FindContour())
{
    stop = true;
}
Controller::~Controller(){
    delete contour;
    delete roiFrame;
    delete prevFrame;
    delete currFrame;
    delete nextFrame;
    delete RGBframe;
    delete inputVideo;
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
        frameCnt    = (int) inputVideo->get(CV_CAP_PROP_FRAME_COUNT);
        fps         = (int) inputVideo->get(CV_CAP_PROP_FPS);
        videoSize = Size((int) inputVideo->get(CV_CAP_PROP_FRAME_WIDTH),
                         (int) inputVideo->get(CV_CAP_PROP_FRAME_HEIGHT));
        return true;
    }
}

void Controller::play(){
    if((!isRunning())){
        if(isStopped()){
            stop = false;
        }
        start(LowPriority);
    }
}

void Controller::msleep(int ms){
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

bool Controller::isNull(){
    if(inputVideo->isOpened())
        return false;
    else
        return true;
}

void Controller::setAdaptThresh(int var){
    contour->setAdaptThresh(double(var));
}

void Controller::setBlkSize(int var){
    contour->setBlkSize(2*var+1);
}

void Controller::run(){
    int delay = (1000/fps);
    cout << cnt ++ << endl;
    if(!inputVideo->read(*prevFrame) || !inputVideo->read(*currFrame) || !inputVideo->read(*nextFrame)){
        stop = true;
    }

    while(!stop){

        //ROI
        int x = videoSize.width/2- 10;
        int y = videoSize.height/2 + 10;
        int width = 120;
        int height = 100;

        contour->getInitialROI(*prevFrame, *currFrame, *nextFrame, x, y, width, height);
        contour->traceMotionROI();
        Mat edgeImg;
        contour->edgeDetection(edgeImg);
        roiImg = QImage((const unsigned char*)(edgeImg.data),
                        edgeImg.cols,edgeImg.rows,QImage::Format_Indexed8);

        prevFrame = currFrame; // renew previous frame
        currFrame = nextFrame; // renew current frame
        if(!inputVideo->read(*nextFrame)){ // renew next frame
            stop = true;
        }


        //Mat to QImage
        if((*prevFrame).channels()==3){
            cv::cvtColor(*prevFrame, *RGBframe, CV_BGR2RGB);
            img = QImage((const unsigned char*)((*RGBframe).data),
                         (*RGBframe).cols, (*RGBframe).rows, QImage::Format_RGB888);
        }
        else{
            img = QImage((const unsigned char*)((*prevFrame).data),
                         (*prevFrame).cols,(*prevFrame).rows,QImage::Format_Indexed8);
        }



        //emit the singnals
        emit processedImage(img, roiImg);
        this->msleep(delay);
    }
}

void Controller::Stop(){
    stop = true;
}

bool Controller::isStopped(){
    return this->stop;
}
