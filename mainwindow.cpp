#include "mainwindow.h"
#include <QWidget>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "qdebug.h"

#define PI 3.14159265


//const QString button_pressed        = "color:rgb(200,200,200); font: bold 16px; border-style:inset; border-width:7px; \
                                         border-color:rgb(0,0,0); border-radius:4px; background-color:rgb(20,20,20)";
//const QString button_released_on    = "color:rgb(255,255,255); font: bold 16px; border-style:outset; border-width:2px; \
                                         border-color:rgb(150,150,150); border-radius:4px; background-color:rgb(38,42,43)";
//const QString button_released_off   = "color:rgb(80,80,80); font: bold 16px; border-style:outset; border-width:2px; \
                                         border-color:rgb(80,80,80); border-radius:4px; background-color:rgb(38,42,43)";

const QString button_pressed        = "color:rgb(81,85,96); font: bold 16px; border-style:inset; border-width:2px; \
                                       border-color:rgb(186,192,206); border-radius:4px; background-color:rgb(163,203,215)";
const QString button_released_on    = "color:rgb(82,89,99); font: bold 16px; border-style:outset; border-width:2px; \
                                       border-color:rgb(217,217,219); border-radius:4px; background-color:rgb(162,191,216)";
const QString button_released_off   = "color:rgb(193,194,199); font: bold 16px; border-style:outset; border-width:2px; \
                                       border-color:rgb(217,217,219); border-radius:4px; background-color:rgb(239,245,248)";
const QString frameLabelStyle       = "color:rgb(82,89,99); font:12px; background-color:rgba(0,0,0,0%) ";
const QString transBkgrd            = "background-color: rgba(0,0,0,0%);";
const QString halfTransBkgrd        = "background-color: rgba(128,128,128,80%);";
const QString forgrdWhite           = "color:white;";
const QString forgrdBlue            = "color:rgb(28, 120, 159);";

const QString forgrdGray            = "color:rgb(82,89,99);";
const QString visStyle              = "color:rgb(239,240,244); border: 2px solid; border-color:rgb(217,217,219)"; //color:rgb(54,58,59)
const QString videoDisplayStyle     = "background-color:rgb(216,222,224)";
const QString forgrdGreen           = "color:rgb(153, 204, 49);"; //(79, 193, 131)
const QString forgrdOrage           = "color:rgb(251, 172, 81);"; //(238, 122, 83)
const QString font20                = "font: 20px";
const QString font16                = "font: 16px";
const QString font16bld             = "font: bold 16px";


template <class T>
inline T sqre(T value){
    return value*value;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    myController(new Controller())
{
    ui->setupUi(this);
    this->setStyleSheet(/*"background-color:rgb(38,42,43)"*/"background-color:rgb(251,251,251)");
    this->setFixedSize(this->width(), this->height());

    // connect displying images
    connect(myController, SIGNAL(load1stImage(QImage)),
            this, SLOT(initialVideoPlayerUI(QImage)));
    connect(myController, SIGNAL(processedImage(QImage, QImage, QImage)),
            this, SLOT(updateVideoplayerUI(QImage, QImage, QImage)));

    //connect slide bar values
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setAdaptThresh(int)));
    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setBlkSize(int)));

    // connect combobox to controller
    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)),
            myController, SLOT(setVideoType(int)));

    // connect checkbox to box_checked event
    qRegisterMetaType<floatArray>("floatArray");
    connect(myController, SIGNAL(detectedProperties(floatArray)),
            this, SLOT(updatePropsVisUI(floatArray)));
    connect(ui->checkBox_area, SIGNAL(stateChanged(int)),
            this, SLOT(box_checked(int)));
    connect(ui->checkBox_perimeter, SIGNAL(stateChanged(int)),
            this, SLOT(box_checked(int)));
    connect(ui->checkBox_centroid, SIGNAL(stateChanged(int)),
            this, SLOT(box_checked(int)));
    connect(ui->checkBox_blebbing, SIGNAL(stateChanged(int)),
            this, SLOT(box_checked(int)));
    connect(ui->checkBox_shape, SIGNAL(stateChanged(int)),
            this, SLOT(box_checked(int)));
    connect(ui->checkBox_speed, SIGNAL(stateChanged(int)),
            this, SLOT(box_checked(int)));

    ui->orgVideo->setStyleSheet(videoDisplayStyle);
    ui->roiVideo1->setStyleSheet(videoDisplayStyle);
    ui->roiVideo2->setStyleSheet(videoDisplayStyle);

    ui->loadVideoButton->setStyleSheet(button_released_on);
    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setStyleSheet(button_released_off);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setStyleSheet(button_released_off);
    ui->stopVideoButton->setEnabled(false);
    ui->drawROIButton->setStyleSheet(button_released_off);
    ui->drawROIButton->setEnabled(false);

    ui->groupBox->setStyleSheet("QGroupBox { color:rgb(82,89,99); \
                                border: 2px solid; border-color:rgb(217,217,219);  border-radius: 5px;\
                                font-size: 16px; font-weight: bold; \
                                margin-top: 7px; margin-bottom: 7px; padding: 0px} \
                                QGroupBox::title {top:-7px;left: 10px; subcontrol-origin: border }");
    const QString checkboxStyle = "color:rgb(82,89,99); font-size: 14px";               
    ui->checkBox_area->setChecked(true);
    ui->checkBox_perimeter->setChecked(true);
    ui->checkBox_area->setStyleSheet(checkboxStyle);
    ui->checkBox_blebbing->setStyleSheet(checkboxStyle);
    ui->checkBox_centroid->setStyleSheet(checkboxStyle);
    ui->checkBox_perimeter->setStyleSheet(checkboxStyle);
    ui->checkBox_shape->setStyleSheet(checkboxStyle);
    ui->checkBox_speed->setStyleSheet(checkboxStyle);

    ui->typeComboBox->setStyleSheet(button_released_on);
    ui->typeComboBox->addItem("Single cell (no overlapping)");
    ui->typeComboBox->addItem("Fix cell window");
    ui->typeComboBox->addItem("Flexible cell window");

    ui->horizontalSlider->setEnabled(false);
    ui->adaptThreshSlider->setRange(1, 51);
    ui->blkSizeSlider->setRange(1, 20);
    ui->adaptThreshSlider->setValue(4); // initial value of constValue for adaptiveThreshold
    ui->blkSizeSlider->setValue(8); // initial value of block size for adaptiveThreshold

    ui->videoDisplayerLabel->setStyleSheet(transBkgrd+forgrdBlue+font20);
    ui->videoDisplayerLabel->setText("Video");

    ui->contourDisplayerLabel->setStyleSheet(transBkgrd+forgrdBlue+font16);
    ui->contourDisplayerLabel->setText("Cell Contour");
    ui->cellDetectionDisplayerLabel->setStyleSheet(transBkgrd+forgrdBlue+font16);
    ui->cellDetectionDisplayerLabel->setText("Cell Detection");

    ui->differenceLabel->setStyleSheet(transBkgrd+forgrdGray+"font:12px");
    ui->differenceLabel->setText("Difference from Neighbors"/*+QString::number(ui->adaptThreshSlider->value())*/);
    ui->blkSizeLabel->setStyleSheet(transBkgrd+forgrdGray+"font:12px");
    ui->blkSizeLabel->setText("Referecing-neighbor Size"/*+QString::number(ui->blkSizeSlider->value())*/);

    ui->frameLabelLeft->setStyleSheet(frameLabelStyle);
    ui->frameLabelLeft->setText("Frame No.");
    ui->frameLabelRight->setStyleSheet(frameLabelStyle);

    // prop1Vis initialize -- areaVis
    QRect areaVisRect = QRect(40, 620, 1150, 280);
    QColor areaVisColor = QColor(153, 204, 49); // green color
    ui->prop1Vis->setGeometry(areaVisRect);
    ui->prop1Vis->setStyleSheet(visStyle);
    prop1Vis = new DataVis(this->centralWidget(), areaVisColor/*, 500, 8000*/);
    prop1Vis->setGeometry(areaVisRect);
    QRect areaRectLabel = QRect(areaVisRect.x(), areaVisRect.y()-25, areaVisRect.width(), 20);
    ui->prop1VisLabel->setGeometry(areaRectLabel);
    ui->prop1VisLabel->setStyleSheet(transBkgrd+"color:rgb("+
                                    QString::number(areaVisColor.red())+","+
                                    QString::number(areaVisColor.green())+","+
                                    QString::number(areaVisColor.blue())+");"+font20);
    ui->prop1VisLabel->setText("  AREA (pixels)");

    // prop2Vis initialize -- prmtVis
    QRect prmtVisRect = QRect(areaVisRect.x(), areaVisRect.y()+areaVisRect.height()+35, areaVisRect.width(), areaVisRect.height());
    QColor prmtVisColor = QColor(251, 172, 81); // orange color
    ui->prop2Vis->setGeometry(prmtVisRect);
    ui->prop2Vis->setStyleSheet(visStyle);
    prop2Vis = new DataVis(this->centralWidget(), prmtVisColor/*, 0, 1350*/);
    prop2Vis->setGeometry(prmtVisRect);
    QRect prmtRectLabel = QRect(prmtVisRect.x(), prmtVisRect.y()-25, prmtVisRect.width(), 20);
    ui->prop2VisLabel->setGeometry(prmtRectLabel);
    ui->prop2VisLabel->setStyleSheet(transBkgrd+"color:rgb("+
                                        QString::number(prmtVisColor.red())+","+
                                        QString::number(prmtVisColor.green())+","+
                                        QString::number(prmtVisColor.blue())+");"+font20);
    ui->prop2VisLabel->setText("  PERIMETER (pixels)");

    encircler = new Encircle(this->centralWidget());
    //encircle->setGeometry(40, 30, 500, 500);
    encircled = false;
}

MainWindow::~MainWindow(){
    delete prop2Vis;
    delete prop1Vis;
    delete encircler;
    delete myController;
    delete ui;
}

void calculateSizes(int &videoWidth, int &videoHeight,
                    int &x, int &y, int &width, int &height,
                    int &x1,int &y1,int &width1,int &height1,
                    int &x2,int &y2,int &width2,int &height2,
                    double &scale){
    const int x0 = 40, y0 = 30;   // offset of orgVideo display window (x0,y0)
    const int w0 = 512, h0 = 512; // width and height of orgVideo display window
    x1 = 590, x2 = 590;           // offset of roiVideo1 & roiVideo2 display window (x1)

    if(videoWidth >= videoHeight){
        scale = double(w0)/double(videoWidth);
        width   = w0;
        height  = w0*videoHeight/videoWidth; //(width > height)
        x = x0;
        y = y0 + (width-height)/2;

        y1 = y0 + width/4 - height/4;
        width1 = width/2-10;
        height1 = height/2-10;

        y2 = y0+ h0/2 + width/4 - height/4;
        width2 = width1;
        height2 = height1;
    }
    else{
        scale = double(h0)/double(videoHeight);
        height  = h0;
        width   = h0*videoWidth/videoHeight; //(width < height)
        x = x0 + (height-width)/2;
        y = y0;

        y1 = y0 + height/4 - width/4; // 30 - (height/2-width/2)/2
        width1 = width/2-10;
        height1 = height/2-10;

        y2 = y0 + h0/2 + height/4 - width/4;
        width2 = width1;
        height2 = height1;
    }
}

void MainWindow::setCanvas(){
    //set video player label size and postion
    //according to the size of the selected video

    int width, height; // size of original video
    myController->getVideoSize(width, height);

    int x, y, w, h, x1, y1, w1, h1, x2, y2, w2, h2;
    double scale;
    calculateSizes(width, height, x, y, w, h,
                   x1, y1, w1, h1, x2, y2, w2, h2, scale);
    cout << "video pos:  x " << x << " y " << y << " width " << w << " height " << h << endl;
    cout << "scale " << scale << endl;

    ui->orgVideo->setGeometry(x, y, w, h);
    encircler->setGeometry(x, y, w, h);
    myController->setScale(scale);

    ui->roiVideo1->setGeometry(x1, y1, w1, h1);
    ui->roiVideo1->setAlignment(Qt::AlignCenter);

    //int video_y_max = ui->orgVideo->y()+ui->orgVideo->height();
    ui->roiVideo2->setGeometry(x2, y2, w2, h2);
    ui->roiVideo2->setAlignment(Qt::AlignCenter);
}

void MainWindow::initialVideoPlayerUI(QImage img)
{
    if(!img.isNull()){
        //original video display
        setCanvas();
        ui->orgVideo->setAlignment(Qt::AlignCenter);
        ui->orgVideo->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        ui->orgVideo->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        //roi video display 1
        ui->roiVideo1->setAlignment(Qt::AlignCenter);
        ui->roiVideo1->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        ui->roiVideo1->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        ui->roiVideo2->setAlignment(Qt::AlignCenter);
        ui->roiVideo2->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        ui->roiVideo2->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
    }
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
void MainWindow::on_stopVideoButton_clicked(){
    cout << "'Stop Video' Button clicked." << endl;

    if(!myController->videoIsPaused())
        myController->pauseVideo();
    myController->releaseVideo();
    ui->frameLabelRight->clear();

    encircled = false;

    prop2Vis->releaseDataVis();
    prop1Vis->releaseDataVis();

    ui->typeComboBox->setEnabled(true);
    ui->typeComboBox->setStyleSheet(button_released_on);

    QPixmap pixmap(1,1); // Works
    pixmap = pixmap.scaled(ui->orgVideo->width(), ui->orgVideo->height());
    pixmap.fill(QColor(216,222,224));
    ui->orgVideo->setPixmap(pixmap);
    QPixmap pixmap1(1,1); // Works
    pixmap1 = pixmap1.scaled(ui->roiVideo1->width(), ui->roiVideo1->height());
    pixmap1.fill(QColor(216,222,224));
    ui->roiVideo1->setPixmap(pixmap1);
    QPixmap pixmap2(1,1); // Works
    pixmap2 = pixmap2.scaled(ui->roiVideo2->width(), ui->roiVideo2->height());
    pixmap2.fill(QColor(216,222,224));
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
                                               "./video", /*QDir::homePath()+"/Desktop/",*/
                                               tr("Video Files (*.mov *mp4 *wmv *mpeg)"));
    delete dialog;

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

        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }
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

inline int propIndex(QString str){
    if(str == "area")
        return 0;
    else if (str == "perimeter")
        return 1;
    else if (str == "centroid")
        return 2;
    else if (str == "shape")
        return 3;
    else if (str == "blebbing")
        return 4;
    else if (str == "speed")
        return 5;
    else
        return 0;
}

void MainWindow::updatePropsVisUI(floatArray property){ //int prop1,prop2, prop3, prop4, prop5

    if(checkedBoxes.size()==1){
        prop1Vis->updateData(property[propIndex(checkedBoxes[0])], myController->getCurrentFrame());
        prop2Vis->turnVisOff();
    }else{
        prop1Vis->updateData(property[propIndex(checkedBoxes[0])], myController->getCurrentFrame());
        prop2Vis->updateData(property[propIndex(checkedBoxes[1])], myController->getCurrentFrame());
    }
}



void MainWindow::box_checked(int state) {

    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;

    if (state == Qt::Checked) {
        if (checkedBoxes.size() == 2) {
            checkBox->setCheckState(Qt::Unchecked);
            return;
        } else if (!checkedBoxes.contains(checkBox->objectName().remove(0, 9))) {
            checkedBoxes.push_back(checkBox->objectName().remove(0, 9));
        }
    } else if (state == Qt::Unchecked) {
        if (checkedBoxes.size() == 0) {
            return; // should not happen
        } else if (checkedBoxes.contains(checkBox->objectName().remove(0, 9))) {
            checkedBoxes.remove(checkedBoxes.indexOf(checkBox->objectName().remove(0, 9)));
        }
    }

    /*
    for(int n = 0; n < checkedBoxes.size(); n++)
        std::cout << checkedBoxes[n].toStdString() << "\n";
    std::cout << "---" << std::endl;*/

    if(checkedBoxes.size()==0){
        QMessageBox msgBox;
        msgBox.setText("Warning: no property is selected!");
        msgBox.exec();
    }
    else if(checkedBoxes.size()==1){
        ui->prop1VisLabel->setText("  "+checkedBoxes[0].toUpper()+" (pixels)");
        ui->prop2VisLabel->setText("  ");
    }else{
        ui->prop1VisLabel->setText("  "+checkedBoxes[0].toUpper()+" (pixels)");
        ui->prop2VisLabel->setText("  "+checkedBoxes[1].toUpper()+" (pixels)");
    }

}

void MainWindow::on_drawROIButton_clicked(){

    encircler->setGeometry(ui->orgVideo->x(), ui->orgVideo->y(),
                           ui->orgVideo->width(), ui->orgVideo->height());
    cout << "video pos:  x " << ui->orgVideo->x()
         << " y " << ui->orgVideo->y()
         << " width " << ui->orgVideo->width()
         << " height " << ui->orgVideo->height() << endl;

    cout << "Encircle Cell Button clicked." << endl;


    // when it is circling mode
    // user can circle the cell of interest
    if(!encircler->isEncircled()){
        this->setCursor(Qt::CrossCursor);
        ui->playVideoButton->setEnabled(false);
        ui->playVideoButton->setStyleSheet(button_released_off);
        ui->stopVideoButton->setEnabled(false);
        ui->stopVideoButton->setStyleSheet(button_released_off);
        ui->drawROIButton->setText("Track Cell");

        myController->pauseVideo();

        encircler->clearCircle();
        encircler->turnOnEncircleMode();
        encircled = true;

        prop1Vis->turnVisOn();
        prop2Vis->turnVisOn();
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
        prop1Vis->turnTrackOn(myController->getNumberOfFrames(),
                             myController->getCurrentFrame());
        prop2Vis->turnTrackOn(myController->getNumberOfFrames(),
                             myController->getCurrentFrame());

        ui->typeComboBox->setEnabled(false);
        ui->typeComboBox->setStyleSheet(button_released_off);

        encircler->turnOffEncircleMode();
        //delete encircle;
        QVector<QPoint> circle;
        encircler->getRegion(circle);
        int circleSize = circle.size();
        if(circleSize < 5){
            cout << "circle not found." << endl;
        }
        else{
            double a = sqre(circleSize/2)/PI;
            double p = /*PI**/circleSize;
            //cout << p << " " << a << endl;
            int area_min = ((int)a/3/100-1)*100;
            area_min = area_min > 0 ? area_min : 0;
            int area_max = ((int)a*10/100+1)*100;
            int prmt_min = ((int)p/5/100-1)*100;
            prmt_min = prmt_min > 0 ? prmt_min : 0;
            int prmt_max = ((int)p*3/100+1)*100;
            prop1Vis->setMinMax(area_min, area_max); //(a/3, a*5)
            prop2Vis->setMinMax(prmt_min, prmt_max); //(p/5, p*3)
            cout << "area min " << area_min << " max " << area_max << endl;
            cout << "prmt min " << prmt_min << " max " << prmt_max << endl;
            myController->setCircle(circle);
        }
    }
}
