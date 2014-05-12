#include "video.h"

int constValue = 7;


Video::Video(): videoSize(Size(0,0)),
                        frameCnt(0),
                        fps(0.0){

}

Video::~Video(){
    inputVideo.release();
}

bool Video::readVideo(const string &filename){

    inputVideo.open(filename);

    if(!inputVideo.isOpened()){
        cout << "Could not open the input video:" << filename << endl;
        return false;
    }
    videoSize = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
                     (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
    frameCnt = inputVideo.get(CV_CAP_PROP_FRAME_COUNT);
    fps = inputVideo.get(CV_CAP_PROP_FPS);
    cout << "FPS: " << fps << ". Number of frames: " << frameCnt << endl;
    cout << "video size: " << videoSize.width << " x " << videoSize.height << endl;
    return true;
}

void Video::displayVideo(){
    namedWindow("Video", CV_WINDOW_AUTOSIZE);
    //int frm = 0;
    while(1){
        //frm++;
        Mat frame;
        bool bSuccess = inputVideo.read(frame);
        if(!bSuccess){
            cout << "Can not read the frame from input video.\n";
            break;
        }
        imshow("Video", frame);
        //string outframe = to_string(frm) + ".tiff";
        //imwrite(outframe, frame);
        if(waitKey(30)==27){
            cout << "ESC key is pressed.\n";
            break;
        }
    }
}

void Video::edgeDetection(){
    int blockSize = 35;
    const string winName = "Cell Transformation";
    namedWindow(winName , CV_WINDOW_FREERATIO );
    createTrackbar("Value", winName, &constValue, 81);

    for (int i = 0; i < frameCnt; i++){
        Mat frame;
        if(inputVideo.read(frame)){
            //Mat image = Mat::zeros(frame.size().height, frame.size().width, CV_8UC1);
            // frame is CV_8UC3 color image
            Rect roi = Rect(videoSize.width/2- 10, videoSize.height/2 + 10, 150, 100);
            Mat image; //grayscale image of each frame
            cvtColor(frame, image, CV_RGB2GRAY); // convert color image to grayscale image
            if(i > 80){
                Mat image_roi = image(roi); //region of interest

                adaptiveThreshold(image_roi, image_roi, 255.0, CV_ADAPTIVE_THRESH_MEAN_C,
                                  CV_THRESH_BINARY_INV, blockSize, constValue);
            }
            imshow(winName, image);
            if(waitKey(30)==27){
                cout << "ESC key is pressed.\n";
                break;
            }
        }
        else{
            cout << "can not read frame " << i << endl;
        }
    }
    destroyAllWindows();
}

