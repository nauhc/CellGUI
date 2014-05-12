#include "controller.h"

Controller::Controller(QObject *parent) : QThread(parent),
   inputVideo(new VideoCapture()),
   frame(new Mat()), RGBframe(new Mat())
{
    stop = true;
}
Controller::~Controller(){
    delete frame;
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

void Controller::run(){
    int delay = (1000/fps);
    while(!stop){
//            Mat *frame = new Mat();
            if(!inputVideo->read(*frame)){
                stop = true;
            }

            //Mat to QImage
            if((*frame).channels()==3){
                cv::cvtColor(*frame, *RGBframe, CV_BGR2RGB);
                img = QImage((const unsigned char*)((*RGBframe).data),
                             (*RGBframe).cols, (*RGBframe).rows, QImage::Format_RGB888);
            }
            else{
                img = QImage((const unsigned char*)((*frame).data),
                             (*frame).cols,(*frame).rows,QImage::Format_Indexed8);
            }
            emit processedImage(img);
            this->msleep(delay);
        }
}

void Controller::Stop(){
    stop = true;
}

bool Controller::isStopped(){
    return this->stop;
}
