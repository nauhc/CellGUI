#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <QImage>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <findcontour.h>
#include <video/tracking.hpp>

using namespace std;
using namespace cv;

class Controller : public QThread
{   Q_OBJECT
private:
    VideoCapture    *inputVideo; //video processing -- openCV
    bool            pause;
    Mat             previousFrame;
    Mat             *frame; // frame from the video
    Mat             *roiFrame; // roi rectangle from frame
    QImage          img; // QImage for displaying video (with box)

    Size            videoSize; // video frame size
    int             frameCnt; // total frame number
    double          fps; // fps



    FindContour     *contour; // findcontour class object
    bool            encircled;
    vector<Point>   hull; // contour of the cell of interest
    //double          scale; // scale = display / real size

signals:
    // signal nonnectted with updateVideoplayerUI SLOT
    void    processedImage(QImage image, QImage ROIimg1, QImage ROIimg2);
    void    detectedArea(int area, int perimeter);

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
    void    pauseVideo();
    bool    videoIsPaused();
    bool    loadVideo(string filename);
    double  getNumberOfFrames();
    double  getCurrentFrame();
    void    releaseVideo();
    void    getVideoSize(int &width, int &height);

    bool    optflow(Mat &frame1, Mat &frame2, vector<Point2f> &points1, vector<Point2f> &points2);

    void    setScale(double scl);
    void    setCircle(QVector<QPoint> points);

};

#endif // CONTROLLER_H
