#include "singleview.h"
#include "style.h"

SingleView::SingleView(QWidget *parent) : QWidget(parent)
{
    resize(1024, 1024);
    setStyleSheet("background-color:rgb(251,251,251)");

    mainVLayout = new QVBoxLayout(this);
    topHLayout = new QHBoxLayout();
    botHLayout = new QHBoxLayout();

    //*************** Top Half ***************

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



    //*************** Top Half ***************
    createNarrVis();
    createCoordVis();

    botHLayout->addWidget(nar_container);
    botHLayout->addWidget(cod_container);


    //*************** Whole ***************
    mainVLayout->addLayout(topHLayout, 1);
    mainVLayout->addLayout(botHLayout, 1);


}

void SingleView::createOrignalVideo()
{
    //orignal video
    orgVideoVLayout = new QVBoxLayout();
    _orgVideo = new QLabel("orgVideo");
    _orgVideo->setMaximumHeight(this->height()/2);
    _orgVideo->setStyleSheet(VIDEO_DISPLAY);
    orgVideoVLayout->addWidget(_orgVideo);
    topHLayout->addLayout(orgVideoVLayout, 3);

}

void SingleView::createROIVideo()
{
    //roi video1 and roi video2
    roiVideoVLayout = new QVBoxLayout();
    roiVdo1 = new QLabel("roi1");
    roiVdo2 = new QLabel("roi2");
    roiVdo1->setMaximumHeight(this->height()/4);
    roiVdo2->setMaximumHeight(this->height()/4);
    roiVdo1->setStyleSheet(VIDEO_DISPLAY);
    roiVdo2->setStyleSheet(VIDEO_DISPLAY);
    roiVideoVLayout->addWidget(roiVdo1);
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
    blebSizeValSlidr = new QSlider(Qt::Horizontal);
    blebSizeValSlidr->setMaximumHeight(30);
    blebSizeValSlidr->setStyleSheet(TRANS_BKGRD);

    diffValSlidr->setRange(1, 51);
    diffValSlidr->setValue(3);
    blkSizeValSlidr->setRange(1, 20);
    blkSizeValSlidr->setValue(11);
    dilValSlidr->setRange(1, 8);
    dilValSlidr->setValue(5);
    blebSizeValSlidr->setRange(1, 10);
    blebSizeValSlidr->setValue(7);

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
    prmtrGridLayout->addWidget(blebSizeValSlidr, 3, 2, 3, 2);

    prmterGroup->setLayout(prmtrGridLayout);
    prmterGroup->setMaximumHeight(120);

    connect(diffValSlidr,    SIGNAL(valueChanged(int)), diffValLabel, SLOT(setNum(int)));
    connect(blkSizeValSlidr, SIGNAL(valueChanged(int)), blkSizeValLabel, SLOT(setNum(int)));
    connect(dilValSlidr,     SIGNAL(valueChanged(int)), dilSizeValLabel, SLOT(setNum(int)));
    connect(blebSizeValSlidr,SIGNAL(valueChanged(int)), blebSizeValLabel, SLOT(setNum(int)));
}

void SingleView::createEncircler()
{
    //encircle button
    encircleButtonHLayout = new QHBoxLayout();
    drawROI = new QPushButton("Encircle Cell");
    drawROI->setFixedHeight(45);
    drawROI->setStyleSheet(BUTTON_RELEASED_OFF);
    encircleButtonHLayout->addWidget(drawROI);

}

void SingleView::createScaleConverter()
{
    //scale converter
    scaleConvertHLayout = new QHBoxLayout();
    mmeter = new QLineEdit("300");
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
    nar = new Narr();
    nar_container = QWidget::createWindowContainer(nar, this);
    nar_container->setMinimumSize(512, 512);

}

void SingleView::createCoordVis()
{
    cod = new Coord();
    cod_container = QWidget::createWindowContainer(cod, this);
    cod_container->setMinimumSize(512, 512);

}


