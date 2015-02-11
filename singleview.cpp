#include "singleview.h"

SingleView::SingleView(QWidget *parent) : QWidget(parent)
{
    mainVLayout = new QVBoxLayout(this);
    topHLayout = new QHBoxLayout();
    butHLayout = new QHBoxLayout();

    //orignal video
    _orgVideo = new QLabel("orgVideo");
    topHLayout->addWidget(_orgVideo);

    //roi video1 and roi video2
    roiVideoVLayout = new QVBoxLayout();
    roiVdo1 = new QLabel("roi1");
    roiVdo2 = new QLabel("roi2");
    roiVideoVLayout->addWidget(roiVdo1);
    roiVideoVLayout->addWidget(roiVdo2);
    topHLayout->addLayout(roiVideoVLayout);

    //---control panel---
    // playBar
    playBarHLayout = new QHBoxLayout();
    playBarHLayout->addWidget(new QLabel("playBar1"));
    playBarHLayout->addWidget(new QLabel("playBar2"));
    playBarHLayout->addWidget(new QLabel("playBar3"));

    // playerButton
    playerButHLayout = new QHBoxLayout();
    playerButHLayout->addWidget(new QPushButton("playerButton1"));
    playerButHLayout->addWidget(new QPushButton("playerButton2"));
    playerButHLayout->addWidget(new QPushButton("playerButton3"));

    // videoType
    videoTypeHLayout = new QHBoxLayout();
    videoTypeHLayout->addWidget(new QLabel("videoType"));

    // cellRole
    cellRoleHLayout = new QHBoxLayout();
    cellRoleHLayout->addWidget(new QLabel("compressed"));
    cellRoleHLayout->addWidget(new QLabel("control"));

    // prmter sliderbars
    prmtrGridLayout = new QGridLayout();
    prmtrGridLayout->addWidget(new QLabel("00"), 0, 0);
    prmtrGridLayout->addWidget(new QLabel("01"), 0, 1);
    prmtrGridLayout->addWidget(new QLabel("02"), 0, 2);
    prmtrGridLayout->addWidget(new QLabel("03"), 0, 3);
    prmtrGridLayout->addWidget(new QLabel("10"), 1, 0);
    prmtrGridLayout->addWidget(new QLabel("13"), 1, 3);
    prmtrGridLayout->addWidget(new QLabel("20"), 2, 0);
    prmtrGridLayout->addWidget(new QLabel("21"), 2, 1);
    prmtrGridLayout->addWidget(new QLabel("22"), 2, 2);
    prmtrGridLayout->addWidget(new QLabel("23"), 2, 3);
    prmtrGridLayout->addWidget(new QLabel("30"), 3, 0);
    prmtrGridLayout->addWidget(new QLabel("33"), 3, 3);

    //encircle button
    encircleButtonHLayout = new QHBoxLayout();
    encircleButtonHLayout->addWidget(new QPushButton("encircle"));

    //scale converter
    scaleConvertHLayout = new QHBoxLayout();
    scaleConvertHLayout->addWidget(new QLineEdit());
    scaleConvertHLayout->addWidget(new QLabel("pixel as"));
    scaleConvertHLayout->addWidget(new QLineEdit());
    scaleConvertHLayout->addWidget(new QLabel("µm"));

    // property combobox
    propertyHLayout = new QHBoxLayout();
    propertyHLayout->addWidget(new QLabel("Property"));
    propertyHLayout->addWidget(new QComboBox());

    controlVLayout = new QVBoxLayout();
    controlVLayout->addLayout(playBarHLayout);
    controlVLayout->addLayout(playerButHLayout);
    controlVLayout->addLayout(videoTypeHLayout);
    controlVLayout->addLayout(cellRoleHLayout);
    controlVLayout->addLayout(prmtrGridLayout);
    controlVLayout->addLayout(encircleButtonHLayout);
    controlVLayout->addLayout(scaleConvertHLayout);
    controlVLayout->addLayout(propertyHLayout);

    topHLayout->addLayout(controlVLayout);



    mainVLayout->addLayout(topHLayout);


    butHLayout->addWidget(new QPushButton());
    butHLayout->addWidget(new QPushButton());


    mainVLayout->addLayout(butHLayout);


}


