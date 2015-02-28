#include "multiview.h"
#include <QDebug>
#include <iostream>
#include <QDirIterator>

MultiView::MultiView(QWidget *parent) : QWidget(parent)/*, dataFilename1(new QString(""))*/{

    setStyleSheet("background-color:rgb(251,251,251)");

    mainVLayout = new QVBoxLayout(this);
    visGLayout = new QGridLayout();

    getFileNames(); // datafileNames prepared

//    createNarVis();
//    createCodVis();
//    createShpVis();

//    visGLayout->addWidget(nar_container1, 0, 0);
//    visGLayout->addWidget(nar_container2, 0, 1);
//    visGLayout->addWidget(nar_container3, 0, 2);
//    visGLayout->addWidget(cod_container1, 1, 0);
//    visGLayout->addWidget(cod_container2, 1, 1);
//    visGLayout->addWidget(cod_container3, 1, 2);
//    visGLayout->addWidget(shp_container1, 2, 0);
//    visGLayout->addWidget(shp_container2, 2, 1);
//    visGLayout->addWidget(shp_container3, 2, 2);

    createLoadFilesButton();

    mainVLayout->addWidget(loadFilesButton);
    mainVLayout->addLayout(visGLayout);

}

void MultiView::createNarVis()
{
//    for(int i = 0; i < datafileNames.size(); i++){
//        Narr *nar_tmp = new Narr();
//        QWidget *nar_container = QWidget::createWindowContainer(nar_tmp, this);
//        nar_container->setMinimumSize(256, 256);
//        nar_container->setMaximumSize(512, 512);

//        nar_list.append(*nar_tmp);
//        container_nar.append(*nar_container);
//    }


    nar_tmp1 = new Narr();
    nar_container1 = QWidget::createWindowContainer(nar_tmp1, this);
    nar_container1->setMinimumSize(256, 256);
    nar_container1->setMaximumSize(512, 512);

    nar_tmp2 = new Narr();
    nar_container2 = QWidget::createWindowContainer(nar_tmp2, this);
    nar_container2->setMinimumSize(256, 256);
    nar_container2->setMaximumSize(512, 512);

    nar_tmp3 = new Narr();
    nar_container3 = QWidget::createWindowContainer(nar_tmp3, this);
    nar_container3->setMinimumSize(256, 256);
    nar_container3->setMaximumSize(512, 512);
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

    cod_tmp3 = new Coord();
    cod_container3 = QWidget::createWindowContainer(cod_tmp3, this);
    cod_container3->setMinimumSize(256, 256);
    cod_container3->setMaximumSize(512, 512);

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

    shp_tmp3 = new Shape();
    shp_container3 = QWidget::createWindowContainer(shp_tmp3, this);
    shp_container3->setMinimumSize(256, 256);
    shp_container3->setMaximumSize(512, 512);
}

void MultiView::createSpacers()
{

}

void MultiView::clearVis()
{

//    nar_tmp1->clear();
//    nar_tmp2->clear();
//    cod_tmp1->clear();
//    cod_tmp2->clear();
}

void MultiView::clearData()
{
//    cellData1.clear();
//    blebs1.clear();
//    contours1.clear();
//    centers1.clear();
//    delete dataFilename1;
//    dataFilename1 =  new QString("");

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
    clearVis();
    clearData();

    readFiles();

    //*** read-property-from-file mode (fileMode = true) //
    connect(this, SIGNAL(readProperties(floatArray)),
            this, SLOT(updatePropsVisUI(floatArray)));

    connect(this, SIGNAL(readCellImg(QImage)),
            this, SLOT(updateCellImg(QImage)));

    connect(this, SIGNAL(readContourNBlebs(QVector<Bleb>,QVector<QPoint>,QPoint)),
            this, SLOT(updateContourNBlebs(QVector<Bleb>,QVector<QPoint>,QPoint)));
    // read-property-from-file mode (fileMode = true) *** //

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

void MultiView::getFileNames()
{
    QString folderPath = "../../../video/ExtractedData/";

    QDirIterator dirIt(folderPath, QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
            if (QFileInfo(dirIt.filePath()).suffix() == "csv")
                datafileNames.push_back(dirIt.filePath());
    }
}

void MultiView::createLoadFilesButton()
{
    loadFilesButton = new QPushButton("Load files");
    loadFilesButton->setStyleSheet(BUTTON_RELEASED_ON);
    loadFilesButton->setMaximumHeight(30);
    connect(this->loadFilesButton, SIGNAL(clicked()), this, SLOT(loadFilesButton_clicked()));

}

bool MultiView::readFiles()
{

    for(int i = 0; i < datafileNames.size(); i++){

        if(!datafileNames[i].isEmpty()){
            //QFileInfo   fi  = QFileInfo(datafileNames[i]);
            if(readDataFile(datafileNames[i])){
                unsigned int cellDataSize = cellData[i].size();
                if(cellDataSize > 20){
                    Narr *nar_tmp = new Narr();
                    QWidget *nar_container = QWidget::createWindowContainer(nar_tmp, this);
                    nar_container->setMinimumSize(256, 256);
                    nar_container->setMaximumSize(512, 512);
                    nar_tmp->setBeginFrame(cellData[i][0][0]);
                    nar_tmp->setMaxFrm(cellData[i][cellDataSize-2][0]);

                    Coord *cod_tmp = new Coord();
                    QWidget *cod_container = QWidget::createWindowContainer(cod_tmp, this);
                    cod_container->setMinimumSize(256, 256);
                    cod_container->setMaximumSize(512, 512);
                    cod_tmp->getMaxFrm(cellData[i][cellDataSize-2][0]);
                    cod_tmp->getMaxSize(QSize(/*640, 480*/800, 600));

//                    nar_tmp1->setBeginFrame(cellData[i][0][0]);
//                    nar_tmp1->setMaxFrm(cellData[i][cellDataSize-2][0]);
//                    cod_tmp1->getMaxFrm(cellData[i][cellDataSize-2][0]);
//                    cod_tmp1->getMaxSize(QSize(/*640, 480*/800, 600));

                    visGLayout->addWidget(nar_container, 0, i);
                    visGLayout->addWidget(cod_container, 1, i);

                    for(unsigned int n = 0; n < cellDataSize; n++){
                        // data
                        //emit readProperties(cellData[i][n]);
                        nar_tmp->updateProperty(cellData[i][n], cellData[i][n][0]);
                        cod_tmp->updateCoord(QPointF(cellData[i][n][3], cellData[i][n][4]), cellData[i][n][0]);
                        //qDebug() << cellData[i][n][3] << " " <<  cellData[i][n][4] << " " << cellData[i][n][0];
                        // img
                        //QImage img = readImgFile(fi.path(), cellData[i][n][0]/*index*/);
                        //emit readCellImg(img);
                    }
                }
                if(cellDataSize <= 20){
                    qDebug() << "Cell Data Size ERROR!";
                }


                // bleb size
                if(readBlebsFile(datafileNames[i]) && readContoursFile(datafileNames[i])){
                    if((contours[i].size() == blebs[i].size()) && (blebs[i].size() > 20)){

                        Shape *shp_tmp = new Shape();
                        QWidget *shp_container = QWidget::createWindowContainer(shp_tmp, this);
                        shp_container->setMinimumSize(256, 256);
                        shp_container->setMaximumSize(512, 512);

                        shp_tmp->setBeginFrm(cellData[i][0][0]);
                        shp_tmp->setMaxFrm(cellData[i][cellDataSize-2][0]);

                        visGLayout->addWidget(shp_container, 2, i);


                        for(int n = 0; n < blebs[i].size(); n++){
                            //emit readContourNBlebs(blebs[i][n], contours[i][n], centers[i][n]);
                            shp_tmp->updateContourNBleb(blebs[i][n], contours[i][n], centers[i][n]);
                        }
                        shp_tmp->setNeedUpdate();
                    }
                    else qDebug() << "Bleb and Contour Data Size EROOR!" << contours[i].size() << " " << blebs[i].size();
                }
                else qDebug() << "Read Bleb and Contour Data files EROOR!";

            }

        }
        else{
            qDebug() << i << "th data file is Empty.";
        }

    } // for loop end

    qDebug() << "DONE!!!!!!!!!!!!!!";

}

bool MultiView::readDataFile(QString &filename)
{
    std::vector<floatArray> cellData1;
    QFile f(/**dataFilename*/filename);
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
            cellData1.push_back(prop);
            //emit readProperties(prop);
        }
        f.close();
        cellData.push_back(cellData1);
        //readBlebsFile();
        //readContoursFile();
        return true;
    }

}

bool MultiView::readBlebsFile(QString &filename)
{

    QString tmp = /**dataFilename*/filename;
    QString fn_b = tmp.remove(tmp.length()-15, 15) + "_b_compressed.dat" ;

    QVector<QVector<Bleb> > blebs1;
    QVector<QPoint>         centers1;

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
            blebs1.push_back(blebs_1frm);
            centers1.push_back(QPoint(centX, centY));
        }

        blebs.push_back(blebs1);
        centers.push_back(centers1);
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

bool MultiView::readContoursFile(QString &filename)
{

    QString tmp = /**dataFilename*/filename;
    QString fn_c = tmp.remove(tmp.length()-15, 15) + "_c_compressed.dat" ;
    //qDebug() << fn_c;

    QVector<QVector<QPoint> >   contours1;

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
            contours1.push_back(contour);
        }
        contours.push_back(contours1);
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

