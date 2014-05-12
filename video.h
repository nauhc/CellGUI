#ifndef VIDEOPROC_H
#define VIDEOPROC_H
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

using namespace std;
using namespace cv;

class Video
{
private:
    VideoCapture inputVideo;

public:
    Size    videoSize;
    int     frameCnt;
    double  fps;

    Video();
    ~Video();
    bool readVideo(const string &filename); //read video from file
    void displayVideo(); //show the video
    void edgeDetection();
    void displayAdaptiveThreshold();
};

#endif // VIDEOPROC_H
