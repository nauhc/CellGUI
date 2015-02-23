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


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    dataFilename(new QString(""))
{
    fileMode = false;

    ui->setupUi(this);
    this->setStyleSheet(/*"background-color:rgb(38,42,43)"*/"background-color:rgb(251,251,251)");
    //this->setFixedSize(this->width(), this->height());
    this->setBaseSize(this->width(), this->height());
    this->setWindowTitle(" Dancing Cell Visualization ");

    //QWidget *tempWidget = new QWidget(this);
    //tempWidget->setGeometry(this->width()/2+120, 0, this->width()/2-120, this->height());
    //SingleView *singleview = new SingleView(tempWidget);

    QHBoxLayout *centralLayout = new QHBoxLayout();
    this->centralWidget()->setLayout(centralLayout);
    this->centralWidget()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    SingleView *singleview = new SingleView(this->centralWidget());
    centralLayout->addWidget(singleview);
    //singleview->show();


    // connect properties and dataVis
    qRegisterMetaType<floatArray>("floatArray");
    qRegisterMetaType<QVector<QPoint> >("QVector<QPoint>");

    loadCellDataAct = new QAction(tr("&Load Processed Temporal Data"), this);
    connect(loadCellDataAct, SIGNAL(triggered()), this, SLOT(loadCellData()));
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadCellDataAct);
}

MainWindow::~MainWindow(){
    delete dataFilename;
    delete loadCellDataAct;
    delete fileMenu;
    delete ui;
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
