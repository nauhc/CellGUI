#include "mainwindow.h"
#include <QWidget>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
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

    dock = new QDockWidget(tr("Basic operations"), this);
    this->addDockWidget(Qt::RightDockWidgetArea, dock);
    dock->setFixedSize(300, 800);
    dock->setFloating(true);
    dockWidget = new QWidget();

    createProptyCheckbox();

    // set to singleview at the beginning
//    loadSigleView();

    // set to cultiview at the beginning
    loadMultiView();

    // Add menu to menu bar
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMode = false;

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

    fileMode = true;

    if(!centralLayout->isEmpty()){
        delete centralLayout->itemAt(0)->widget();
    }

    MultiView *multiview = new MultiView(this->centralWidget());
    centralLayout->addWidget(multiview);

    dock->setWidget(dockWidget);
    dock->showMaximized();

}

void MainWindow::loadSigleView()
{
    this->setFixedSize(1280, 1300);

    dock->hide();

    fileMode = false;
    if(!centralLayout->isEmpty()){
        delete centralLayout->itemAt(0)->widget();
    }

    SingleView *singleview = new SingleView(this->centralWidget());
    centralLayout->addWidget(singleview);

}

void MainWindow::box_checked(int state)
{    QCheckBox *checkBox = qobject_cast<QCheckBox*>(sender());
     if (!checkBox) return;

     if (state == Qt::Checked) {
         if(checkedBoxes.size() == 4){
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
}

void MainWindow::createProptyCheckbox()
{

    propGroup = new QGroupBox("Property Selection: ");
    prop0 = new QCheckBox("Area");
    prop1 = new QCheckBox("Perimeter");
    prop2 = new QCheckBox("Blebs Number & Size");
    prop3 = new QCheckBox("Centroid Trajectory");
    prop4 = new QCheckBox("Shape");

    prop0->setChecked(true);
    prop1->setChecked(false);
    prop2->setChecked(true);
    prop3->setChecked(true);
    prop4->setChecked(true);

    propVLayout = new QVBoxLayout();
    propVLayout->addWidget(prop0);
    propVLayout->addWidget(prop1);
    propVLayout->addWidget(prop2);
    propVLayout->addWidget(prop3);
    propVLayout->addWidget(prop4);

    propGroup->setLayout(propVLayout);
//    propGroup->setFixedHeight(100);

    mainVLayout = new QVBoxLayout;
    mainVLayout->addWidget(propGroup);
//    QSpacerItem *spacer = new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::Maximum);
    mainVLayout->addStretch();
//    spacer-
//    mainVLayout->addItem();
    setLayout(mainVLayout);

    connect(prop0, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop1, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop2, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop3, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));
    connect(prop4, SIGNAL(stateChanged(int)), this, SLOT(box_checked(int)));

}

