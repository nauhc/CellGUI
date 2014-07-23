#include "mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    myController(new Controller())
{
    ui->setupUi(this);
//    this->setStyleSheet("background-color:rgb(38,42,43)");
    connect(myController, SIGNAL(processedImage(QImage, QImage, QImage)),
            this, SLOT(updateVideoplayerUI(QImage, QImage, QImage)));
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setAdaptThresh(int)));
    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setBlkSize(int)));
    connect(myController, SIGNAL(detectedArea(int, int)),
            this, SLOT(updateDataVisUI(int, int)));

//    const QString style = "border-style:outset; border-width:5px;border-color:rgb(28,120,159); border-radius: 8px;";
//    ui->orgVideo->setStyleSheet(style);
//    ui->roiVideo1->setStyleSheet(style);
//    ui->roiVideo2->setStyleSheet(style);
//    ui->loadVideoButton->setStyleSheet(style);
//    ui->playVideoButton->setStyleSheet(style);
//    ui->stopVideoButton->setStyleSheet(style);
//    ui->drawROIButton->setStyleSheet(style);
    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setEnabled(false);

    ui->horizontalSlider->setEnabled(false);
    ui->drawROIButton->setEnabled(false);
    ui->adaptThreshSlider->setRange(1, 51);
    ui->blkSizeSlider->setRange(1, 20);
    ui->adaptThreshSlider->setValue(7); // initial value of constValue for adaptiveThreshold
    ui->blkSizeSlider->setValue(8); // initial value of block size for adaptiveThreshold

//    ui->videoDisplayerLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->videoDisplayerLabel->setText("<span style='color:#FFFFFF;'>Video</span>");
//    ui->videoDisplayerLabel->setFont(QFont("Helvetica", 20));
//    ui->contourDisplayerLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->contourDisplayerLabel->setText("<span style='color:#FFFFFF;'>Cell Contour</span>");
//    ui->contourDisplayerLabel->setFont(QFont("Helvetica", 14));
//    ui->cellDetectionDisplayerLabel->setAttribute(Qt::WA_TranslucentBackground);
    ui->cellDetectionDisplayerLabel->setText("<span style='color:#FFFFFF;'>Cell Detection</span>");
//    ui->cellDetectionDisplayerLabel->setFont(QFont("Helvetica", 14));


    encircle = new Encircle(this->centralWidget());
    encircle->setGeometry(40, 30, 500, 500);
    encircled = false;

    areaVis = new DataVis(this->centralWidget());
    areaVis->setGeometry(40, 610, 1170, 500);
}

MainWindow::~MainWindow(){
    delete areaVis;
    delete encircle;
    delete myController;
    delete ui;
}

void MainWindow::updateVideoplayerUI(QImage img, QImage ROIimg1, QImage ROIimg2){
    if(!img.isNull()){
        //original video display
        ui->orgVideo->setAlignment(Qt::AlignCenter);
        ui->orgVideo->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        ui->orgVideo->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        //roi video display 1
        ui->roiVideo1->setAlignment(Qt::AlignCenter);
        ui->roiVideo1->setPixmap(
                    QPixmap::fromImage(ROIimg1).scaled(
                        ui->roiVideo1->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        ui->roiVideo2->setAlignment(Qt::AlignCenter);
        ui->roiVideo2->setPixmap(
                    QPixmap::fromImage(ROIimg2).scaled(
                        ui->roiVideo2->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));

        int currFrm     = myController->getCurrentFrame();
        int totalFrm    = myController->getNumberOfFrames();
        ui->horizontalSlider->setValue(currFrm);
        ui->frameLabel->setText(QString::number(currFrm) +" / "+ QString::number(totalFrm));
        if(currFrm == totalFrm){
            ui->playVideoButton->setEnabled(false);
            ui->stopVideoButton->setEnabled(true);
        }
    }

}

// paly or pause video
void MainWindow::on_playVideoButton_clicked()
{
    cout << "'Play/Pause Video' Button clicked." << endl;
    if(myController->videoIsPaused()){
        myController->playVideo();
        ui->playVideoButton->setText("Pause");
        ui->stopVideoButton->setEnabled(false);
        ui->drawROIButton->setEnabled(false);
    }else{
        myController->pauseVideo();
        ui->playVideoButton->setText("Play");
        ui->stopVideoButton->setEnabled(true);
        if(!encircled)
            ui->drawROIButton->setEnabled(true);
    }
}

//stop and release video
void MainWindow::on_stopVideoButton_clicked()
{
    cout << "'Stop Video' Button clicked." << endl;

    if(!myController->videoIsPaused())
        myController->pauseVideo();
    myController->releaseVideo();
    ui->frameLabel->clear();

    encircled = false;

    areaVis->releaseAreaVis();

    QPixmap pixmap(1,1); // Works
    pixmap = pixmap.scaled(ui->orgVideo->width(), ui->orgVideo->height());
    pixmap.fill(QColor(0, 0, 0));
    ui->orgVideo->setPixmap(pixmap);
    QPixmap pixmap1(1,1); // Works
    pixmap1 = pixmap1.scaled(ui->roiVideo1->width(), ui->roiVideo1->height());
    pixmap1.fill(QColor(0, 0, 0));
    ui->roiVideo1->setPixmap(pixmap1);
    QPixmap pixmap2(1,1); // Works
    pixmap2 = pixmap2.scaled(ui->roiVideo2->width(), ui->roiVideo2->height());
    pixmap2.fill(QColor(0, 0, 0));
    ui->roiVideo2->setPixmap(pixmap2);

    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setText("play");
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setEnabled(false);
    ui->horizontalSlider->setValue(0);

    ui->drawROIButton->setEnabled(false);
}

void MainWindow::on_loadVideoButton_clicked()
{
    cout << "'Load Video' Button clicked." << endl;

    /*
     if(!myController->videoIsNull()){
        cout << "*******check point*******" << endl;
        delete myController;
        myController = new Controller();
    }*/

//    QFileDialog dialog = new QFileDialog();
//    dialog.setNameFilter("All C++ files (*.avi *.mov *.mpg *.mp4)");

//    QString filename = QFileDialog::getOpenFileName(this,
//                                                    tr("Open Video"), "../../../video/",
//                                                    tr("Video Files (*.avi *.mov *.mpg *.mp4"));

    QString filepath    = "/Users/chuanwang/Sourcecode/CellGUI/video/";
    QString name        = "test.mov";
    QString filename    = filepath + name;

    if (!filename.isEmpty()){
        if (!myController->loadVideo(filename.toStdString())){
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        else{
            ui->loadVideoButton->setEnabled(false);
            ui->playVideoButton->setEnabled(true); //
            ui->drawROIButton->setEnabled(true);
            ui->horizontalSlider->setMaximum(myController->getNumberOfFrames());
            ui->frameLabel->setText("0 / " + QString::number(myController->getNumberOfFrames()));

            //set video player label size and postion
            //according to the size of the selected video
            int w, h;
            myController->getVideoSize(w, h);
            int x, y, width, height;
            if(w>h){
                width   = 500;
                height  = 500*h/w;
                x = 40;
                y = 30+(width-height)/2;
            }else{
                width   = 500*w/h;
                height  = 500;
                x = 40+(height-width)/2;
                y = 30;
            }
            ui->orgVideo->setGeometry(x, y, width, height);
            ui->orgVideo->setAlignment(Qt::AlignCenter);
            //ui->orgVideo->setPixmap(QPixmap::fromImage(myController->getFrame(1)).scaled(
            //                        ui->orgVideo->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
            ui->roiVideo1->setGeometry(650, 30, width/2-10, height/2-10);
            ui->roiVideo1->setAlignment(Qt::AlignCenter);
            ui->roiVideo2->setGeometry(650, 290, width/2-10, height/2-10);
            ui->roiVideo2->setAlignment(Qt::AlignCenter);

        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }
}

void MainWindow::updateDataVisUI(int area, int perimeter){
    areaVis->updateData(area, perimeter, myController->getCurrentFrame());
}

void MainWindow::on_drawROIButton_clicked(){
    cout << "Encircle Cell Button clicked." << endl;

    // when it is circling mode
    // user can circle the cell of interest
    if(!encircle->isEncircled()){

        ui->playVideoButton->setEnabled(false);
        ui->stopVideoButton->setEnabled(false);
        ui->drawROIButton->setText("Track Cell");

        myController->pauseVideo();

        encircle->clearCircle();
        encircle->turnOnEncircleMode();
        encircled = true;

        areaVis->turnVisOn();
    }

    // when circling mode is turned off, track starts
    // pass the circled region to controller
    // and clear the drawing
    else{
        //ui->stopVideoButton->setEnabled(true);
        ui->playVideoButton->setEnabled(true);
        on_playVideoButton_clicked();
        ui->drawROIButton->setText("Encircle Cell");

        areaVis->turnTrackOn(myController->getNumberOfFrames(),
                             myController->getCurrentFrame());

        encircle->turnOffEncircleMode();
        //delete encircle;
        QVector<QPoint> circle;
        encircle->getRegion(circle);

        myController->setCircle(circle);
    }
}














