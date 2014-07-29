#include "mainwindow.h"
#include <QWidget>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "qdebug.h"

const QString button_pressed        = "color:rgb(200,200,200); font: bold 16px; border-style:inset; border-width:7px; border-color:rgb(0,0,0); border-radius:4px; background-color:rgb(20,20,20)";
const QString button_released_on    = "color:rgb(255,255,255); font: bold 16px; border-style:outset; border-width:2px; border-color:rgb(150,150,150); border-radius:4px; background-color:rgb(38,42,43)";
const QString button_released_off   = "color:rgb(80,80,80); font: bold 16px; border-style:outset; border-width:2px; border-color:rgb(80,80,80); border-radius:4px; background-color:rgb(38,42,43)";
const QString frameLabelStyle       = "color:white; font:12px; background-color:rgba(0,0,0,0%) ";
const QString transBkgrd            = "background-color: rgba(0,0,0,0%);";
const QString halfTransBkgrd        = "background-color: rgba(128,128,128,80%);";
const QString forgrdWhite           = "color:white;";
const QString forgrdGreen           = "color:rgb(153, 204, 49);";
const QString forgrdOrage           = "color:rgb(251, 172, 81);";
const QString font20                = "font: 20px";
const QString font16                = "font: 16px";
const QString font16bld             = "font: bold 16px";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    myController(new Controller())
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:rgb(38,42,43)");
    this->setFixedSize(this->width(), this->height());
    connect(myController, SIGNAL(processedImage(QImage, QImage, QImage)),
            this, SLOT(updateVideoplayerUI(QImage, QImage, QImage)));
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setAdaptThresh(int)));
    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setBlkSize(int)));
    connect(myController, SIGNAL(detectedArea(int, int)),
            this, SLOT(updateDataVisUI(int, int)));

    //const QString style = "border-style:outset; border-width:2px; border-color:rgb(128,128,128); border-radius: 4px;"; //border-color:rgb(28,120,159);
    //ui->orgVideo->setStyleSheet(style);
    //ui->roiVideo1->setStyleSheet(style);
    //ui->roiVideo2->setStyleSheet(style);
    ui->loadVideoButton->setStyleSheet(button_released_on);
    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setStyleSheet(button_released_off);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setStyleSheet(button_released_off);
    ui->stopVideoButton->setEnabled(false);
    ui->drawROIButton->setStyleSheet(button_released_off);
    ui->drawROIButton->setEnabled(false);

    ui->horizontalSlider->setEnabled(false);
    ui->adaptThreshSlider->setRange(1, 51);
    ui->blkSizeSlider->setRange(1, 20);
    ui->adaptThreshSlider->setValue(4); // initial value of constValue for adaptiveThreshold
    ui->blkSizeSlider->setValue(8); // initial value of block size for adaptiveThreshold

    ui->videoDisplayerLabel->setStyleSheet(transBkgrd+forgrdWhite+font20);
    ui->videoDisplayerLabel->setText("Video");

    ui->contourDisplayerLabel->setStyleSheet(transBkgrd+forgrdWhite+font16);
    ui->contourDisplayerLabel->setText("Cell Contour");
    ui->cellDetectionDisplayerLabel->setStyleSheet(transBkgrd+forgrdWhite+font16);
    ui->cellDetectionDisplayerLabel->setText("Cell Detection");

    ui->differenceLabel->setStyleSheet(transBkgrd+forgrdWhite+"font:12px");
    ui->differenceLabel->setText("Difference from Neighbors"/*+QString::number(ui->adaptThreshSlider->value())*/);
    ui->blkSizeLabel->setStyleSheet(transBkgrd+forgrdWhite+"font:12px");
    ui->blkSizeLabel->setText("Referecing-neighbor Size"/*+QString::number(ui->blkSizeSlider->value())*/);

    ui->frameLabelLeft->setStyleSheet(frameLabelStyle);
    ui->frameLabelLeft->setText("Frame No.");
    ui->frameLabelRight->setStyleSheet(frameLabelStyle);

    // areaVis
    QRect areaVisRect = QRect(40, 620, 1170, 280);
    QColor areaVisColor = QColor(153, 204, 49); // green color
    ui->areaVis->setGeometry(areaVisRect);
    ui->areaVis->setStyleSheet("background-color: rgb(54,58,59)");
    areaVis = new DataVis(this->centralWidget(), areaVisColor, 500, 8000);
    areaVis->setGeometry(areaVisRect);
    QRect areaRectLabel = QRect(areaVisRect.x(), areaVisRect.y()-25, areaVisRect.width(), 20);
    ui->areaVisLabel->setGeometry(areaRectLabel);
    ui->areaVisLabel->setStyleSheet(transBkgrd+"color:rgb("+
                                    QString::number(areaVisColor.red())+","+
                                    QString::number(areaVisColor.green())+","+
                                    QString::number(areaVisColor.blue())+");"+font20);
    ui->areaVisLabel->setText("  Area (pixels)");

    // prmtVis
    QRect prmtVisRect = QRect(areaVisRect.x(), areaVisRect.y()+areaVisRect.height()+35, areaVisRect.width(), areaVisRect.height());
    QColor prmtVisColor = QColor(251, 172, 81); // orange color
    ui->blebbingVis->setGeometry(prmtVisRect);
    ui->blebbingVis->setStyleSheet("background-color: rgb(54,58,59)");
    prmtVis = new DataVis(this->centralWidget(), prmtVisColor, 0, 1350);
    prmtVis->setGeometry(prmtVisRect);
    QRect prmtRectLabel = QRect(prmtVisRect.x(), prmtVisRect.y()-25, prmtVisRect.width(), 20);
    ui->blebbingVisLabel->setGeometry(prmtRectLabel);
    ui->blebbingVisLabel->setStyleSheet(transBkgrd+"color:rgb("+
                                        QString::number(prmtVisColor.red())+","+
                                        QString::number(prmtVisColor.green())+","+
                                        QString::number(prmtVisColor.blue())+");"+font20);
    ui->blebbingVisLabel->setText("  Perimeter (pixels)");

    encircle = new Encircle(this->centralWidget());
    encircle->setGeometry(40, 30, 500, 500);
    encircled = false;

//    //Time/Frame Axis (h)
//    ui->frameAxis->setGeometry(areaVisRect.x(), areaVisRect.y(), areaVisRect.width(), 2*areaVisRect.height()+20+60);
//    ui->frameAxis->setStyleSheet(transBkgrd);
//    QPixmap pixmap(1,1); // Works
//    pixmap = pixmap.scaled(ui->frameAxis->width(), ui->frameAxis->height());
//    pixmap.fill(Qt::transparent);
//    QPainter painter(&pixmap);
//    QPen myPen(QColor(128, 128, 128, 128));
//    myPen.setWidth(3);
//    painter.setPen(myPen);
//    painter.drawLine(0, pixmap.height()-18, pixmap.width(), pixmap.height()-18);
//    QRectF rectX = QRectF(QPointF(pixmap.width()-250, pixmap.height()-40),
//                          QPointF(pixmap.width()-10, pixmap.height()-20));
//    QString textX = "Frame";
//    painter.setFont(QFont("Arial", 18));
//    painter.drawText(rectX, Qt::AlignRight, textX);
//    ui->frameAxis->setPixmap(pixmap);

//    ui->frameAxisSlider->setGeometry(prmtVisRect.x(), prmtVisRect.y()+prmtVisRect.height()+10,
//                                     prmtVisRect.width(), 30);
//    ui->frameAxisSlider->setStyleSheet(transBkgrd);
//    QString str = "\
//            QSlider::groove:horizontal {\
//            color: rgba(128,128,128,128)\
//            border: 0px solid #999999;\
//            height: 2px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */\
//            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\
//            margin: 2px 0;\
//            }\
//            QSlider::handle:horizontal {\
//            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);\
//            border: 1px solid #5c5c5c;\
//            width: 2px;\
//            height: 4px;\
//            margin: -4px 0; /* handle is placed by default on the contents rect of the groove. Expand outside the groove */\
//            border-radius: 3px;\
//            }\
//            ";
//    ui->frameAxisSlider->setStyleSheet(str);
}

MainWindow::~MainWindow(){
    delete prmtVis;
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
        ui->frameLabelRight->setText(QString::number(currFrm) +" / "+ QString::number(totalFrm));
        ui->frameLabelRight->setStyleSheet(frameLabelStyle);
        if(currFrm == totalFrm){
            ui->playVideoButton->setEnabled(false);
            ui->playVideoButton->setStyleSheet(button_released_off);
            ui->stopVideoButton->setEnabled(true);
            ui->stopVideoButton->setStyleSheet(button_released_on);
            ui->drawROIButton->setEnabled(false);
            ui->drawROIButton->setStyleSheet(button_released_off);
        }
    }

}

void MainWindow::on_playVideoButton_pressed(){
    ui->playVideoButton->setStyleSheet(button_pressed);
}
void MainWindow::on_playVideoButton_released(){
    if(ui->playVideoButton->isEnabled())
        ui->playVideoButton->setStyleSheet(button_released_on);
    else
        ui->playVideoButton->setStyleSheet(button_released_off);
}
// play or pause video
void MainWindow::on_playVideoButton_clicked()
{

    cout << "'Play/Pause Video' Button clicked." << endl;
    if(myController->videoIsPaused()){
        myController->playVideo();
        ui->playVideoButton->setText("Pause");
        ui->playVideoButton->setStyleSheet(button_released_on);
        ui->stopVideoButton->setEnabled(false);
        ui->stopVideoButton->setStyleSheet(button_released_off);
        ui->drawROIButton->setEnabled(false);
        ui->drawROIButton->setStyleSheet(button_released_off);
    }else{
        myController->pauseVideo();
        ui->playVideoButton->setText("Play");
        ui->playVideoButton->setStyleSheet(button_released_on);
        ui->stopVideoButton->setEnabled(true);
        ui->stopVideoButton->setStyleSheet(button_released_on);
        if(!encircled){
            ui->drawROIButton->setEnabled(true);
            ui->drawROIButton->setStyleSheet(button_released_on);
        }
    }
}

void MainWindow::on_stopVideoButton_pressed(){
    ui->stopVideoButton->setStyleSheet(button_pressed);
}
void MainWindow::on_stopVideoButton_released(){
    if(ui->stopVideoButton->isEnabled())
        ui->stopVideoButton->setStyleSheet(button_released_on);
    else
        ui->stopVideoButton->setStyleSheet(button_released_off);
}
//stop and release video
void MainWindow::on_stopVideoButton_clicked()
{
    cout << "'Stop Video' Button clicked." << endl;

    if(!myController->videoIsPaused())
        myController->pauseVideo();
    myController->releaseVideo();
    ui->frameLabelRight->clear();

    encircled = false;

    prmtVis->releaseDataVis();
    areaVis->releaseDataVis();

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
    ui->loadVideoButton->setStyleSheet(button_released_on);
    ui->playVideoButton->setText("play");
    ui->playVideoButton->setEnabled(false);
    ui->playVideoButton->setStyleSheet(button_released_off);
    ui->stopVideoButton->setEnabled(false);
    ui->stopVideoButton->setStyleSheet(button_released_off);
    ui->horizontalSlider->setValue(0);
    ui->drawROIButton->setEnabled(false);
    ui->drawROIButton->setStyleSheet(button_released_off);
    ui->contourDisplayerLabel->setStyleSheet(transBkgrd+forgrdWhite+font16bld);
    ui->cellDetectionDisplayerLabel->setStyleSheet(transBkgrd+forgrdWhite+font16bld);

}

void MainWindow::on_loadVideoButton_pressed(){
    ui->loadVideoButton->setStyleSheet(button_pressed);
}
void MainWindow::on_loadVideoButton_released(){
    if(ui->loadVideoButton->isEnabled())
        ui->loadVideoButton->setStyleSheet(button_released_on);
    else
        ui->loadVideoButton->setStyleSheet(button_released_off);
}

void MainWindow::on_loadVideoButton_clicked()
{
    cout << "'Load Video' Button clicked." << endl;


    QFileDialog *dialog = new QFileDialog();
    QString filename = dialog->getOpenFileName(this,
                                               tr("Open Video"),
                                               "./video",
//                                               QDir::homePath()+"/Desktop/",
                                               tr("Video Files (*.mov)"));
    delete dialog;

    //    QString filepath    = "/Users/chuanwang/Sourcecode/CellGUI/video/";
    //    QString name        = "test.mov";
    //    QString filename    = filepath + name;

    if (!filename.isEmpty()){
        if (!myController->loadVideo(filename.toStdString())){
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        else{
            ui->loadVideoButton->setEnabled(false);
            ui->loadVideoButton->setStyleSheet(button_released_off);
            ui->playVideoButton->setEnabled(true);
            ui->playVideoButton->setStyleSheet(button_released_on);
            ui->drawROIButton->setEnabled(false);
            ui->drawROIButton->setStyleSheet(button_released_off);
            ui->horizontalSlider->setMaximum(myController->getNumberOfFrames());
            ui->frameLabelRight->setText(" 0 / " + QString::number(myController->getNumberOfFrames()));

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
    areaVis->updateData(area, myController->getCurrentFrame());
    prmtVis->updateData(perimeter, myController->getCurrentFrame());
}

void MainWindow::on_drawROIButton_pressed(){
    ui->drawROIButton->setStyleSheet(button_pressed);
}
void MainWindow::on_drawROIButton_released(){
    if(ui->drawROIButton->isEnabled())
        ui->drawROIButton->setStyleSheet(button_released_on);
    else
        ui->drawROIButton->setStyleSheet(button_released_off);
}
void MainWindow::on_drawROIButton_clicked(){
    cout << "Encircle Cell Button clicked." << endl;

    // when it is circling mode
    // user can circle the cell of interest
    if(!encircle->isEncircled()){
        this->setCursor(Qt::CrossCursor);
        ui->playVideoButton->setEnabled(false);
        ui->playVideoButton->setStyleSheet(button_released_off);
        ui->stopVideoButton->setEnabled(false);
        ui->stopVideoButton->setStyleSheet(button_released_off);
        ui->drawROIButton->setText("Track Cell");

        myController->pauseVideo();

        encircle->clearCircle();
        encircle->turnOnEncircleMode();
        encircled = true;

        areaVis->turnVisOn();
        prmtVis->turnVisOn();
    }

    // when circling mode is turned off, track starts
    // pass the circled region to controller
    // and clear the drawing
    else{
        //ui->stopVideoButton->setEnabled(true);
        ui->playVideoButton->setEnabled(true);
        ui->playVideoButton->setStyleSheet(button_released_on);
        on_playVideoButton_clicked();
        ui->drawROIButton->setText("Encircle Cell");
        ui->drawROIButton->setStyleSheet(button_released_off);
        ui->contourDisplayerLabel->setStyleSheet(halfTransBkgrd+forgrdOrage+"border-radius:4px;"+font16bld);
        ui->cellDetectionDisplayerLabel->setStyleSheet(halfTransBkgrd+forgrdGreen+"border-radius:4px;"+font16bld);
        this->setCursor(Qt::ArrowCursor);
        areaVis->turnTrackOn(myController->getNumberOfFrames(),
                             myController->getCurrentFrame());
        prmtVis->turnTrackOn(myController->getNumberOfFrames(),
                             myController->getCurrentFrame());

        encircle->turnOffEncircleMode();
        //delete encircle;
        QVector<QPoint> circle;
        encircle->getRegion(circle);

        myController->setCircle(circle);
    }
}






