#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <QImage>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <findcontour.h>

using namespace std;
using namespace cv;

class Controller : public QThread
{   Q_OBJECT
private:
    VideoCapture *inputVideo; //video processing -- openCV
    bool stop;
    Mat *frame; // the next frame
    //Mat *prevFrame; // frame from the video
    //Mat *nextFrame; // the one after the next frame
    Mat *RGBframe; // color frame from the video
    Mat *roiFrame; // roi rectangle from frame
    QImage img; // QImage for displaying
    QImage roiImg; // QImage for ROI for displaying

    Size videoSize; // video frame size
    int frameCnt; // total frame number
    double fps; // fps

    FindContour *contour;

signals:
    // signal nonnectted with updateVideoplayerUI SLOT
    void processedImage(QImage image, QImage ROIimg);
private slots:
    void setAdaptThresh(int var);
    void setBlkSize(int var);

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
