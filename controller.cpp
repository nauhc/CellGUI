#include "controller.h"
#include "qdebug.h"
#include "iostream"
#include <QFileInfo>


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
    if(inputVideo->isOpened()){
        inputVideo->release();
        delete inputVideo;
    }
    if(csvFile.is_open())
        csvFile.close();
    delete roiFrame;
//    delete frame; // Do not delete frame here. It has been released in FindContour class!
    delete contour;
    cout << "controller deleted" << endl;
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
    // 32-bit, 3 channel
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

bool Controller::loadVideo(string filename){
    inputVideo->open(filename);
    if(!inputVideo->isOpened()){
        cout << "Could not open the input video:" << filename << endl;
        return false;
    }
    else{

        cout << "Input video file: " << filename << " opened." << endl;
        frameCnt    = (int) inputVideo->get(CV_CAP_PROP_FRAME_COUNT);
        fps         = inputVideo->get(CV_CAP_PROP_FPS);
        videoSize   = Size((int) inputVideo->get(CV_CAP_PROP_FRAME_WIDTH),
                         (int) inputVideo->get(CV_CAP_PROP_FRAME_HEIGHT));
        cout << "video size: " << videoSize << "\n";
        cout << "frame count: " << frameCnt << "\n";
        cout << "fps:" << fps << endl;

        //prepare writing data to file
        QFileInfo   fi  = QFileInfo(QString::fromStdString(filename));
        QString     ff  = fi.path()+fi.baseName();
        string      fn  = ff.toUtf8().constData();
        const char* fnn = fn.c_str();
        //check if file exists, if exists delete the file
        ifstream ifile(fn);
        if(ifile){
            if(remove(fnn)!= 0)
                cout << "error deleting existing csv file" << endl;
        }

        //prepare to write data to file
        csvFile.open(fn+".csv", ios::out);

        if(!inputVideo->read(*frame)){
            cout << "Unable to retrieve the first frame from video stream." << endl;
            return false;
        }
        emit load1stImage(cvMatToQImage(*frame));
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

bool Controller::optflow(Mat &frame1, Mat &frame2, vector<Point2f> &points1, vector<Point2f> &points2){
    if(frame1.size() != frame2.size()){
        cout << "can not apply optflow on images of different sizes." << endl;
        return false;
    }

    Size imgSize = frame1.size();
    Mat frame1gray, frame2gray;
    cvtColor(frame1, frame1gray, CV_RGB2GRAY);
    cvtColor(frame2, frame2gray, CV_RGB2GRAY);

    int cornerCnt = 200;
    TermCriteria criteria = TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    vector<uchar> status;
    vector<float> errors;
    Size winSize = Size(11, 11);
    double quality = 0.01;
    double minDist = 2.0;

    points1.reserve(cornerCnt);
    points2.reserve(cornerCnt);

    goodFeaturesToTrack(frame1gray, points1, cornerCnt, quality, minDist, Mat()/*, 3, true, 0.04*/);
    goodFeaturesToTrack(frame2gray, points2, cornerCnt, quality, minDist, Mat()/*, 3, true, 0.04*/);
    cornerSubPix(frame1gray, points1, winSize, Size(-1, -1), criteria);
    cornerSubPix(frame2gray, points2, winSize, Size(-1, -1), criteria);

    status.reserve(cornerCnt);
    errors.reserve(cornerCnt);
    calcOpticalFlowPyrLK(frame1gray, frame2gray, points1, points2, status, errors, winSize, 10, criteria, 0);

//    // draw optflow
//    Mat flowFrame = frame1gray;
//    unsigned int size = status.size();
//    for(unsigned int i = 0; i < size; i++ ){
//        Point p0( ceil( points1[i].x ), ceil( points1[i].y ) );
//        Point p1( ceil( points2[i].x ), ceil( points2[i].y ) );
//        //std::cout << "(" << p0.x << "," << p0.y << ")" << "\n";
//        //std::cout << "(" << p1.x << "," << p1.y << ")" << std::endl;

//        //draw lines to visualize the flow
//        double angle = atan2((double) p0.y - p1.y, (double) p0.x - p1.x);
//        double length = 0.005 * (double) (imgSize.width);
//        line(flowFrame, p0, p1, Scalar(255), 1 );
//        Point p;
//        p.x = (int) (p1.x + length * cos(angle + 3.14/4));
//        p.y = (int) (p1.y + length * sin(angle + 3.14/4));
//        line(flowFrame, p, p1, Scalar(255), 1 );
//        p.x = (int) (p1.x + length * cos(angle - 3.14/4));
//        p.y = (int) (p1.y + length * sin(angle - 3.14/4));
//        line(flowFrame, p, p1, Scalar(255), 1 );
//    }
//    imshow("window", flowFrame);
//    cvWaitKey(10);

    return true;
}

void Controller::setScale(double scl){
    contour->setScale(scl);
}

void Controller::setCircle(QVector<QPoint> points)
{
    encircled = true;
    for(int i = 0; i < points.size(); i++){
        Point p =  Point(points[i].x(), points[i].y());
        hull.push_back(p);
    }
    //cout << "hull coordinates: " << hull << endl;
}

void Controller::setAdaptThresh(int var){
    contour->setAdaptThresh(double(var));
}

void Controller::setBlkSize(int var){
    contour->setBlkSize(2*var+1);
}

void Controller::run(){
    int delay = (1500/fps);


    int cnt = 0;
    while(!pause && cnt < frameCnt){
        if(!inputVideo->read(nextFrame)){
            cout << "Unable to retrieve frame from video stream." << endl;
            pause = true;
            continue;
        }
        cnt++;

        int frameIdx = inputVideo->get(CV_CAP_PROP_POS_FRAMES);
        //cout << "frame " << frameIdx << endl;

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
            int area; // area of the cell getting from cellDetection
            int perimeter; // perimeter of the cell getting from cellDetection
            Point2f centroid; // centroid of the cell getting from cellDetection

            // optflow detection of entire frame
            vector<Point2f> points1, points2;
            bool opt = optflow(*frame, nextFrame, points1, points2);
            if (!opt){
                cout << "optical flow detection failed on frame " << frameIdx << endl;
                continue;
            }

            contour->cellDetection(*frame, hull, contourImg, edgeImg,
                                   points1, points2,
                                   area, perimeter, centroid, frameIdx);
            floatArray property;
            property.push_back(float(area));
            property.push_back(float(perimeter));
            property.push_back(centroid.x);
            property.push_back(centroid.y);
            property.push_back(0.0);
            property.push_back(0.0);
            csvFile << frameIdx << "," << area << "," << perimeter << "," << centroid << endl;

            emit detectedProperties(property);
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

        *frame = nextFrame.clone();

        //emit the singnals
        emit processedImage(img, roiImg1, roiImg2);
        this->msleep(delay);
    }

    if(cnt==frameCnt){
        csvFile.close();
        cout << "data file (csv) saved." << endl;
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
