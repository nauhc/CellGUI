#include "multiview.h"
#include <QDebug>
#include <iostream>

MultiView::MultiView(QWidget *parent) : QWidget(parent), dataFilename(new QString("")){

    setStyleSheet("background-color:rgb(251,251,251)");

    mainVLayout = new QVBoxLayout(this);
    visGLayout = new QGridLayout();


    createNarVis();
    createCodVis();
    createShpVis();


    QLabel *label3 = new QLabel("333333");
    QLabel *label6 = new QLabel("666666");
    QLabel *label9 = new QLabel("999999");
    visGLayout->addWidget(nar_container1, 0, 0);
    visGLayout->addWidget(nar_container2, 0, 1);
    visGLayout->addWidget(label3, 0, 2);
    visGLayout->addWidget(cod_container1, 1, 0);
    visGLayout->addWidget(cod_container2, 1, 1);
    visGLayout->addWidget(label6, 1, 2);
    visGLayout->addWidget(shp_container1, 2, 0);
    visGLayout->addWidget(shp_container2, 2, 1);
    visGLayout->addWidget(label9, 2, 2);

    createLoadFilesButton();

    mainVLayout->addWidget(loadFilesButton);
    mainVLayout->addLayout(visGLayout);

}


void MultiView::createNarVis()
{
    nar_tmp1 = new Narr();
    nar_container1 = QWidget::createWindowContainer(nar_tmp1, this);
    nar_container1->setMinimumSize(256, 256);
    nar_container1->setMaximumSize(512, 512);
    nar_tmp2 = new Narr();
    nar_container2 = QWidget::createWindowContainer(nar_tmp2, this);
    nar_container2->setMinimumSize(256, 256);
    nar_container2->setMaximumSize(512, 512);
}

void MultiView::createCodVis()
{
    cod_tmp1 = new Coord();
    cod_container1 = QWidget::createWindowContainer(cod_tmp1, this);
    cod_container1->setMinimumSize(256, 256);
    cod_container1->setMaximumSize(512, 512);
    cod_tmp2 = new Coord();
    cod_container2 = QWidget::createWindowContainer(cod_tmp2, this);
    cod_container2->setMinimumSize(256, 256);
    cod_container2->setMaximumSize(512, 512);

}

void MultiView::createShpVis()
{
    shp_tmp1 = new Shape();
    shp_container1 = QWidget::createWindowContainer(shp_tmp1, this);
    shp_container1->setMinimumSize(256, 256);
    shp_container1->setMaximumSize(512, 512);

    shp_tmp2 = new Shape();
    shp_container2 = QWidget::createWindowContainer(shp_tmp2, this);
    shp_container2->setMinimumSize(256, 256);
    shp_container2->setMaximumSize(512, 512);

}

void MultiView::createSpacers()
{

}

void MultiView::clearAll()
{
    nar_tmp1->clear();
    nar_tmp2->clear();
    cod_tmp1->clear();
    cod_tmp2->clear();
    cellData.clear();
    delete dataFilename;
    dataFilename =  new QString("");
}

void MultiView::loadFilesButton_pressed()
{
    loadFilesButton->setStyleSheet(BUTTON_PRESSED);
}

void MultiView::loadFilesButton_released()
{
    loadFilesButton->setStyleSheet(BUTTON_RELEASED_ON);
}

void MultiView::loadFilesButton_clicked()
{
    std::cout << "'Load Cell Data' menu selected." << std::endl;
    clearAll();

    //*** read-property-from-file mode (fileMode = true) //
    connect(this, SIGNAL(readProperties(floatArray)),
            this, SLOT(updatePropsVisUI(floatArray)));
    connect(this, SIGNAL(readCellImg(QImage)),
            this, SLOT(updateCellImg(QImage)));
    // read-property-from-file mode (fileMode = true) *** //

    connect(this, SIGNAL(readContourNBlebs(QVector<Bleb>,QVector<QPoint>,QPoint)),
            this, SLOT(updateContourNBlebs(QVector<Bleb>,QVector<QPoint>,QPoint)));

    QFileDialog *dialog = new QFileDialog();
    *dataFilename = dialog->getOpenFileName(this,
                                            tr("Open Video"),
                                            "../../../video", /*QDir::homePath()+"/Desktop/",*/
                                            tr("Data Files (*.csv)"));
    delete dialog;

    if(!dataFilename->isEmpty()){
        //prepare writing data to file
        QFileInfo   fi  = QFileInfo(*dataFilename);
        //QString     ff  = fi.path()+"/"+fi.baseName();
        //string      fn  = ff.toUtf8().constData();

        //this->setWindowTitle(" Dancing Cell Visualization: "+fi.fileName());

        if(readDataFile()){
            unsigned int cellDataSize = cellData.size();
            if(cellDataSize > 20){
                nar_tmp1->setBeginFrame(cellData[0][0]);
                nar_tmp1->setMaxFrm(cellData[cellDataSize-2][0]);
                cod_tmp1->getMaxFrm(cellData[cellDataSize-2][0]);
                cod_tmp1->getMaxSize(QSize(640, 480));
                for(unsigned int n = 0; n < cellDataSize; n++){
                    // data
                    emit readProperties(cellData[n]);
                    // img
                    QImage img = readImgFile(fi.path(), cellData[n][0]/*index*/);
                    emit readCellImg(img);
                }
            }
            if(cellDataSize <= 20){
                qDebug() << "Cell Data Size ERROR!";
            }
            // bleb size
            if(readBlebsFile() && readContoursFile()){
                if((contours.size() == blebs.size()) && (blebs.size() > 20)){
                    shp_tmp1->setBeginFrm(cellData[0][0]);
                    shp_tmp1->setMaxFrm(cellData[cellDataSize-2][0]);
                    for(unsigned int n = 0; n < blebs.size(); n++){
                        emit readContourNBlebs(blebs[n], contours[n], centers[n]);
                    }
                }
                else qDebug() << "Bleb and Contour Data Size EROOR!" << contours.size() << " " << blebs.size();
            }
            else qDebug() << "Read Bleb and Contour Data files EROOR!";

        }
     }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }

}

void MultiView::updatePropsVisUI(floatArray property)
{
    nar_tmp1->updateProperty(property, property[0]);
    cod_tmp1->updateCoord(QPointF(property[3], property[4]), property[0]);
//    shp_tmp1->updateContourNBleb();
}

void MultiView::updateCellImg(QImage cell)
{
    nar_tmp1->updateCellImg(cell);
}

void MultiView::updateCellImg(QImage cell, QVector<QPoint> smoothContour)
{
    nar_tmp1->updateCellImg(cell, smoothContour);
}

void MultiView::updateContourNBlebs(QVector<Bleb> blebs, QVector<QPoint> contour, QPoint centroid) //QVector<Bleb>, QVector<QPoint>
{
    shp_tmp1->updateContourNBleb(blebs, contour, centroid);
}

void MultiView::createLoadFilesButton()
{
    loadFilesButton = new QPushButton("Load files");
    loadFilesButton->setStyleSheet(BUTTON_RELEASED_ON);
    connect(this->loadFilesButton, SIGNAL(clicked()), this, SLOT(loadFilesButton_clicked()));

}

bool MultiView::readDataFile()
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
        //readBlebsFile();
        //readContoursFile();
        return true;
    }

}

bool MultiView::readBlebsFile()
{
    QString tmp = *dataFilename;
    QString fn_b = tmp.remove(tmp.length()-15, 15) + "_b_compressed.dat" ;

    QFile f(fn_b);
    if(!f.open(QIODevice::ReadOnly)){
        qDebug() << "Reading bleb's dat file not found.";
        return false;
    }
    else{
        QDataStream in(&f);
        while(!in.atEnd()){
            qint32  blebsNum;
            qreal   centX, centY;
            in >> centX;
            in >> centY;
            in >> blebsNum;
            QVector<Bleb> blebs_1frm;
            for(int i = 0; i < blebsNum; i++){
                qint32 blebSize;
                in >> blebSize;
                Bleb bleb;
                bleb.size = blebSize;
                for(int j = 0; j < blebSize; j++){
                    polarPoint p;
                    in >> p.r;
                    in >> p.theta;
                    bleb.bunch_polar.push_back(p);
                }
                blebs_1frm.push_back(bleb);
            }
            blebs.push_back(blebs_1frm);
            centers.push_back(QPoint(centX, centY));
        }
    }

//    std::cout << "blebs sizes load to file " << blebs.size() << std::endl;
//    for(unsigned int n = 0; n < blebs.size(); n++){
//        std::cout << "frame  " << n << std::endl;
//        std::cout << "centroid  (" << centers[n].x() << ", " << centers[n].y() << ")" << std::endl;
////        for(unsigned int m = 0; m < blebs[n].size(); m++){
////            std::cout << "bleb  " << m << std::endl;
////            for(unsigned int k = 0; k < blebs[n][m].bunch_polar.size(); k++){
////                std::cout << "(r " << blebs[n][m].bunch_polar[k].r
////                          << " , theta " << blebs[n][m].bunch_polar[k].theta << ") ";
////            }
////            std::cout << std::endl;
////        }
////        std::cout << std::endl;
//    }
    return true;
}

bool MultiView::readContoursFile()
{
    QString tmp = *dataFilename;
    QString fn_c = tmp.remove(tmp.length()-15, 15) + "_c_compressed.dat" ;
    //qDebug() << fn_c;

    QFile ff(fn_c);
    if(!ff.open(QIODevice::ReadOnly)){
        qDebug() << "Reading contours' dat file not found.";
        return false;
    }
    else{
        QDataStream in(&ff);
        while(!in.atEnd()){
            qint32 contourSize;
            in >> contourSize;
            QVector<QPoint> contour;
            for(int i = 0; i < contourSize; i++){
                int x,y;
                in >> x;
                in >> y;
                contour.push_back(QPoint(x, y));
            }
            contours.push_back(contour);
        }
    }

//    std::cout << "contours sizes load to file " << contours.size() << std::endl;
//    for(unsigned int n = 0; n < contours.size(); n++){
//        std::cout << "frame  " << n << std::endl;
//        for(unsigned int m = 0; m < contours[n].size(); m++){
//            std::cout << "(x "  << contours[n][m].x() << ", y " << contours[n][m].y() << ") ";
//        }
//        std::cout << std::endl;
//    }

    return true;
}

QImage MultiView::readImgFile(QString fp, unsigned int idx) // filepath, index
{
    QImage img;
    QString str = fp+"/cell"+QString::number(int(idx))+".png";
    //qDebug() << str;
    img.load(str);
    return img;
}

