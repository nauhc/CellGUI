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
    dock = new QDockWidget(tr("Basic operations"), this);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setFixedSize(300, 800);
    dock->setFloating(true);
    dockWidget = new QWidget();
    dockMainVLayout = new QVBoxLayout;
    createTimeSlider();
    createProptyCheckbox();
    createParaCheckbox();
    dockWidget->setLayout(dockMainVLayout);

    // set to singleview at the beginning
    //    loadSigleView();

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
    connect(loadSigleViewAct, SIGNAL(triggered()), this, SLOT(loadSigleView()));
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

    dock->setWidget(dockWidget);
    dock->showMaximized();

    multiview = new MultiView(this->centralWidget());

    centralLayout->addWidget(multiview);

    connect(para0, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
    connect(para1, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
    connect(para2, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
    connect(para3, SIGNAL(stateChanged(int)), this, SLOT(paraCheckBox_checked(int)));
}

void MainWindow::loadSigleView()
{
    this->setFixedSize(1280, 1300);

    dock->hide();

    if(!centralLayout->isEmpty()){
        delete centralLayout->itemAt(0)->widget();
    }

    SingleView *singleview = new SingleView(this->centralWidget());
    centralLayout->addWidget(singleview);

}

void MainWindow::propCheckBox_checked(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;

    //     if (state == Qt::Checked) {
    //         if(checkedBoxes.size() == 4){
    //             checkBox->setCheckState(Qt::Unchecked);
    //             return;
    //         } else if (!checkedBoxes.contains(checkBox->objectName().remove(0, 9))) {
    //             checkedBoxes.push_back(checkBox->objectName().remove(0, 9));
    //         }

    //     } else if (state == Qt::Unchecked) {
    //         if (checkedBoxes.size() == 0) {
    //             return; // should not happen
    //         } else if (checkedBoxes.contains(checkBox->objectName().remove(0, 9))) {
    //             checkedBoxes.remove(checkedBoxes.indexOf(checkBox->objectName().remove(0, 9)));
    //         }
    //     }

    if (state == Qt::Checked){
        if(checkBox->text() == "Area"){
            multiview->pushProps(0);
        }
        else if(checkBox->text() == "Perimeter"){
            multiview->pushProps(1);
        }
        else if(checkBox->text() == "Blebs Number and Size"){
            multiview->pushProps(2);
        }
        else if(checkBox->text() == "Centroid Trajectory"){
            multiview->pushProps(3);
        }
        else if(checkBox->text() == "Date"){
            multiview->pushProps(4);
        }
    }
    else if (state == Qt::Unchecked) {
        if(checkBox->text() == "Area"){
            multiview->popProps(0);
        }
        else if(checkBox->text() == "Perimeter"){
            multiview->popProps(1);
        }
        else if(checkBox->text() == "Blebs Number and Size"){
            multiview->popProps(2);
        }
        else if(checkBox->text() == "Centroid Trajectory"){
            multiview->popProps(3);
        }
        else if(checkBox->text() == "Date"){
            multiview->popProps(4);
        }
    }
}

void MainWindow::paraCheckBox_checked(int state)
{
    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
    if (!checkBox) return;
    if (state == Qt::Checked)    {
        if (checkBox->text() == "Pressure"){
            para0->setChecked(true);
            para1->setChecked(false);
            para2->setChecked(false);
            para3->setChecked(false);
            multiview->sortbyParameter(0);
        }
        else if (checkBox->text() == "Force"){
            para0->setChecked(false);
            para1->setChecked(true);
            para2->setChecked(false);
            para3->setChecked(false);
            multiview->sortbyParameter(1);
        }
        else if (checkBox->text() == "Force Offset"){
            para0->setChecked(false);
            para1->setChecked(false);
            para2->setChecked(true);
            para3->setChecked(false);
            multiview->sortbyParameter(2);
        }
        else if (checkBox->text() == "Date"){
            para0->setChecked(false);
            para1->setChecked(false);
            para2->setChecked(false);
            para3->setChecked(true);
            multiview->sortbyParameter(3);
        }
    }

}

void MainWindow::createTimeSlider()
{
    timeRangeLayout = new QVBoxLayout;
    timeStartLayout = new QHBoxLayout;
    timeEndLayout = new QHBoxLayout;
    timeStartLabel = new QLabel("Frame range start ");
    timeEndLabel = new QLabel("Frame range end ");
    timeStartSlider = new QSlider(Qt::Horizontal);
    timeEndSlider = new QSlider(Qt::Horizontal);
    timeStartLayout->addWidget(timeStartLabel);
    timeStartLayout->addWidget(timeStartSlider);
    timeEndLayout->addWidget(timeEndLabel);
    timeEndLayout->addWidget(timeEndSlider);
    timeRangeLayout->addLayout(timeStartLayout);
    timeRangeLayout->addLayout(timeEndLayout);

    dockMainVLayout->addLayout(timeRangeLayout);
    dockMainVLayout->addStretch();

}

void MainWindow::createProptyCheckbox()
{

    propGroup = new QGroupBox(" Property Selection: ");
    propGroup->setStyleSheet(GROUPBOX);
    prop0 = new QCheckBox("Area");
    prop1 = new QCheckBox("Perimeter");
    prop2 = new QCheckBox("Blebs Number and Size");
    prop3 = new QCheckBox("Centroid Trajectory");
    prop4 = new QCheckBox("Shape");
    prop0->setStyleSheet(CHECKBOX);
    prop1->setStyleSheet(CHECKBOX);
    prop2->setStyleSheet(CHECKBOX);
    prop3->setStyleSheet(CHECKBOX);
    prop4->setStyleSheet(CHECKBOX);

    prop0->setChecked(false);
    prop1->setChecked(false);
    prop2->setChecked(false);
    prop3->setChecked(false);
    prop4->setChecked(true);

    propVLayout = new QVBoxLayout();
    propVLayout->addWidget(prop0);
    propVLayout->addWidget(prop1);
    propVLayout->addWidget(prop2);
    propVLayout->addWidget(prop3);
    propVLayout->addWidget(prop4);

    propGroup->setLayout(propVLayout);
    propGroup->setFixedHeight(250);

    dockMainVLayout->addWidget(propGroup);
    dockMainVLayout->addStretch();

    connect(prop0, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop1, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop2, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop3, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));
    connect(prop4, SIGNAL(stateChanged(int)), this, SLOT(propCheckBox_checked(int)));

}

void MainWindow::createParaCheckbox()
{
    paraGroup = new QGroupBox(" Sort on: ");
    paraGroup->setStyleSheet(GROUPBOX);
    para0   = new QCheckBox("Pressure");
    para1   = new QCheckBox("Force");
    para2   = new QCheckBox("Force Offset");
    para3   = new QCheckBox("Date");
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
    dockMainVLayout->addStretch();

}

