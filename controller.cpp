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
    compressedCell = true;
    pause = true;
    encircled = false;
    scale = 1.0;
    pixel = 300.0;
    micMeter = 20.0;
    micMtr_Pixel = micMeter/pixel;
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
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_ARGB32 );
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

bool Controller::loadVideo(string file, string fn){
    inputVideo->open(file);
    if(!inputVideo->isOpened()){
        cout << "Could not open the input video:" << file << endl;
        return false;
    }
    else{

        cout << "Input video file: " << file << " opened." << endl;
        frameCnt    = (int) inputVideo->get(CV_CAP_PROP_FRAME_COUNT);
        fps         = inputVideo->get(CV_CAP_PROP_FPS);
        videoSize   = Size((int) inputVideo->get(CV_CAP_PROP_FRAME_WIDTH),
                         (int) inputVideo->get(CV_CAP_PROP_FRAME_HEIGHT));
        cout << "video size: " << videoSize << "\n";
        cout << "frame count: " << frameCnt << "\n";
        cout << "fps:" << fps << endl;

        const char* fnn = fn.c_str();
        //check if file exists, if exists delete the file
        ifstream ifile(fn);
        if(ifile){
            if(remove(fnn)!= 0)
                cout << "error deleting existing csv file" << endl;
        }

        //prepare to write data to file

        std::transform(fn.begin(), fn.end(), fn.begin(), ::tolower);
        if(compressedCell)
            fn = fn + "_compressed";
        else
            fn = fn + "_control";
        cout << "writing data to file " << fn+".csv" << endl;
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
            pauseMutex.lock();
            pause = false;
            pauseMutex.unlock();
        }
        start(LowPriority); // start controller thread
    }
}

void Controller::msleep(int ms)
{
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}

bool Controller::videoIsNull()
{
    if(inputVideo->isOpened())
        return false;
    else
        return true;
}

void Controller::releaseVideo()
{
    // release video
    inputVideo->release();
    //clear contour
    encircled = false;
    hull.clear();
    csvFile.close();
}


QSize Controller::getVideoSize(/*int &width, int &height*/)
{
    return(QSize(videoSize.width, videoSize.height));
//    width = videoSize.width;
//    height = videoSize.height;
}

bool Controller::optflow(Mat &frame1, Mat &frame2, vector<Point2f> &points1, vector<Point2f> &points2){
    if(frame1.size() != frame2.size()){
        cout << "can not apply optflow on images of different sizes." << endl;
        return false;
    }

    //Size imgSize = frame1.size();
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

void Controller::setScale(double scl)
{
    scale = scl;
    contour->setScale(scale);
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

void Controller::setAdaptThresh(int var)
{
    contour->setAdaptThresh(double(var));
}

void Controller::setBlkSize(int var)
{
    contour->setBlkSize(2*var+1);
}

void Controller::setDilSize(int var){
    contour->setDilSize(var);
}

void Controller::setblebSizeRatio(int var)
{
    blebSizeRatio = 1.0/double((11-var)*50);
    //cout << "blebSizeRatio changed to: 1/" << (11-var)*50 << endl;
    contour->setblebSizeRatio(var);
}

void Controller::setVideoType(int tp)
{
    videoType = tp;
    switch (videoType) {
    case 0:
        cout << "video Type: Single cell video. \n" << endl;
        break;
    case 1:
        cout << "video Type: fix window video. \n" << endl;
        break;
    case 2:
        cout << "video Type: flexible video. \n" << endl;
        break;
    default:
        cout << "video Type: fix window video. \n" << endl;
        break;
    }
}

void Controller::setPixel(QString text)
{
    pixel = text.toDouble();
    micMtr_Pixel = micMeter/pixel;
    cout << "micrometerPerPixel: " << micMtr_Pixel << endl;
}

void Controller::setMicMeter(QString text)
{
    micMeter = text.toDouble();
    micMtr_Pixel = micMeter/pixel;
    cout << "micrometerPerPixel: " << micMtr_Pixel << endl;
}

// --- connected component begin ---
void search(Mat &img, int &label, int j, int i, int &cnt, vector<Point> &bunch){
    img.at<uchar>(j,i) = label;
    bunch.push_back(Point(i, j));
    cnt++;
    for(int jj = -1; jj <= 1; jj++){
        if(j+jj < 0 || j+jj > img.rows)
            continue;
        for(int ii = -1; ii <= 1; ii++){
            if(i+ii < 0 || i+ii > img.cols)
                continue;
            if(ii==0 && jj==0)
                continue;
            if(img.at<uchar>(j+jj, i+ii) == 255)
                search(img, label, j+jj, i+ii, cnt, bunch);
        }
    }
}
void find_component(Mat &img, int &label, int &thr, Point2f &centroid, int &BIN, vector<Bleb> &blebs){
    for(int j = 0; j < img.rows; j++){
        for(int i = 0; i < img.cols; i++){
            if(img.at<uchar>(j, i) == 255){
                int cnt = 0;
                label += 1;
                vector<Point> bunch;
                search(img, label, j, i, cnt, bunch);
                if(bunch.size()>= thr){
                    Bleb bleb(bunch, centroid, BIN);
                    blebs.push_back(bleb);
                }
            }
        }
    }
}
void recursive_connected_components(Mat &src, int &thr, Point2f &centroid, int &BIN, vector<Bleb> &blebs){
    Mat img_label = src.clone();
    int label = 0; // start from 1
    find_component(img_label, label, thr, centroid, BIN, blebs);
}
// --- connected component end ---

void Controller::findBlebs(int &area, Point2f &centroid, int &BIN, vector<Bleb> &blebs){
    Mat overlapBlebs = Mat::zeros(frame->rows, frame->cols, CV_8UC1);
    for(int n = 0; n < WINSIZE; n++){
        //imshow("blebsImg"+to_string(n), blebsImgWIN[n]);
        Rect rect_tmp = rectWIN[n];
        for(int j = rect_tmp.y; j < rect_tmp.y+rect_tmp.height; j++){
            for(int i = rect_tmp.x; i < rect_tmp.x+rect_tmp.width; i++){
                if(blebsImgWIN[n].at<uchar>(j-rect_tmp.y, i-rect_tmp.x) != 0){
                    overlapBlebs.at<uchar>(j,i) += 200/WINSIZE;
                }
            }
        }
    }
    imshow("overlapBlebs", overlapBlebs);
    threshold(overlapBlebs, overlapBlebs, 200*1/2, 255, THRESH_BINARY);
    //Mat ero;
    //erode(overlapBlebs, ero, getStructuringElement(
    //      MORPH_ELLIPSE, Size( 2*3+1, 2*3+1 ), Point(3, 3)));
    //imshow("erosion", ero);

    int size_thrld = blebSizeRatio*area;
    recursive_connected_components(overlapBlebs, size_thrld, centroid, BIN, blebs);
}


void Controller::run(){
    int delay = (1500/fps);
    int cnt = 0;

    pauseMutex.lock();
    bool paused = pause;
    pauseMutex.unlock();

    while(!paused && cnt < frameCnt){
        if(!inputVideo->read(nextFrame)){
            cout << "Unable to retrieve frame from video stream." << endl;
            pauseMutex.lock();
            pause = true;
            pauseMutex.unlock();
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
            //roiImg2 = img;
            Mat binaryImg;
            contour->binaryImage(*frame, binaryImg);
            roiImg2 = cvMatToQImage(binaryImg);
        }
        else{
            //draw bounding box on ROI and show in original video player

            Mat boxedImg = Mat(frame->rows, frame->cols, CV_8UC3);
            contour->boundingBox(boxedImg);

            Mat contourImg;
            Mat edgeImg;

            //properties getting from cellDetection
            int             area; // area of the cell
            int             perimeter; // perimeter of the cell
            Point2f         centroid; // centroid of the cell
            float           shape; // shape of the cell: standard deviation of distances (contour points 2 centroid)
            Mat             cell_alpha; // cell image without background
            vector<Point>   smooth_contour_curve; // smoothed contour
            Mat             blebsImg;
            Rect            rect;
            vector<Bleb>    blebs; // the deteced blebs
            vector<int>     blebs_bin(BIN, 0); // for each bin, if there is a bleb -> size, if not -> 0
            vector<Point2f> contur; // contour of the cell

            // optflow detection of entire frame
            vector<Point2f> points1, points2;
            bool opt = optflow(*frame, nextFrame, points1, points2);
            if (!opt){
                cout << "optical flow detection failed on frame " << frameIdx << endl;
                continue;
            }

            switch (videoType) {
            case 0:
                contour->singleCellDetection(*frame, hull, contourImg, edgeImg,
                                             area, perimeter, centroid, shape, cell_alpha, smooth_contour_curve,
                                             blebsImg, rect, /*blebs,*/ frameIdx);
                break;
            case 1:
                contour->cellDetection(*frame, hull, contourImg, edgeImg, points1, points2,
                                       area, perimeter, centroid, shape, /*blebs,*/ frameIdx);
                break;
            case 2:
                contour->cellDetection(*frame, hull, contourImg, edgeImg, points1, points2,
                                       area, perimeter, centroid, shape, /*blebs,*/ frameIdx);
                break;
            default:
                contour->cellDetection(*frame, hull, contourImg, edgeImg, points1, points2,
                                       area, perimeter, centroid, shape, /*blebs,*/ frameIdx);
                break;
            }

//            vector<int> compression_params;
//            compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
//            compression_params.push_back(9);
//            QString cellFileName1 = "cell" + QString::number(frameIdx) + ".png";
//            imwrite(cellFileName1.toStdString(), cell_alpha, compression_params);


            floatArray property;
            property.push_back(float(area));
            property.push_back(float(perimeter));
            property.push_back(centroid.x);
            property.push_back(shape);
            property.push_back(centroid.y);//not appliable yet
            property.push_back(0.0);//not appliable yet

            // keep a time(frame) window and filter the detected features
            if(blebsImgWIN.size() < WINSIZE){
                blebsImgWIN.push_back(blebsImg);
                rectWIN.push_back(rect);
            }
            else if(blebsImgWIN.size() == WINSIZE){
                blebsImgWIN.pop_front();
                rectWIN.pop_front();
                blebsImgWIN.push_back(blebsImg);
                rectWIN.push_back(rect);
                findBlebs(area, centroid, BIN, blebs);
            }
            else{
                cout << "error: time window size too large. " << endl;
            }



            //double subImgSize = contourImg.cols*contourImg.rows;
            double area_ratio = micMtr_Pixel*micMtr_Pixel/scale/scale;
            double len_ratio  = micMtr_Pixel/scale/scale;
            //cout << "area_ratio " << area_ratio << endl;
            for(unsigned int n = 0; n < blebs.size(); n++){
                circle(boxedImg, blebs[n].center, 3, Scalar(144, 57, 123, 64), -1);
                blebs_bin[blebs[n].bin] = blebs[n].size /** area_ratio*/;
            }

            csvFile << frameIdx << ","
                    << area * area_ratio << ","
                    << perimeter * len_ratio << ","
                    << centroid.x << "," << centroid.y << ","
                    << shape << ","
                    << blebs.size() << ", ";

//            for(unsigned int b = 0; b < blebs_bin.size(); b++)
//                cout << blebs_bin[b] << " ";
//            cout << endl;

            for(unsigned int b = 0; b < blebs_bin.size(); b++)
                csvFile << blebs_bin[b] << ",";
            csvFile << endl;

            emit detectedProperties(property);
            QVector<QPoint> smoothContour;
            for(unsigned int n = 0; n < smooth_contour_curve.size(); n++)
                smoothContour.push_back(QPoint(smooth_contour_curve[n].x, smooth_contour_curve[n].y));
            emit detectedCellImg(cvMatToQImage(cell_alpha), smoothContour);

            img = cvMatToQImage(boxedImg);
            roiImg1 = cvMatToQImage(contourImg);
            roiImg2 = cvMatToQImage(edgeImg);
        }

        *frame = nextFrame.clone();

        //emit the singnals
        emit processedImage(img, roiImg1, roiImg2);
        this->msleep(delay);

        pauseMutex.lock();
        paused = pause;
        pauseMutex.unlock();
    }

    if(cnt==frameCnt){
        csvFile.close();
        cout << "data file (csv) saved." << endl;
    }
}

void Controller::pauseVideo(){
    //QMutexLocker    locker(pauseMutex);
    pauseMutex.lock();
    pause = true;
    pauseMutex.unlock();
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
