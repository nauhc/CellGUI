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
    VideoCapture    *inputVideo; //video processing -- openCV
    bool            stop;
    Mat             *frame; // frame from the video
//    Mat             *RGBframe; // color frame from the video
    Mat             *roiFrame; // roi rectangle from frame
    QImage          img; // QImage for displaying
    QImage          roiImg; // QImage for ROI for displaying

    Size            videoSize; // video frame size
    int             frameCnt; // total frame number
    double          fps; // fps

    FindContour     *contour;
    bool            circled;
    vector<Point>   circle;

signals:
    void    processedImage(QImage image, QImage ROIimg); // signal nonnectted with updateVideoplayerUI SLOT
private slots:
    void    setAdaptThresh(int var);
    void    setBlkSize(int var);

protected:
    void    run();
    void    msleep(int ms);

public:
    Controller(QObject *parent=0);
    ~Controller();

    bool    videoIsNull();
    void    playVideo();
    void    stopVideo();
    bool    videoIsStopped();
    bool    loadVideo(string filename);
    double  getNumberOfFrames();
    double  getCurrentFrame();
    void    releaseVideo();
    void    getVideoSize(int &width, int &height);

    void    setCircle(QVector<QPoint> points);


};

#endif // CONTROLLER_H
