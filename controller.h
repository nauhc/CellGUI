#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "fstream"
#include <QThread>
#include <QImage>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <findcontour.h>
#include <video/tracking.hpp>
#include <bleb.h>
#include <QDir>
#include <QFile>

using namespace std;
using namespace cv;


typedef vector<float> floatArray;

class Controller : public QThread
{   Q_OBJECT
private:

    VideoCapture    *inputVideo; //video processing -- openCV
    bool            pause;
    QMutex          pauseMutex;
    //Mat             previousFrame;
    Mat             nextFrame;
    Mat             *frame; // frame from the video
    Mat             *roiFrame; // roi rectangle from frame
    QImage          img; // QImage for displaying video (with box)

    Size            videoSize; // video frame size
    int             frameCnt; // total frame number
    double          fps; // fps

    string          filepath; // filepath for a particular movie
    ofstream        csvFile;// output to file
    QFile           blebsFile; // output blebs positions to file
    QFile           contourFile; // output contours to file

    FindContour     *contour; // findcontour class object
    bool            encircled;
    vector<Point>   hull; // contour of the cell of interest
    double          scale; // scale = display / real size
    double          pixel;
    double          micMeter;
    double          micMtr_Pixel;

    int             videoType; // 1.single cell 2.fix window 3.flexible

    int             WINSIZE = 10;
    int             BIN = 24;
    double          blebSizeRatio;
    QList<Mat>      blebsImgWIN; // ROI images in a time window
    QList<Rect>     rectWIN; // the rects of ROI images a time window
    void            findBlebs(int &area, Point2f &centroid, int &BIN, vector<Bleb> &blebs);

signals:
    void    load1stImage(QImage image);
    // signal nonnectted with updateVideoplayerUI SLOT
    void    processedImage(QImage image, QImage ROIimg1, QImage ROIimg2);
    void    detectedProperties(floatArray prop);
    void    detectedCellImg(QImage cell, QVector<QPoint> smoothContour);

private slots:
    void    setAdaptThresh(int var);
    void    setBlkSize(int var);
    void    setDilSize(int var);
    void    setblebSizeRatio(int var);
    void    setVideoType(int tp);
    void    setPixel(QString text);
    void    setMicMeter(QString text);

protected:
    void    run();
    void    msleep(int ms);

public:
    bool            compressedCell; // true:compressed cell, false:control cell

    Controller(QObject *parent=0);
    ~Controller();

    bool    videoIsNull();
    void    playVideo();
    void    pauseVideo();
    bool    videoIsPaused();
    bool    loadVideo(string file, string ff, string fb);
    double  getNumberOfFrames();
    double  getCurrentFrame();
    void    releaseVideo();
    QSize   getVideoSize(/*int &width, int &height*/);

    bool    optflow(Mat &frame1, Mat &frame2, vector<Point2f> &points1, vector<Point2f> &points2);

    void    setScale(double scl);
    void    setCircle(QVector<QPoint> points);

};

#endif // CONTROLLER_H
