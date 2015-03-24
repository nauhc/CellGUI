#include "singleview.h"
#include "style.h"
#include "QDebug"

template <class T>
inline T sqre(T value){
    return value*value;
}

SingleView::SingleView(QWidget *parent) : QWidget(parent), controller(new Controller())
{
    // connect properties and dataVis
    qRegisterMetaType<floatArray>("floatArray");
    qRegisterMetaType<QVector<QPoint> >("QVector<QPoint>");

    //resize(1280, 1280);
    resize(parent->size());
    setStyleSheet("background-color:rgb(251,251,251)");

    mainVLayout = new QVBoxLayout(this);
    topHLayout = new QHBoxLayout();
    botHLayout = new QHBoxLayout();

    //*************** Top Half **************

    //--- Original Video ---
    createOrignalVideo();

    //--- ROI Video ---
    createROIVideo();

    //--- Control Panel ---
    createPlayerController();
    createVideoTypeSeletionComBox();
    createCellRoleCheckBox();
    createPrmtrPanel();
    createEncircler();
    createScaleConverter();
    createPropertySelector();

    topHLayout->addLayout(controlVLayout, 2);
    topHLayout->setGeometry(QRect(0, 0, this->width(), this->height()/2));


    //*************** Bottom Half ***************
    createNarrVis();
    createCoordVis();

    botHLayout->addWidget(nar);
    botHLayout->addWidget(cod);


    //************* Main = top + bottom *********
    mainVLayout->addLayout(topHLayout, 1);
    mainVLayout->addLayout(botHLayout, 1);

    connect(controller, SIGNAL(load1stImage(QImage)),
            this, SLOT(initialVideoPlayerUI(QImage)));
    connect(controller, SIGNAL(processedImage(QImage,QImage,QImage)),
            this, SLOT(updateVideoplayerUI_(QImage,QImage,QImage)));

    encircler_ = new Encircle(this);
    encircled_ = false;

}

SingleView::~SingleView()
{
    delete nar;
    delete cod;
    delete encircler_;
    delete controller;
}

void SingleView::initialVideoPlayerUI(QImage img)
{
    if(!img.isNull()){
        //original video display
        setCanvas();
        _orgVideo->setAlignment(Qt::AlignCenter);
        _orgVideo->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        _orgVideo->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        //roi video display 1
        roiVdo1->setAlignment(Qt::AlignCenter);
        roiVdo1->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        roiVdo1->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        roiVdo2->setAlignment(Qt::AlignCenter);
        roiVdo2->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        roiVdo2->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
    }

}

void SingleView::updateVideoplayerUI_(QImage img, QImage ROIimg1, QImage ROIimg2)
{
    if(!img.isNull()){
        //original video display
        _orgVideo->setAlignment(Qt::AlignCenter);
        _orgVideo->setPixmap(
                    QPixmap::fromImage(img).scaled(
                        _orgVideo->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        //roi video display 1
        roiVdo1->setAlignment(Qt::AlignCenter);
        roiVdo1->setPixmap(
                    QPixmap::fromImage(ROIimg1).scaled(
                        roiVdo1->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));
        roiVdo2->setAlignment(Qt::AlignCenter);
        roiVdo2->setPixmap(
                    QPixmap::fromImage(ROIimg2).scaled(
                        roiVdo2->size(),
                        Qt::KeepAspectRatio,
                        Qt::FastTransformation));

        int currFrm     = controller->getCurrentFrame();
        int totalFrm    = controller->getNumberOfFrames();
        playerSlider->setValue(currFrm);
        frmLabelRight->setText(QString::number(currFrm) +" / "+ QString::number(totalFrm));
        frmLabelRight->setStyleSheet(FRM_LABEL);
        if(currFrm == totalFrm){
            playButton->setEnabled(false);
            playButton->setStyleSheet(BUTTON_RELEASED_OFF);
            stopButton->setEnabled(true);
            stopButton->setStyleSheet(BUTTON_RELEASED_ON);
            drawROI->setEnabled(false);
            drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
        }
    }
}

void SingleView::loadButton_pressed()
{
    loadButton->setStyleSheet(BUTTON_PRESSED);
}

void SingleView::loadButton_released()
{
    if(loadButton->isEnabled())
        loadButton->setStyleSheet(BUTTON_RELEASED_ON);
    else
        loadButton->setStyleSheet(BUTTON_RELEASED_OFF);
}

void SingleView::loadButton_clicked()
{
    cout << "'Load Video' Button clicked." << endl;

    nar->clear();
    cod->clear();

    //*** realtime recognition mode (fileMode = false) //
    connect(controller, SIGNAL(detectedProperties(floatArray)),
            this, SLOT(updatePropsVisUI_(floatArray)));
    connect(controller, SIGNAL(detectedCellImg(QImage,QVector<QPoint>)),
            this, SLOT(updateCellImg_(QImage,QVector<QPoint>)));

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

    if(compressedCheckBox->isChecked())
        ff = ff + "_compressed";
    else
        ff = ff + "_control";

    QDir dir(ff);
    if(!dir.exists()){
        qDebug() << "Creating " << ff << " directory";
        dir.mkpath(ff);
    }
    else{
        qDebug() << ff << " already exists";
    }

    this->setWindowTitle(" Dancing Cell Visualization: "+fi.fileName());
    //this->setWindowTitle(" Dancing Cell Visualization: "+fi.fileName());
    delete dialog;
    if (!filename.isEmpty()){
        if (!controller->loadVideo(filename.toStdString(), ff.toStdString(), fb.toStdString())){
            QMessageBox msgBox;
            msgBox.setText("The selected video could not be opened!");
            msgBox.exec();
        }
        else{
            loadButton->setEnabled(false);
            loadButton->setStyleSheet(BUTTON_RELEASED_OFF);
            playButton->setEnabled(true);
            playButton->setStyleSheet(BUTTON_RELEASED_ON);
            drawROI->setEnabled(false);
            drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
            playerSlider->setMaximum(controller->getNumberOfFrames());
            frmLabelRight->setText(" 0 / " + QString::number(controller->getNumberOfFrames()));
            nar->setMaxFrm(controller->getNumberOfFrames());
            cod->setMaxSize(controller->getVideoSize());
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }

}

void SingleView::updatePropsVisUI_(floatArray property)
{
    nar->updateProperty(property, controller->getCurrentFrame()/*property[0]*/);
    cod->updateCoord(QPointF(property[3], property[4]), controller->getCurrentFrame()/*property[0]*/);
    //qDebug() << QPointF(property[3], property[4]) << controller->getCurrentFrame();
}

void SingleView::updateCellImg_(QImage cell, QVector<QPoint> smoothContour)
{
    nar->updateCellImg(cell, smoothContour);
}

void SingleView::updateCellImg_(QImage cell)
{
    nar->updateCellImg(cell);
}

void SingleView::playButton_pressed()
{
    playButton->setStyleSheet(BUTTON_PRESSED);
}

void SingleView::playButton_released()
{
    if(playButton->isEnabled())
        playButton->setStyleSheet(BUTTON_RELEASED_ON);
    else
        playButton->setStyleSheet(BUTTON_RELEASED_OFF);
}

void SingleView::playButton_clicked()
{
    cout << "'Play/Pause Video' Button clicked." << endl;
    if(controller->videoIsPaused()){
        controller->playVideo();
        playButton->setText("Pause");
        playButton->setStyleSheet(BUTTON_RELEASED_ON);
        stopButton->setEnabled(false);
        stopButton->setStyleSheet(BUTTON_RELEASED_OFF);
        drawROI->setEnabled(false);
        drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
    }else{
        controller->pauseVideo();
        playButton->setText("Play");
        playButton->setStyleSheet(BUTTON_RELEASED_ON);
        stopButton->setEnabled(true);
        stopButton->setStyleSheet(BUTTON_RELEASED_ON);
        if(!encircled_){
            drawROI->setEnabled(true);
            drawROI->setStyleSheet(BUTTON_RELEASED_ON);
        }
    }
}

void SingleView::stopButton_pressed()
{
    stopButton->setStyleSheet(BUTTON_PRESSED);
}

void SingleView::stopButton_released()
{
    if(stopButton->isEnabled())
        stopButton->setStyleSheet(BUTTON_RELEASED_ON);
    else
        stopButton->setStyleSheet(BUTTON_RELEASED_OFF);
}

void SingleView::stopButton_clicked()
{
    cout << "'Stop Video' Button clicked." << endl;

    if(!controller->videoIsPaused())
        controller->pauseVideo();
    controller->releaseVideo();
    frmLabelRight->clear();

    encircled_ = false;

    nar->clear();
    cod->clear();

    typeComBox->setEnabled(true);
    typeComBox->setStyleSheet(BUTTON_RELEASED_ON);

    QPixmap pixmap(1,1); // Works
    pixmap = pixmap.scaled(_orgVideo->width(), _orgVideo->height());
    pixmap.fill(QColor(216,222,224));
    _orgVideo->setPixmap(pixmap);
    QPixmap pixmap1(1,1); // Works
    pixmap1 = pixmap1.scaled(roiVdo1->width(), roiVdo1->height());
    pixmap1.fill(QColor(216,222,224));
    roiVdo1->setPixmap(pixmap1);
    QPixmap pixmap2(1,1); // Works
    pixmap2 = pixmap2.scaled(roiVdo2->width(), roiVdo2->height());
    pixmap2.fill(QColor(216,222,224));
    roiVdo2->setPixmap(pixmap2);

    loadButton->setEnabled(true);
    loadButton->setStyleSheet(BUTTON_RELEASED_ON);
    playButton->setText("play");
    playButton->setEnabled(false);
    playButton->setStyleSheet(BUTTON_RELEASED_OFF);
    stopButton->setEnabled(false);
    stopButton->setStyleSheet(BUTTON_RELEASED_OFF);
    playerSlider->setValue(0);
    drawROI->setEnabled(false);
    drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
//    ui->contourDisplayerLabel->setStyleSheet(TRANS_BKGRD+FORGRD_WHITE+FONT16BLD);
//    ui->cellDetectionDisplayerLabel->setStyleSheet(TRANS_BKGRD+FORGRD_WHITE+FONT16BLD);

}

void SingleView::createOrignalVideo()
{
    //orignal video
    orgVideoVLayout = new QVBoxLayout();
    _orgVideo = new QLabel(""); //orignal video
//    _orgVideo->setMinimumWidth(55);
//    _orgVideo->setMinimumHeight(512);
    _orgVideo->setFixedSize(550, 550);
    //_orgVideo->setMaximumHeight(this->height()/2);
    _orgVideo->setStyleSheet(VIDEO_DISPLAY);
    _orgVideo->heightForWidth(_orgVideo->width());
    orgVideoVLayout->addWidget(_orgVideo);
    topHLayout->addLayout(orgVideoVLayout, 3);

}

void SingleView::createROIVideo()
{
    //roi video1 and roi video2
    roiVideoVLayout = new QVBoxLayout();
    roiVdo1Label = new QLabel("Cell Contour");
    roiVdo2Label = new QLabel("Cell Detection");
    roiVdo1Label->setMaximumHeight(45);
    roiVdo2Label->setMaximumHeight(45);
    roiVdo1Label->setStyleSheet("color:rgb(28, 120, 159); font: bold 18px; background-color:rgba(0,0,0,0%)");
    roiVdo2Label->setStyleSheet("color:rgb(28, 120, 159); font: bold 18px; background-color:rgba(0,0,0,0%)");
    roiVdo1 = new QLabel(""); //roi1
    roiVdo2 = new QLabel(""); //roi2
//    roiVdo1->setMaximumHeight(this->height()/4);
//    roiVdo2->setMaximumHeight(this->height()/4);
    roiVdo1->setStyleSheet(VIDEO_DISPLAY);
    roiVdo2->setStyleSheet(VIDEO_DISPLAY);
    roiVideoVLayout->addWidget(roiVdo1Label);
    roiVideoVLayout->addWidget(roiVdo1);
    roiVideoVLayout->addWidget(roiVdo2Label);
    roiVideoVLayout->addWidget(roiVdo2);
    topHLayout->addLayout(roiVideoVLayout, 1);
}

void SingleView::createPlayerController()
{
    // playBar
    playBarHLayout = new QHBoxLayout();
    frmLabelLeft = new QLabel("Frame");
    frmLabelRight = new QLabel("0000");
    frmLabelLeft->setMaximumHeight(30);
    frmLabelRight->setMaximumHeight(30);
    frmLabelLeft->setStyleSheet(FRM_LABEL);
    frmLabelRight->setStyleSheet(FRM_LABEL);
    playerSlider = new QSlider();
    playerSlider->setOrientation(Qt::Horizontal);
    playerSlider->setMaximumHeight(30);
    playBarHLayout->addWidget(frmLabelLeft);
    playBarHLayout->addWidget(playerSlider);
    playBarHLayout->addWidget(frmLabelRight);

    // playerButton
    playerButHLayout = new QHBoxLayout();
    loadButton = new QPushButton("File");
    playButton = new QPushButton("Play");
    stopButton = new QPushButton("Stop");
    loadButton->setStyleSheet(BUTTON_RELEASED_ON);
    playButton->setStyleSheet(BUTTON_RELEASED_OFF);
    stopButton->setStyleSheet(BUTTON_RELEASED_OFF);
    loadButton->setMinimumHeight(45);
    playButton->setMinimumHeight(45);
    stopButton->setMinimumHeight(45);
    playerButHLayout->addWidget(loadButton);
    playerButHLayout->addWidget(playButton);
    playerButHLayout->addWidget(stopButton);

    connect(this->loadButton, SIGNAL(clicked()), this, SLOT(loadButton_clicked()));
    connect(this->loadButton, SIGNAL(pressed()), this, SLOT(loadButton_pressed()));
    connect(this->loadButton, SIGNAL(released()), this, SLOT(loadButton_released()));

    connect(this->playButton, SIGNAL(clicked()), this, SLOT(playButton_clicked()));
    connect(this->playButton, SIGNAL(pressed()), this, SLOT(playButton_pressed()));
    connect(this->playButton, SIGNAL(released()), this, SLOT(playButton_released()));

    connect(this->stopButton, SIGNAL(clicked()), this, SLOT(stopButton_clicked()));
    connect(this->stopButton, SIGNAL(pressed()), this, SLOT(stopButton_pressed()));
    connect(this->stopButton, SIGNAL(released()), this, SLOT(stopButton_released()));

}

void SingleView::createVideoTypeSeletionComBox()
{
    // videoType
    videoTypeHLayout = new QHBoxLayout();
    typeComBox = new QComboBox();
    typeComBox->setFixedHeight(30);
    typeComBox->setStyleSheet(BUTTON_RELEASED_ON);
    typeComBox->addItem("Single cell (no overlapping)");
    typeComBox->addItem("Fix cell window");
    typeComBox->addItem("Flexible cell window");
    videoTypeHLayout->addWidget(typeComBox);

    connect(typeComBox, SIGNAL(currentIndexChanged(int)),
            controller, SLOT(setVideoType(int)));
}

void SingleView::compressedCheckBox_checked(int state)
{
    if (state == Qt::Checked) {
        controlCheckBox->setCheckState(Qt::Unchecked);
        controller->compressedCell = true;
    }else{
        controlCheckBox->setCheckState(Qt::Checked);
        controller->compressedCell = false;
    }
}

void SingleView::controlCheckBox_checked(int state)
{
    if (state == Qt::Checked) {
        compressedCheckBox->setCheckState(Qt::Unchecked);
        controller->compressedCell = false;
    }else{
        compressedCheckBox->setCheckState(Qt::Checked);
        controller->compressedCell = true;
    }

}

void SingleView::createCellRoleCheckBox()
{
    // cellRole
    roleGroup = new QGroupBox();
    compressedCheckBox = new QCheckBox("Compressed Cell");
    controlCheckBox = new QCheckBox("Control Cell");
    compressedCheckBox->setChecked(true);
    cellRoleHLayout = new QHBoxLayout();
    cellRoleHLayout->addWidget(compressedCheckBox);
    cellRoleHLayout->addWidget(controlCheckBox);
    cellRoleHLayout->addStretch(1);
    roleGroup->setLayout(cellRoleHLayout);
    roleGroup->setMaximumHeight(50);

    connect(this->compressedCheckBox, SIGNAL(stateChanged(int)), this, SLOT(compressedCheckBox_checked(int)));
    connect(this->controlCheckBox, SIGNAL(stateChanged(int)), this, SLOT(controlCheckBox_checked(int)));

}

void SingleView::updateBlebSizeText(int val)
{
    blebSizeValLabel->setText("1/"+QString::number((11-val)*50));
}

void SingleView::createPrmtrPanel()
{
    // prmter sliderbars
    prmterGroup = new QGroupBox();
    prmtrGridLayout = new QGridLayout();
    QLabel      *diffLabel = new QLabel("Difference:");
    QLabel      *blckSizeLabel = new QLabel("Block Size:");
    QLabel      *dilationSizeLabel = new QLabel("Dilation Size:");
    QLabel      *blebSizeLabel = new QLabel("Bleb Size Ratio:");
    diffLabel->setStyleSheet(TRANS_BKGRD);
    blckSizeLabel->setStyleSheet(TRANS_BKGRD);
    dilationSizeLabel->setStyleSheet(TRANS_BKGRD);
    blebSizeLabel->setStyleSheet(TRANS_BKGRD);
    diffValLabel = new QLabel("3");
    diffValLabel->setMaximumHeight(30);
    diffValLabel->setStyleSheet(TRANS_BKGRD);
    blkSizeValLabel = new QLabel("11");
    blkSizeValLabel->setMaximumHeight(30);
    blkSizeValLabel->setStyleSheet(TRANS_BKGRD);
    dilSizeValLabel = new QLabel("5");
    dilSizeValLabel->setMaximumHeight(30);
    dilSizeValLabel->setStyleSheet(TRANS_BKGRD);
    blebSizeValLabel = new QLabel("1/200");
    blebSizeValLabel->setMaximumHeight(30);
    blebSizeValLabel->setStyleSheet(TRANS_BKGRD);
    diffValSlidr = new QSlider(Qt::Horizontal);
    diffValSlidr->setMaximumHeight(30);
    diffValSlidr->setStyleSheet(TRANS_BKGRD);
    blkSizeValSlidr = new QSlider(Qt::Horizontal);
    blkSizeValSlidr->setMaximumHeight(30);
    blkSizeValSlidr->setStyleSheet(TRANS_BKGRD);
    dilValSlidr = new QSlider(Qt::Horizontal);
    dilValSlidr->setMaximumHeight(30);
    dilValSlidr->setStyleSheet(TRANS_BKGRD);
    blebSizeRtoSlidr = new QSlider(Qt::Horizontal);
    blebSizeRtoSlidr->setMaximumHeight(30);
    blebSizeRtoSlidr->setStyleSheet(TRANS_BKGRD);

    prmtrGridLayout->addWidget(diffLabel, 0, 0);
    prmtrGridLayout->addWidget(diffValLabel, 0, 1);
    prmtrGridLayout->addWidget(blckSizeLabel, 0, 2);
    prmtrGridLayout->addWidget(blkSizeValLabel, 0, 3);
    prmtrGridLayout->addWidget(diffValSlidr, 1, 0, 1, 2);
    prmtrGridLayout->addWidget(blkSizeValSlidr, 1, 2, 1, 2);
    prmtrGridLayout->addWidget(dilationSizeLabel, 2, 0);
    prmtrGridLayout->addWidget(dilSizeValLabel, 2, 1);
    prmtrGridLayout->addWidget(blebSizeLabel, 2, 2);
    prmtrGridLayout->addWidget(blebSizeValLabel, 2, 3);
    prmtrGridLayout->addWidget(dilValSlidr, 3, 0, 3, 2);
    prmtrGridLayout->addWidget(blebSizeRtoSlidr, 3, 2, 3, 2);

    prmterGroup->setLayout(prmtrGridLayout);
    prmterGroup->setMaximumHeight(120);


    connect(diffValSlidr,    SIGNAL(valueChanged(int)), diffValLabel, SLOT(setNum(int)));
    connect(diffValSlidr,    SIGNAL(valueChanged(int)), controller,   SLOT(setAdaptThresh(int)));
    connect(blkSizeValSlidr, SIGNAL(valueChanged(int)), blkSizeValLabel, SLOT(setNum(int)));
    connect(blkSizeValSlidr, SIGNAL(valueChanged(int)), controller,   SLOT(setBlkSize(int)));
    connect(dilValSlidr,     SIGNAL(valueChanged(int)), dilSizeValLabel, SLOT(setNum(int)));
    connect(dilValSlidr,     SIGNAL(valueChanged(int)), controller,   SLOT(setDilSize(int)));
    connect(blebSizeRtoSlidr,SIGNAL(valueChanged(int)), this, SLOT(updateBlebSizeText(int)));
    connect(blebSizeRtoSlidr,SIGNAL(valueChanged(int)), controller,   SLOT(setblebSizeRatio(int)));


    diffValSlidr->setRange(1, 51);
    diffValSlidr->setValue(3);
    blkSizeValSlidr->setRange(1, 20);
    blkSizeValSlidr->setValue(11);
    dilValSlidr->setRange(1, 8);
    dilValSlidr->setValue(5);
    blebSizeRtoSlidr->setRange(1, 10);
    blebSizeRtoSlidr->setValue(7);

}

void SingleView::createEncircler()
{
    //encircle button
    encircleButtonHLayout = new QHBoxLayout();
    drawROI = new QPushButton("Encircle Cell");
    drawROI->setFixedHeight(45);
    drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
    encircleButtonHLayout->addWidget(drawROI);

    connect(drawROI, SIGNAL(clicked()), this, SLOT(drawROIButton_clicked()));

}

void SingleView::drawROIButton_pressed()
{
    drawROI->setStyleSheet(BUTTON_PRESSED);
}

void SingleView::drawROIButton_released()
{
    if(drawROI->isEnabled())
        drawROI->setStyleSheet(BUTTON_RELEASED_ON);
    else
        drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
}

void SingleView::drawROIButton_clicked()
{

//    encircler_->setGeometry(_orgVideo->x(), _orgVideo->y(),
//                           _orgVideo->width(), _orgVideo->height());

    cout << "video pos:  x " << _orgVideo->x()
         << " y " << _orgVideo->y()
         << " width " << _orgVideo->width()
         << " height " << _orgVideo->height() << endl;

    cout << "Encircle Cell Button clicked." << endl;


    // when it is circling mode
    // user can circle the cell of interest
    if(!encircler_->isEncircled()){
        //this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        this->setCursor(Qt::CrossCursor);
        playButton->setEnabled(false);
        playButton->setStyleSheet(BUTTON_RELEASED_OFF);
        stopButton->setEnabled(false);
        stopButton->setStyleSheet(BUTTON_RELEASED_OFF);
        drawROI->setText("Track Cell");

        controller->pauseVideo();

        int startFrm = controller->getCurrentFrame();
        int totalFrm = controller->getNumberOfFrames();
        nar->setBeginFrm(startFrm);
        cod->setBeginFrm(startFrm);
        cod->setMaxFrm(totalFrm-startFrm, totalFrm-startFrm);

        encircler_->clearCircle();
        encircler_->turnOnEncircleMode();
        encircled_ = true;
    }

    // when circling mode is turned off, track starts
    // pass the circled region to controller
    // and clear the drawing
    else{
        //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        //stopButton->setEnabled(true);
        playButton->setEnabled(true);
        playButton->setStyleSheet(BUTTON_RELEASED_ON);
        playButton_clicked();
        drawROI->setText("Encircle Cell");
        drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
//        ui->contourDisplayerLabel->setStyleSheet(HALFTRANS_BKGRD+FORGRD_ORAGE+"border-radius:4px;"+FONT16BLD);
//        ui->cellDetectionDisplayerLabel->setStyleSheet(HALFTRANS_BKGRD+FORGRD_GREEN+"border-radius:4px;"+FONT16BLD);
        this->setCursor(Qt::ArrowCursor);

        typeComBox->setEnabled(false);
        typeComBox->setStyleSheet(BUTTON_RELEASED_OFF);

        encircler_->turnOffEncircleMode();
        //delete encircle;
        QVector<QPoint> circle;
        encircler_->getRegion(circle);
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
            controller->setCircle(circle);

        }
    }
}

void SingleView::createScaleConverter()
{
    //scale converter
    scaleConvertHLayout = new QHBoxLayout();
//    mmeter = new QLineEdit("300");
    mmeter = new QLineEdit("150");
    pixel = new QLineEdit("20");
    QLabel  *pixelas = new QLabel("pixel as");
    QLabel  *mm      = new QLabel("µm");
    mmeter->setMaximumHeight(30);
    pixel->setMaximumHeight(30);
    pixelas->setMaximumHeight(30);
    mm->setMaximumHeight(30);
    scaleConvertHLayout->addWidget(mmeter);
    scaleConvertHLayout->addWidget(pixelas);
    scaleConvertHLayout->addWidget(pixel);
    scaleConvertHLayout->addWidget(mm);
}

void SingleView::createPropertySelector()
{
    // property combobox
    propertyHLayout = new QHBoxLayout();
    propComBox = new QComboBox();
    propComBox->setFixedHeight(30);
    propComBox->addItem("Area");
    propComBox->addItem("Perimeter");
    propComBox->addItem("Bleb size and number");
    propComBox->setEnabled(true);
    propComBox->setStyleSheet(BUTTON_RELEASED_ON);
    propertyHLayout->addWidget(new QLabel("Property"));
    propertyHLayout->addWidget(propComBox);

    controlVLayout = new QVBoxLayout();
    controlVLayout->addLayout(playBarHLayout, 1);
    controlVLayout->addLayout(playerButHLayout, 2);
    controlVLayout->addLayout(videoTypeHLayout, 2);
    controlVLayout->addWidget(roleGroup, 2);
    controlVLayout->addWidget(prmterGroup, 2);
    controlVLayout->addLayout(encircleButtonHLayout, 2);
    controlVLayout->addLayout(scaleConvertHLayout, 2);
    controlVLayout->addLayout(propertyHLayout, 2);

}

void SingleView::createNarrVis()
{
    nar = new Narr(this, false);
//    nar_container = QWidget::createWindowContainer(nar, this);
//    nar_container->setMinimumSize(512, 512);
//    nar_container->setMaximumSize(640, 640);
    nar->setMinimumSize(512, 512);
    nar->setMaximumSize(640, 640);

    connect(propComBox, SIGNAL(currentIndexChanged(int)),
            nar, SLOT(setPropType(int)));
}

void SingleView::createCoordVis()
{
    cod = new Coord(this, false);
//    cod_container = QWidget::createWindowContainer(cod, this);
//    cod_container->setMinimumSize(512, 512);
//    cod_container->setMaximumSize(640, 640);

    cod->setMinimumSize(512, 512);
    cod->setMaximumSize(640, 640);

}

void calcSizes(int &videoWidth, int &videoHeight,
               int x_border, int y_border, int width_border, int height_border,
               int &x,       int &y,       int &width,       int &height,
               double &scale){ // assume the video displaying area is square

    if(videoWidth >= videoHeight){
        scale = double(width_border)/double(videoWidth);
        width   = width_border;
        height  = width_border*videoHeight/videoWidth; //(width > height)
        x = x_border;
        //y = y_border + (width-height)/2;
        y = y_border + (height_border-height)/2;
    }
    else{
        scale = double(height_border)/double(videoHeight);
        height  = height_border;
        width   = height_border*videoWidth/videoHeight; //(width < height)
        //x = x_border + (height-width)/2;
        x = x_border + (width_border-width)/2;
        y = y_border;
    }
}

void SingleView::setCanvas()
{
    //set video player label size and postion
    //according to the size of the selected video

    QSize si = controller->getVideoSize(/*width, height*/);
    int width = si.width(), height = si.height(); // size of original video

    // orgVideo
    int x, y, w, h;
    double scale;
    calcSizes(width, height, _orgVideo->x(), _orgVideo->y(), _orgVideo->width(), _orgVideo->height(), x, y, w, h, scale);
    _orgVideo->setGeometry(x, y, w, h);
    encircler_->setGeometry(x, y, w, h);
    controller->setScale(scale);

    // ROIVideo1
    int x1, y1, w1, h1;
    double scale1;
    calcSizes(width, height, roiVdo1->x(), roiVdo1->y(), roiVdo1->width(), roiVdo1->height(), x1, y1, w1, h1, scale1);
    // orgVideo
    int x2, y2, w2, h2;
    double scale2;
    calcSizes(width, height, roiVdo2->x(), roiVdo2->y(), roiVdo2->width(), roiVdo2->height(), x2, y2, w2, h2, scale2);


//    cout << "orgVideo Widget: x " << _orgVideo->x() << " y " << _orgVideo->y()
//         << " width " << _orgVideo->width() << " height " << _orgVideo->height() << endl;
    cout << "video pos:  x " << x << " y " << y << " width " << w << " height " << h << endl;

}



