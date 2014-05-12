#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <QImage>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


using namespace std;
using namespace cv;

class Controller : public QThread
{   Q_OBJECT
private:
    VideoCapture *inputVideo;
    bool stop;
    Mat *frame;
    Mat *RGBframe;
    QImage img;

    Size videoSize;
    int frameCnt;
    double fps;



signals:
    void processedImage(QImage image);

protected:
    void run();
    void msleep(int ms);

public:
    Controller(QObject *parent=0);
    ~Controller();

    bool isNull();
    void play();
    void Stop();
    bool isStopped();
    bool loadVideo(string filename);
    void playVideo();

};

#endif // CONTROLLER_H
