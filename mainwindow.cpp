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
//    this->setBaseSize(this->width(), this->height());

    this->setWindowTitle(" Dancing Cell Visualization ");

    centralLayout = new QHBoxLayout();
    this->centralWidget()->setLayout(centralLayout);
    //this->centralWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//    dock = new QDockWidget(tr("Basic operations"), this);
//    this->addDockWidget(Qt::RightDockWidgetArea, dock);
//    //dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);
//    dock->setFixedSize(300, 800);
//    //dock->showMaximized();


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

//    dockMenu = new DockMenu();
//    //multiview->createDockMenu(dock);
//    dock->setWidget(dockMenu);
//    dockMenu->show();
//    dock->showMaximized();
////    dock->show();

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

