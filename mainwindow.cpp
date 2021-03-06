#include "mainwindow.h"
#include <QWidget>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QObject>
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "multiview.h"
#include "singleview.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:rgb(251,251,251)");
    //this->setBaseSize(this->width(), this->height());

    this->setWindowTitle(" Dancing Cell Visualization ");

    centralLayout = new QHBoxLayout();
    this->centralWidget()->setLayout(centralLayout);
    //this->centralWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // add dock for multiview
    dock = new QDockWidget(tr("Control"), this);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setFixedSize(300, 1500);
    dock->setFloating(true);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);

    dockWidget = new QWidget();
    dockMainVLayout = new QVBoxLayout;

    // set to singleview at the beginning
//    loadSingleView();

    // set to multiview at the beginning
    loadMultiView();

    // Add menu to menu bar
    fileMenu = menuBar()->addMenu(tr("&File"));

    // Add MultiView to centralWidget layout
    loadMultiViewAct = new QAction(tr("&Load Processed Temporal Data"), this);
    connect(loadMultiViewAct, SIGNAL(triggered()), this, SLOT(loadMultiView()));
    fileMenu->addAction(loadMultiViewAct);

    // Add SigleView to centralWidget layout
    loadSigleViewAct = new QAction(tr("&Extract Data From Video File"), this);
    connect(loadSigleViewAct, SIGNAL(triggered()), this, SLOT(loadSingleView()));
    fileMenu->addAction(loadSigleViewAct);

}

MainWindow::~MainWindow(){
    delete loadSigleViewAct;
    delete loadMultiViewAct;
    delete fileMenu;
    delete ui;
}

void MainWindow::loadMultiView()
{
    this->showMaximized();
    if(!centralLayout->isEmpty()){
        delete centralLayout->itemAt(0)->widget();
    }

    multiview = new MultiView(this->centralWidget());
    centralLayout->addWidget(multiview);

    createvisSizeSlider();
    createTimeSlider();
    createProptyCheckbox();
    createParaCheckbox();
    dockWidget->setLayout(dockMainVLayout);

    dock->setWidget(dockWidget);
    dock->showMaximized();


    connect(para0, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
    connect(para1, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
    connect(para2, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
    connect(para3, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
    createFileCheckBoxes();

}

void MainWindow::loadSingleView()
{
    this->setFixedSize(1280, 1300);

    dock->hide();

    if(!centralLayout->isEmpty()){
        delete centralLayout->itemAt(0)->widget();
    }

    centralWidget()->resize(1280, 1280);
    SingleView *singleview = new SingleView(centralWidget());

    centralLayout->addWidget(singleview);

}

void MainWindow::propCheckBox_checked(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;
    if (state == Qt::Checked){
        if(checkBox->text() == "Area"){
            prop0->setChecked(true);
            prop1->setChecked(false);
            prop2->setChecked(false);
            prop3->setChecked(false);
            prop4->setChecked(false);
            prop5->setChecked(false);
            multiview->clearProps();
            multiview->pushProps(0);
        }
        else if(checkBox->text() == "Perimeter"){
            prop0->setChecked(false);
            prop1->setChecked(true);
            prop2->setChecked(false);
            prop3->setChecked(false);
            prop4->setChecked(false);
            prop5->setChecked(false);
            multiview->clearProps();
            multiview->pushProps(1);
        }
        else if(checkBox->text() == "Blebs Number and Size"){
            prop0->setChecked(false);
            prop1->setChecked(false);
            prop2->setChecked(true);
            prop3->setChecked(false);
            prop4->setChecked(false);
            prop5->setChecked(false);
            multiview->clearProps();
            multiview->pushProps(2);
        }
        else if(checkBox->text() == "Centroid Trajectory"){
            prop0->setChecked(false);
            prop1->setChecked(false);
            prop2->setChecked(false);
            prop3->setChecked(true);
            prop4->setChecked(false);
            prop5->setChecked(false);
            multiview->clearProps();
            multiview->pushProps(3);
        }
        else if(checkBox->text() == "Shape"){
            prop0->setChecked(false);
            prop1->setChecked(false);
            prop2->setChecked(false);
            prop3->setChecked(false);
            prop4->setChecked(true);
            prop5->setChecked(false);
            multiview->clearProps();
            multiview->pushProps(4);
        }else if (checkBox->text() == "Show All Properties"){
            prop0->setChecked(false);
            prop1->setChecked(false);
            prop2->setChecked(false);
            prop3->setChecked(false);
            prop4->setChecked(false);
            prop5->setChecked(true);
            multiview->clearProps();
            multiview->pushProps(0);
            multiview->pushProps(1);
            multiview->pushProps(2);
            multiview->pushProps(3);
            multiview->pushProps(4);
        }
    }
    //    else if (state == Qt::Unchecked) {
    //        if(checkBox->text() == "Area"){
    //            multiview->popProps(0);
    //        }
    //        else if(checkBox->text() == "Perimeter"){
    //            multiview->popProps(1);
    //        }
    //        else if(checkBox->text() == "Blebs Number and Size"){
    //            multiview->popProps(2);
    //        }
    //        else if(checkBox->text() == "Centroid Trajectory"){
    //            multiview->popProps(3);
    //        }
    //        else if(checkBox->text() == "Shape"){
    //            multiview->popProps(4);
    //        }
    //    }
}

void MainWindow::paraCheckBox_checked(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;
    if (state == Qt::Checked)    {
        if (checkBox->text() == "Probe (µN)"){
            para0->setChecked(true);
            para1->setChecked(false);
            para2->setChecked(false);
            para3->setChecked(false);
            multiview->clearCanvas();
            multiview->sortbyParameter(0);
            delete fileGroup;
            createFileCheckBoxes();
        }
        else if (checkBox->text() == "Force Ascending (µN)"){
            para0->setChecked(false);
            para1->setChecked(true);
            para2->setChecked(false);
            para3->setChecked(false);
            multiview->clearCanvas();
            multiview->sortbyParameter(1);
            delete fileGroup;
            createFileCheckBoxes();
        }
        else if (checkBox->text() == "Temperature"){
            para0->setChecked(false);
            para1->setChecked(false);
            para2->setChecked(true);
            para3->setChecked(false);
            multiview->clearCanvas();
            multiview->sortbyParameter(2);
            delete fileGroup;
            createFileCheckBoxes();
        }
        else if (checkBox->text() == "Date Ascending"){
            para0->setChecked(false);
            para1->setChecked(false);
            para2->setChecked(false);
            para3->setChecked(true);
            multiview->clearCanvas();
            multiview->sortbyParameter(3);
            delete fileGroup;
            createFileCheckBoxes();
        }
    }

}

void MainWindow::visSideLenSlider_valueChanged(int v)
{
    visSideLenLabel->setText("View Size: "+QString::number(int(260 + v*10)));
    multiview->setVisSideLen(int(260 + v*10));
}

void MainWindow::createvisSizeSlider()
{
    visSideLenLayout = new QVBoxLayout;

    visSideLenLabel = new QLabel("View Size: 300");
    visSideLenLabel->setStyleSheet(CHECKBOX);
    visSideLenLabel->setFixedWidth(300);
    visSideLenSlider = new QSlider(Qt::Horizontal);
    visSideLenSlider->setRange(0, 10);
    visSideLenSlider->setValue(6);
    visSideLenLayout->addWidget(visSideLenLabel);
    visSideLenLayout->addWidget(visSideLenSlider);

    dockMainVLayout->addLayout(visSideLenLayout);

    connect(visSideLenSlider, SIGNAL(valueChanged(int)), this, SLOT(visSideLenSlider_valueChanged(int)));
}

void MainWindow::createTimeSlider()
{
    //    dockMainVLayout->addStretch();

    timeGroup = new QGroupBox(" Time Range Selection: ");
    timeGroup->setStyleSheet(GROUPBOX);

    timeRangeLayout = new QVBoxLayout;
    QLabel   *tmp = new QLabel("");
    timeRangeLayout->addWidget(tmp);

    timeSttLayout = new QVBoxLayout;
    timeSttLabel = new QLabel("Frame Start: 0");
    timeSttLabel->setFixedWidth(250);
    timeSttLabel->setStyleSheet(CHECKBOX);
    timeSttSlider = new QSlider(Qt::Horizontal);
    timeSttSlider->setStyleSheet(TRANS_BKGRD);
    timeSttSlider->setRange(0, 2500-1);
    timeSttSlider->setValue(0);
    timeSttLayout->addWidget(timeSttLabel);
    timeSttLayout->addWidget(timeSttSlider);

    timeEndLayout = new QVBoxLayout;
    timeEndLabel = new QLabel("Frame End: 2500");
    timeEndLabel->setFixedWidth(250);
    timeEndLabel->setStyleSheet(CHECKBOX);
    timeEndSlider = new QSlider(Qt::Horizontal);
    timeEndSlider->setStyleSheet(TRANS_BKGRD);
    timeEndSlider->setRange(1, 2500);
    timeEndSlider->setValue(2500);
    timeEndLayout->addWidget(timeEndLabel);
    timeEndLayout->addWidget(timeEndSlider);

    timeRangeLayout->addLayout(timeSttLayout);
    timeRangeLayout->addLayout(timeEndLayout);

    timeGroup->setLayout(timeRangeLayout);
    timeGroup->setFixedHeight(130);

    //dockMainVLayout->addLayout(timeRangeLayout);
    dockMainVLayout->addWidget(timeGroup);
    //    dockMainVLayout->addStretch();

    connect(timeSttSlider, SIGNAL(valueChanged(int)), this, SLOT(timeSrtSlider_valueChanged(int)));
    connect(timeEndSlider, SIGNAL(valueChanged(int)), this, SLOT(timeEndSlider_valueChanged(int)));
}

void MainWindow::timeEndSlider_valueChanged(int v)
{
    timeEndLabel->setText("Frame End: "+QString::number(int(v)));
    if(timeSttSlider->value() > timeEndSlider->value()){
        timeSttSlider->setValue(timeEndSlider->value());
        int sttV = timeEndSlider->value() < 1 ? 1 : timeEndSlider->value() - 1;
        timeSttLabel->setText("Frame Start: "+QString::number(int(sttV)));
    }
    multiview->setTimeEnd(v);
}

void MainWindow::timeSrtSlider_valueChanged(int v)
{
    timeSttLabel->setText("Frame Start: "+QString::number(int(v)));
    if(timeEndSlider->value() < timeSttSlider->value()){
        timeEndSlider->setValue(timeSttSlider->value());
        int endV = timeSttSlider->value() > 2500 ? 2500 : timeSttSlider->value() + 1;
        timeEndLabel->setText("Frame End: "+QString::number(int(endV)));
    }
    multiview->setTimeStt(v);
}

void MainWindow::fileCheckBox_checked(int state)
{
    QObject* obj = sender();
    int idx = obj->objectName().remove("fileName").toInt();

    multiview->updateIndexAndValue(idx, state);

}

void MainWindow::createProptyCheckbox()
{

    propGroup = new QGroupBox(" Property Selection: ");
    propGroup->setStyleSheet(GROUPBOX);
    prop0 = new QCheckBox("Area"); // 0
    prop1 = new QCheckBox("Perimeter"); // 1
    prop2 = new QCheckBox("Blebs Number and Size"); // 2
    prop3 = new QCheckBox("Centroid Trajectory"); // 3
    prop4 = new QCheckBox("Shape"); // 4
    prop5 = new QCheckBox("Show All Properties"); // 5
    prop0->setStyleSheet(CHECKBOX);
    prop1->setStyleSheet(CHECKBOX);
    prop2->setStyleSheet(CHECKBOX);
    prop3->setStyleSheet(CHECKBOX);
    prop4->setStyleSheet(CHECKBOX);
    prop5->setStyleSheet(CHECKBOX);

    prop0->setChecked(false);
    prop1->setChecked(false);
    prop2->setChecked(false);
    prop3->setChecked(true);
    prop4->setChecked(false);
    prop5->setChecked(false);

    propVLayout = new QVBoxLayout();
    propVLayout->addWidget(prop0);
    propVLayout->addWidget(prop1);
    propVLayout->addWidget(prop2);
    propVLayout->addWidget(prop3);
    propVLayout->addWidget(prop4);
    propVLayout->addWidget(prop5);

    propGroup->setLayout(propVLayout);
    propGroup->setFixedHeight(270);

    dockMainVLayout->addWidget(propGroup);
    //    dockMainVLayout->addStretch();

    connect(prop0, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop1, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop2, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop3, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop4, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop5, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));

}

void MainWindow::createParaCheckbox()
{
    paraGroup = new QGroupBox(" Sort: ");
    paraGroup->setStyleSheet(GROUPBOX);
    para0   = new QCheckBox("Probe Size (µN)");
    para1   = new QCheckBox("Force Ascending (µN)");
    para2   = new QCheckBox("Temperature");
    para3   = new QCheckBox("Date Ascending");
    para0->setStyleSheet(CHECKBOX);
    para1->setStyleSheet(CHECKBOX);
    para2->setStyleSheet(CHECKBOX);
    para3->setStyleSheet(CHECKBOX);

    para0->setChecked(false);
    para1->setChecked(false);
    para2->setChecked(false);
    para3->setChecked(true);

    paraVLayout = new QVBoxLayout();
    paraVLayout->addWidget(para0);
    paraVLayout->addWidget(para1);
    paraVLayout->addWidget(para2);
    paraVLayout->addWidget(para3);

    paraGroup->setLayout(paraVLayout);
    paraGroup->setFixedHeight(150);

    dockMainVLayout->addWidget(paraGroup);

}

void MainWindow::createFileCheckBoxes()
{
    fileGroup = new QGroupBox(" File List: ");
    fileGroup->setStyleSheet(GROUPBOX);

    QVector<QFileInfo> fileInfos = multiview->getFilenames();
    QVector<unsigned int> indices = multiview->getUpdatedIndex();

    fileVLayout = new QVBoxLayout();

    QScrollArea *area = new QScrollArea();
    area->setWidgetResizable(true);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *tmpLayout = new QVBoxLayout();
    QWidget *tmpWidget = new QWidget();



    for(int i = 0; i < fileInfos.size(); i++){
        int idx = indices[i];
        QString filename = fileInfos[idx].baseName();

        QCheckBox *checkbox = new QCheckBox(filename);
        checkbox->setChecked(true);
        checkbox->setObjectName("fileName"+QString::number(i));
        //checkbox->isChecked();
        connect(checkbox, SIGNAL(stateChanged(int)), this, SLOT(fileCheckBox_checked(int)));
        fileVLayout->addWidget(checkbox);
    }

    for (int i = 0; i < fileVLayout->count(); i++)
    {
        if (QWidgetItem *myItem = dynamic_cast <QWidgetItem*>(fileVLayout->itemAt(i))) {
            QWidget *wid = myItem->widget();
            //qDebug() << wid->objectName();
            fileCheckBoxes << myItem->widget();
        }
    }

    //qDebug() << fileCheckBoxes.size();

    /*for (int i = 0; i < fileCheckBoxes.count(); i++)
    {
        QWidget *wid = qobject_cast <QWidget*>(fileCheckBoxes.at(i));
        if (wid)
            qDebug() << wid->objectName();
    }*/


    tmpWidget->setLayout(fileVLayout);
    area->setWidget(tmpWidget);
    area->setStyleSheet(SCROLLBAR_TN);
    tmpLayout->addWidget(area);

    fileGroup->setLayout(tmpLayout);
    dockMainVLayout->addWidget(fileGroup);

    //    qDebug() << fileCheckBoxes;
}




















