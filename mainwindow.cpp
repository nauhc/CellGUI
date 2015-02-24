#include "mainwindow.h"
#include <QWidget>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "multiview.h"
#include "singleview.h"

//#include "style.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
    //,dataFilename(new QString(""))
{
    ui->setupUi(this);
    this->setStyleSheet("background-color:rgb(251,251,251)");
    this->setBaseSize(this->width(), this->height());
    this->setWindowTitle(" Dancing Cell Visualization ");

    centralLayout = new QHBoxLayout();
    this->centralWidget()->setLayout(centralLayout);
    //this->centralWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//    // set to singleview at the beginning
//    SingleView *singleview = new SingleView(this->centralWidget());
//    centralLayout->addWidget(singleview);

    MultiView *multiview = new MultiView(this->centralWidget());
    centralLayout->addWidget(multiview);


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
    //delete dataFilename;
    delete loadMultiViewAct;
    delete fileMenu;
    delete ui;
}

void MainWindow::loadMultiView()
{
    fileMode = true;

    if(!centralLayout->isEmpty()){
        delete centralLayout->itemAt(0)->widget();
    }

    MultiView *multiview = new MultiView(this->centralWidget());
    centralLayout->addWidget(multiview);
    //multiview->show();


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

void MainWindow::loadSigleView()
{
    fileMode = false;
    if(!centralLayout->isEmpty()){
        delete centralLayout->itemAt(0)->widget();
    }

    SingleView *singleview = new SingleView(this->centralWidget());
    centralLayout->addWidget(singleview);

}

