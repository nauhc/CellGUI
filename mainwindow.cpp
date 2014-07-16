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
    connect(myController, SIGNAL(processedImage(QImage, QImage)),
            this, SLOT(updateVideoplayerUI(QImage, QImage)));
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setAdaptThresh(int)));
    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setBlkSize(int)));
    connect(myController, SIGNAL(detectedArea(int)),
            this, SLOT(updateAreaVisUI(int)));

    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    ui->drawROIButton->setEnabled(false);
    ui->adaptThreshSlider->setRange(1, 51);
    ui->blkSizeSlider->setRange(1, 20);
    ui->adaptThreshSlider->setValue(7); // initial value of constValue for adaptiveThreshold
    ui->blkSizeSlider->setValue(8); // initial value of block size for adaptiveThreshold

    drawMode = false;
    encircle = new Encircle(drawMode, this->centralWidget());
    encircle->setGeometry(40, 30, 500, 500);

    areaVis = new AreaVis(this->centralWidget());
    areaVis->setGeometry(560, 610, 1120, 500);
}

MainWindow::~MainWindow(){
    delete areaVis;
    delete encircle;
    delete myController;
    delete ui;
}

void MainWindow::updateVideoplayerUI(QImage img, QImage ROIimg){
    if(!img.isNull()){
        //original video display
        ui->orgVideo->setAlignment(Qt::AlignCenter);
        ui->orgVideo->setPixmap(
                    QPixmap::fromImage(img).scaled(
                    ui->orgVideo->size(),
                    Qt::KeepAspectRatio,
                    Qt::FastTransformation));
        //roi video display
        ui->roiVideo->setAlignment(Qt::AlignCenter);
        ui->roiVideo->setPixmap(
                    QPixmap::fromImage(ROIimg).scaled(
                    ui->roiVideo->size(),
                    Qt::KeepAspectRatio,
                    Qt::FastTransformation));
        ui->horizontalSlider->setValue(myController->getCurrentFrame());
        ui->frameLabel->setText(QString::number(int(myController->getCurrentFrame()))
                                +" / "+ QString::number(myController->getNumberOfFrames()));
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

    areaVis->turnTrackOff();
    areaVis->turnVisOff();

    QPixmap pixmap1(1,1); // Works
    pixmap1 = pixmap1.scaled(ui->roiVideo->width(), ui->roiVideo->height());
    pixmap1.fill(QColor(0, 0, 0));
    ui->roiVideo->setPixmap(pixmap1);
    QPixmap pixmap2(1,1); // Works
    pixmap2 = pixmap2.scaled(ui->orgVideo->width(), ui->orgVideo->height());
    pixmap2.fill(QColor(0, 0, 0));
    ui->orgVideo->setPixmap(pixmap2);

    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setEnabled(false);
    ui->horizontalSlider->setValue(0);

    ui->drawROIButton->setEnabled(false);
}

void MainWindow::on_loadVideoButton_clicked()
{
    cout << "'Load Video' Button clicked." << endl;

    /*if(!myController->videoIsNull()){
        cout << "*******check point*******" << endl;
        delete myController;
        myController = new Controller();
    }*/


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
                ui->playVideoButton->setEnabled(true);
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
                    x = 40; y = 30+(width-height)/2;
                }else{
                    width   = 500*w/h;
                    height  = 500;
                    x = 40+(height-width)/2; y = 30;
                }
                ui->orgVideo->setGeometry(x, y, width, height);
                ui->orgVideo->setAlignment(Qt::AlignCenter);
                //ui->orgVideo->setPixmap(QPixmap::fromImage(myController->getFrame(1)).scaled(
                //                        ui->orgVideo->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
                ui->roiVideo->setGeometry(x, y + 580, width, height);
                ui->roiVideo->setAlignment(Qt::AlignCenter);
            }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }
}

void MainWindow::updateAreaVisUI(int area){
    areaVis->updateArea(area, myController->getCurrentFrame());
}

void MainWindow::on_drawROIButton_clicked(){
    cout << "Encircle Cell Button clicked." << endl;

    // when it is circling mode
    // user can circle the cell of interest
    if(!drawMode){
        drawMode = true;

        ui->playVideoButton->setEnabled(false);
        ui->stopVideoButton->setEnabled(false);
        ui->drawROIButton->setText("Track Cell");

        myController->pauseVideo();

        areaVis->turnVisOn();

        encircle->clearCircle();
        encircle->turnOnEncircleMode();
    }

    // when circling mode is turned off, track starts
    // pass the circled region to controller
    // and clear the drawing
    else{
        drawMode = false;

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
        //myController->playVideo();

    }
}














