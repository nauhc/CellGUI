#include "mainwindow.h"
#include <QWidget>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "multiview.h"
#include "singleview.h"

#include "style.h"

template <class T>
inline T sqre(T value){
    return value*value;
}

void MainWindow::updateBlebSizeSliderText(int value)
{
    ui->blebSizeRatioNum->setText("1/"+QString::number((11-value)*50));
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    myController(new Controller()),
    dataFilename(new QString(""))
{
    fileMode = false;

    ui->setupUi(this);
    this->setStyleSheet(/*"background-color:rgb(38,42,43)"*/"background-color:rgb(251,251,251)");
    this->setFixedSize(this->width(), this->height());
    this->setWindowTitle(" Dancing Cell Visualization ");

    QWidget *tempWidget = new QWidget(this);
    tempWidget->setGeometry(this->width()/2+120, 0, this->width()/2-120, this->height());

    SingleView *singleview = new SingleView(tempWidget);
    singleview->show();


    // connect displaying images
    connect(myController, SIGNAL(load1stImage(QImage)),
            this, SLOT(initialVideoPlayerUI(QImage)));
    connect(myController, SIGNAL(processedImage(QImage, QImage, QImage)),
            this, SLOT(updateVideoplayerUI(QImage, QImage, QImage)));



    //connect slide bar values
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setAdaptThresh(int)));
    connect(ui->adaptThreshSlider, SIGNAL(valueChanged(int)),
            ui->diffrerenceNum, SLOT(setNum(int)));

    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setBlkSize(int)));
    connect(ui->blkSizeSlider, SIGNAL(valueChanged(int)),
            ui->blkSizeNum, SLOT(setNum(int)));

    connect(ui->dilSizeSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setDilSize(int)));
    connect(ui->dilSizeSlider, SIGNAL(valueChanged(int)),
            ui->dilSizeNum, SLOT(setNum(int)));

    connect(ui->blebSizeRatioSlider, SIGNAL(valueChanged(int)),
            myController, SLOT(setblebSizeRatio(int)));
    connect(ui->blebSizeRatioSlider, SIGNAL(valueChanged(int)),
            this, SLOT(updateBlebSizeSliderText(int)));


    // connect combobox to controller // single cell / fix window ...
    connect(ui->typeComboBox, SIGNAL(currentIndexChanged(int)),
            myController, SLOT(setVideoType(int)));

    ui->typeComboBox->setStyleSheet(BUTTON_RELEASED_ON);
    ui->typeComboBox->addItem("Single cell (no overlapping)");
    ui->typeComboBox->addItem("Fix cell window");
    ui->typeComboBox->addItem("Flexible cell window");


    // connect lineEditors to setPixel2micMeter event
    ui->pixelLineEdit->setText("300");
    ui->mmeterLineEdit->setText("20");
    connect(ui->pixelLineEdit, SIGNAL(textChanged(QString)),
            myController, SLOT(setPixel(QString)));
    connect(ui->mmeterLineEdit, SIGNAL(textChanged(QString)),
            myController, SLOT(setMicMeter(QString)));
//    connect(ui->pixelLineEdit, SIGNAL(textChanged(QString)),  //!!!!
//            narr2Vis, SLOT(setPixel(QString)));
//    connect(ui->mmeterLineEdit, SIGNAL(textChanged(QString)),
//            narr2Vis, SLOT(setMicMeter(QString)));



    ui->orgVideo->setStyleSheet(VIDEO_DISPLAY);
    ui->roiVideo1->setStyleSheet(VIDEO_DISPLAY);
    ui->roiVideo2->setStyleSheet(VIDEO_DISPLAY);

    ui->loadVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
    ui->loadVideoButton->setEnabled(true);
    ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
    ui->playVideoButton->setEnabled(false);
    ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
    ui->stopVideoButton->setEnabled(false);
    ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_OFF);
    ui->drawROIButton->setEnabled(false);


    ui->propComboBox->setStyleSheet(BUTTON_RELEASED_ON);
    ui->propComboBox->addItem("Area");
    ui->propComboBox->addItem("Perimeter");
    ui->propComboBox->addItem("Bleb size & number");

    ui->horizontalSlider->setEnabled(false);

    ui->adaptThreshSlider->setRange(1, 51);
    ui->adaptThreshSlider->setValue(3); // initial value of constValue for adaptiveThreshold
    ui->blkSizeSlider->setRange(1, 20);
    ui->blkSizeSlider->setValue(11); // initial value of block size for adaptiveThreshold
    ui->dilSizeSlider->setRange(1, 8);
    ui->dilSizeSlider->setValue(5); // initial value of dilation size for fiding contours
    ui->blebSizeRatioSlider->setRange(1, 10);
    ui->blebSizeRatioSlider->setValue(7);

    ui->videoDisplayerLabel->setStyleSheet(TRANS_BKGRD+FORGRD_BLUE+FONT20);
    ui->videoDisplayerLabel->setText("Video");

    ui->contourDisplayerLabel->setStyleSheet(TRANS_BKGRD+FORGRD_BLUE+FONT16);
    ui->contourDisplayerLabel->setText("Cell Contour");
    ui->cellDetectionDisplayerLabel->setStyleSheet(TRANS_BKGRD+FORGRD_BLUE+FONT16);
    ui->cellDetectionDisplayerLabel->setText("Cell Detection");

    ui->differenceLabel->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");
    ui->differenceLabel->setText("Neighb Diff ");
    ui->diffrerenceNum->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");

    ui->blkSizeLabel->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");
    ui->blkSizeLabel->setText("Neighb Size ");
    ui->blkSizeNum->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");

    ui->dilSizeLabel->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");
    ui->dilSizeLabel->setText("Dilation Size ");
    ui->dilSizeNum->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");

    ui->blebSizeRatioLabel->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");
    ui->blebSizeRatioLabel->setText("Bleb Size Ratio ");
    ui->blebSizeRatioNum->setStyleSheet(TRANS_BKGRD+FORGRD_GRAY+"font:12px");

    ui->frameLabelLeft->setStyleSheet(FRM_LABEL);
    ui->frameLabelLeft->setText("Frame");
    ui->frameLabelRight->setStyleSheet(FRM_LABEL);
    ui->frameLabelRight->setText("0000");


    // connect properties and dataVis
    qRegisterMetaType<floatArray>("floatArray");
    qRegisterMetaType<QVector<QPoint> >("QVector<QPoint>");

    // Vis
    narr1Vis = new Narr();
    narr1Vis->resize(512, 512);
    narr1Vis->show();
    QWidget *container = QWidget::createWindowContainer(narr1Vis, this->centralWidget());
    QRect narr1VisRect = QRect(40, 600, /*(this->width()-160)/2*/600, 600);
    container->setGeometry(narr1VisRect);

    // connect property type combobox to function
    connect(ui->propComboBox, SIGNAL(currentIndexChanged(int)),
            narr1Vis, SLOT(setPropType(int)));


    narr2Vis = new Coord();
    narr2Vis->resize(512, 512);
    narr2Vis->show();
    QWidget *container2 = QWidget::createWindowContainer(narr2Vis, this->centralWidget());
    QRect narr2VisRect = QRect(40+620, 600, /*(this->width()-160)/2*/600, 600);
    container2->setGeometry(narr2VisRect);



    encircler = new Encircle(this->centralWidget());
    //encircle->setGeometry(40, 30, 500, 500);
    encircled = false;

    loadCellDataAct = new QAction(tr("&Load Processed Temporal Data"), this);
    connect(loadCellDataAct, SIGNAL(triggered()), this, SLOT(loadCellData()));
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadCellDataAct);
}

MainWindow::~MainWindow(){
    delete dataFilename;
    delete loadCellDataAct;
    delete fileMenu;
    delete narr1Vis;
    delete narr2Vis;
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

    QSize si = myController->getVideoSize(/*width, height*/);
    int width = si.width(), height = si.height(); // size of original video

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
        ui->frameLabelRight->setStyleSheet(FRM_LABEL);
        if(currFrm == totalFrm){
            ui->playVideoButton->setEnabled(false);
            ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
            ui->stopVideoButton->setEnabled(true);
            ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
            ui->drawROIButton->setEnabled(false);
            ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_OFF);
        }
    }

}

void MainWindow::on_playVideoButton_pressed(){
    ui->playVideoButton->setStyleSheet(BUTTON_PRESSED);
}
void MainWindow::on_playVideoButton_released(){
    if(ui->playVideoButton->isEnabled())
        ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
    else
        ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
}
// play or pause video
void MainWindow::on_playVideoButton_clicked()
{
    cout << "'Play/Pause Video' Button clicked." << endl;
    if(myController->videoIsPaused()){
        myController->playVideo();
        ui->playVideoButton->setText("Pause");
        ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
        ui->stopVideoButton->setEnabled(false);
        ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
        ui->drawROIButton->setEnabled(false);
        ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_OFF);
    }else{
        myController->pauseVideo();
        ui->playVideoButton->setText("Play");
        ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
        ui->stopVideoButton->setEnabled(true);
        ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
        if(!encircled){
            ui->drawROIButton->setEnabled(true);
            ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_ON);
        }
    }
}

void MainWindow::on_stopVideoButton_pressed(){
    ui->stopVideoButton->setStyleSheet(BUTTON_PRESSED);
}
void MainWindow::on_stopVideoButton_released(){
    if(ui->stopVideoButton->isEnabled())
        ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
    else
        ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
}
//stop and release video
void MainWindow::on_stopVideoButton_clicked(){
    cout << "'Stop Video' Button clicked." << endl;

    if(!myController->videoIsPaused())
        myController->pauseVideo();
    myController->releaseVideo();
    ui->frameLabelRight->clear();

    encircled = false;

    narr1Vis->clear();
    narr2Vis->clear();

    ui->typeComboBox->setEnabled(true);
    ui->typeComboBox->setStyleSheet(BUTTON_RELEASED_ON);

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
    ui->loadVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
    ui->playVideoButton->setText("play");
    ui->playVideoButton->setEnabled(false);
    ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
    ui->stopVideoButton->setEnabled(false);
    ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
    ui->horizontalSlider->setValue(0);
    ui->drawROIButton->setEnabled(false);
    ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_OFF);
    ui->contourDisplayerLabel->setStyleSheet(TRANS_BKGRD+FORGRD_WHITE+FONT16BLD);
    ui->cellDetectionDisplayerLabel->setStyleSheet(TRANS_BKGRD+FORGRD_WHITE+FONT16BLD);

}

void MainWindow::on_loadVideoButton_pressed(){
    ui->loadVideoButton->setStyleSheet(BUTTON_PRESSED);

}
void MainWindow::on_loadVideoButton_released(){
    if(ui->loadVideoButton->isEnabled())
        ui->loadVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
    else
        ui->loadVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
}

void MainWindow::on_loadVideoButton_clicked()
{
    fileMode = false;
    cout << "'Load Video' Button clicked." << endl;

    narr1Vis->clear();
    narr2Vis->clear();

    //*** realtime recognition mode (fileMode = false) //
    connect(myController, SIGNAL(detectedProperties(floatArray)),
            this, SLOT(updatePropsVisUI(floatArray)));
    connect(myController, SIGNAL(detectedCellImg(QImage, QVector<QPoint>)),
            this, SLOT(updateCellImg(QImage, QVector<QPoint>)));


    QFileDialog *dialog = new QFileDialog();
    QString filename = dialog->getOpenFileName(this,
                                               tr("Open Video"),
                                               "../../../video", /*QDir::homePath()+"/Desktop/",*/
                                               tr("Video Files (*.mov *mp4 *wmv *mpeg)"));

    //prepare writing data to file
    QFileInfo   fi  = QFileInfo(filename);
    QString     fb  = fi.baseName();
    QString     fp  = fi.path();
    QString     ff  = fp+"/"+fb;

    if(ui->checkBox_compressed->isChecked())
        ff = ff + "_compressed";
    else
        ff = ff + "_control";

    QDir dir(ff);
    if(!dir.exists()){
        qDebug() << "Creating " << ff << "directory";
        dir.mkpath(ff);
    }
    else{
        qDebug() << ff << " already exists";
    }

    this->setWindowTitle(" Dancing Cell Visualization: "+fi.fileName());
    delete dialog;

    if (!filename.isEmpty()){
        if (!myController->loadVideo(filename.toStdString(), ff.toStdString(), fb.toStdString())){
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        else{
            ui->loadVideoButton->setEnabled(false);
            ui->loadVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
            ui->playVideoButton->setEnabled(true);
            ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
            ui->drawROIButton->setEnabled(false);
            ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_OFF);
            ui->horizontalSlider->setMaximum(myController->getNumberOfFrames());
            ui->frameLabelRight->setText(" 0 / " + QString::number(myController->getNumberOfFrames()));
            narr1Vis->setMaxFrm(myController->getNumberOfFrames());
            narr2Vis->getMaxSize(myController->getVideoSize());
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }
}

void MainWindow::on_drawROIButton_pressed(){
    ui->drawROIButton->setStyleSheet(BUTTON_PRESSED);
}

void MainWindow::on_drawROIButton_released(){
    if(ui->drawROIButton->isEnabled())
        ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_ON);
    else
        ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_OFF);
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

    if(!fileMode){
        narr1Vis->updateProperty(property, myController->getCurrentFrame()/*property[0]*/);
        narr2Vis->updateCoord(QPointF(property[3], property[4]), myController->getCurrentFrame()/*property[0]*/);
    }else{
        narr1Vis->updateProperty(property, property[0]);
        narr2Vis->updateCoord(QPointF(property[3], property[4]), property[0]);
    }

}

void MainWindow::updateCellImg(QImage cell, QVector<QPoint> smoothContour)
{
    //cell.save("cellImg"+QString::number(int(myController->getCurrentFrame()))+".png", "PNG");
    narr1Vis->updateCellImg(cell, smoothContour);
}

void MainWindow::updateCellImg(QImage cell)
{
    //cell.save("cellImg"+QString::number(int(myController->getCurrentFrame()))+".png", "PNG");
    narr1Vis->updateCellImg(cell);

}


void MainWindow::control_box_checked(int state)
{
    if (state == Qt::Checked) {
        ui->checkBox_compressed->setCheckState(Qt::Unchecked);
        myController->compressedCell = false;
    }else{
        ui->checkBox_compressed->setCheckState(Qt::Checked);
        myController->compressedCell = true;
    }
}

void MainWindow::compressed_box_checked(int state)
{
    if (state == Qt::Checked) {
        ui->checkBox_control->setCheckState(Qt::Unchecked);
        myController->compressedCell = true;
    }else{
        ui->checkBox_control->setCheckState(Qt::Checked);
        myController->compressedCell = false;
    }
}



void MainWindow::loadCellData()
{
    fileMode = true;
    MultiView *multiview = new MultiView(this->centralWidget());
    multiview->show();
//    this->setCentralWidget(multiview);

//    cout << "'Load Cell Data' menu selected." << endl;

//    narr1Vis->clear();
//    narr2Vis->clear();
//    cellData.clear();

//    //*** read-property-from-file mode (fileMode = true) //
//    connect(this, SIGNAL(readProperties(floatArray)),
//            this, SLOT(updatePropsVisUI(floatArray)));
//    connect(this, SIGNAL(readCellImg(QImage)),
//            this, SLOT(updateCellImg(QImage)));
//    // read-property-from-file mode (fileMode = true) *** //

//    QFileDialog *dialog = new QFileDialog();
//    *dataFilename = dialog->getOpenFileName(this,
//                                            tr("Open Video"),
//                                            "../../../video", /*QDir::homePath()+"/Desktop/",*/
//                                            tr("Data Files (*.csv)"));
//    delete dialog;

//    if(!dataFilename->isEmpty()){
//        //prepare writing data to file
//        QFileInfo   fi  = QFileInfo(*dataFilename);
//        //QString     ff  = fi.path()+"/"+fi.baseName();
//        //string      fn  = ff.toUtf8().constData();

//        this->setWindowTitle(" Dancing Cell Visualization: "+fi.fileName());

//        if(readDataFile()){
//            unsigned int cellDataSize = cellData.size();
//            if(cellDataSize > 20){
//                narr1Vis->setBeginFrame(cellData[0][0]);
//                narr1Vis->setMaxFrm(cellData[cellDataSize-2][0]);
//                narr2Vis->getMaxFrm(cellData[cellDataSize-2][0]);
//                narr2Vis->getMaxSize(QSize(640, 480));
//                for(unsigned int n = 0; n < cellDataSize; n++){
//                    // data
//                    emit readProperties(cellData[n]);
//                    // img
//                    QImage img = readImgFile(fi.path(), cellData[n][0]/*index*/);
//                    emit readCellImg(img);
//                }
//            }
//            else{
//                qDebug() << "Cell Data Size ERROR!";
//            }
//        }
//    }
//    else{
//        QMessageBox msgBox;
//        msgBox.setText("Filename empty!");
//        msgBox.exec();
//    }

////    delete dataFilename;
////    dataFilename = new QString("");
}

bool MainWindow::readDataFile()
{
    QFile f(*dataFilename);
    if(!f.open(QIODevice::ReadOnly)){
        qDebug() << "Reading csv file not found.";
        return false;
    }else{
        QTextStream in(&f);
        while(!in.atEnd()) { // each row
            QString line = in.readLine();
            if(line.isEmpty()){
                continue;
            }
            if(line.isNull()){
                break;
            }
            QVector<float> row;
            foreach (const QString &cell, line.split(",")) {
                //row.append(cell.trimmed());
                row.append(cell.trimmed().toFloat());
            }
            //qDebug() << row;
            floatArray prop;
            prop.push_back(float(row[0])); // frameIndex
            prop.push_back(float(row[1])); // area
            prop.push_back(float(row[2])); // perimeter
            prop.push_back(float(row[3])); // centroid.x
            prop.push_back(float(row[4])); // centroid.y
            prop.push_back(float(row[5])); // shape
            prop.push_back(float(row[6])); // blebNum
//            for(unsigned int n = 0; n < prop.size(); n++)
//                std::cout << prop[n] << " ";
//            std::cout << std::endl;
            cellData.push_back(prop);
            //emit readProperties(prop);
        }
        f.close();
        return true;
    }
}

QImage MainWindow::readImgFile(QString fp, unsigned int idx) // filepath, index
{
    QImage img;
    QString str = fp+"/cell"+QString::number(int(idx))+".png";
    //qDebug() << str;
    img.load(str);
    return img;
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
        ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
        ui->stopVideoButton->setEnabled(false);
        ui->stopVideoButton->setStyleSheet(BUTTON_RELEASED_OFF);
        ui->drawROIButton->setText("Track Cell");

        myController->pauseVideo();

        narr1Vis->setBeginFrame(myController->getCurrentFrame());

        encircler->clearCircle();
        encircler->turnOnEncircleMode();
        encircled = true;
    }

    // when circling mode is turned off, track starts
    // pass the circled region to controller
    // and clear the drawing
    else{
        //ui->stopVideoButton->setEnabled(true);
        ui->playVideoButton->setEnabled(true);
        ui->playVideoButton->setStyleSheet(BUTTON_RELEASED_ON);
        on_playVideoButton_clicked();
        ui->drawROIButton->setText("Encircle Cell");
        ui->drawROIButton->setStyleSheet(BUTTON_RELEASED_OFF);
        ui->contourDisplayerLabel->setStyleSheet(HALFTRANS_BKGRD+FORGRD_ORAGE+"border-radius:4px;"+FONT16BLD);
        ui->cellDetectionDisplayerLabel->setStyleSheet(HALFTRANS_BKGRD+FORGRD_GREEN+"border-radius:4px;"+FONT16BLD);
        this->setCursor(Qt::ArrowCursor);

        ui->typeComboBox->setEnabled(false);
        ui->typeComboBox->setStyleSheet(BUTTON_RELEASED_OFF);

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
            cout << "area min " << area_min << " max " << area_max << endl;
            cout << "prmt min " << prmt_min << " max " << prmt_max << endl;
            myController->setCircle(circle);

        }
    }
}
