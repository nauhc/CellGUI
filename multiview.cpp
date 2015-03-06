#include "multiview.h"
#include <QDebug>
#include <iostream>
#include <QDirIterator>

MultiView::MultiView(QWidget *parent) :
    QWidget(parent)
    , filenamesLoaded(false)
    , minFrm(0)
    , maxFrm(5000)
    {

    setStyleSheet("background-color:rgb(251,251,251)");

    mainVLayout = new QVBoxLayout(this);
    visGLayout = new QGridLayout();


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
//    createVisCanvas();

    mainVLayout->addWidget(loadFilesButton);
    mainVLayout->addLayout(visGLayout);
//    mainVLayout->addWidget(visContainer);

}

void MultiView::createSpacers()
{

}

void MultiView::clearVis()
{

}

void MultiView::clearData()
{
    filenamesLoaded = false;
    if(!cellData.empty())
        cellData.clear();

    if(!blebs.empty())
        blebs.clear();

    if(!contours.empty())
        contours.clear();

    if(!centers.empty())
        centers.clear();

    if(!datafileInfos.empty())
        datafileInfos.clear();

/*
//    nar_list.clear();
//    cod_list.clear();
//    shp_list.clear();
//    file_list.clear();
//    container_cod.clear();
//    container_nar.clear();
//    container_shp.clear(); */
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

    getFileNames(); // datafileNames prepared
    readFiles();

/*
//    connect(this, SIGNAL(readProperties(floatArray)),
//            this, SLOT(updatePropsVisUI(floatArray)));

//    connect(this, SIGNAL(readCellImg(QImage)),
//            this, SLOT(updateCellImg(QImage)));

//    connect(this, SIGNAL(readContourNBlebs(QVector<Bleb>,QVector<QPoint>,QPoint)),
//            this, SLOT(updateContourNBlebs(QVector<Bleb>,QVector<QPoint>,QPoint))); */

}

void MultiView::viewByProp1()
{

}

/*
//void MultiView::updatePropsVisUI(floatArray property)
//{
//    nar_tmp1->updateProperty(property, property[0]);
//    cod_tmp1->updateCoord(QPointF(property[3], property[4]), property[0]);
////    shp_tmp1->updateContourNBleb();
//}

//void MultiView::updateCellImg(QImage cell)
//{
//    nar_tmp1->updateCellImg(cell);
//}

//void MultiView::updateCellImg(QImage cell, QVector<QPoint> smoothContour)
//{
//    nar_tmp1->updateCellImg(cell, smoothContour);
//}

//void MultiView::updateContourNBlebs(QVector<Bleb> blebs, QVector<QPoint> contour, QPoint centroid) //QVector<Bleb>, QVector<QPoint>
//{
//    shp_tmp1->updateContourNBleb(blebs, contour, centroid);
//}*/

void MultiView::getFileNames()
{
    QString folderPath = "../../../video/ExtractedData/";

    QDirIterator dirIt(folderPath, QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
            if (QFileInfo(dirIt.filePath()).suffix() == "csv")
//                datafileNames.push_back(dirIt.filePath());
                datafileInfos.push_back(dirIt.fileInfo());
    }

    filenamesLoaded = true;
}

void MultiView::createLoadFilesButton()
{
    loadFilesButton = new QPushButton("Load files");
    loadFilesButton->setStyleSheet(BUTTON_RELEASED_ON);
    loadFilesButton->setMaximumHeight(30);
    connect(this->loadFilesButton, SIGNAL(clicked()), this, SLOT(loadFilesButton_clicked()));

}

void MultiView::createVisCanvas()
{
    visWindow = new VisWindow();
    visContainer = QWidget::createWindowContainer(visWindow, this);
//    visContainer->setMinimumSize(this->width()*0.8, this->height()*0.8);
    visContainer->setBaseSize(this->width()*0.8, this->height()*0.8);
    visWindow->resize(/*visContainer->size()*/ QSize(1024, 768));
}

bool MultiView::readFiles()
{

    int fileNum = datafileInfos.size();
    int containerSide = 300;
    int space = 5;
    if (fileNum > 4)
        containerSide = (this->width()-space*(fileNum-1))/fileNum;
    else
        containerSide = this->height()/4-space;

    for(int i = 0; i < fileNum; i++){
        QString datafilename = datafileInfos[i].absoluteFilePath();

        if(!datafilename.isEmpty()){
            //QFileInfo   fi  = QFileInfo(datafileNames[i]);
            qDebug() << "Reading file " << datafilename;
            if(readDataFile(datafilename)){
                maxFrm = 500/*0*/;
                unsigned int cellDataSize = cellData[i].size();
                if(cellDataSize > 20){
                    int idxMin = cellData[i][0][0];
                    int idxMax = cellData[i][cellDataSize-2][0];

//                    if ((idxMin + maxFrm) > idxMax)
//                        maxFrm = idxMax;
//                    else
                        maxFrm = idxMin + maxFrm;

                    QString file = datafileInfos[i].fileName();
                    QLabel *nameLabel = new QLabel(file.remove(file.length()-4, 4));
                    nameLabel->setFixedSize(containerSide, 30);
                    nameLabel->setAlignment(Qt::AlignHCenter);
                    //nameLabel->setAlignment(Qt::AlignVCenter);

                    Narr *nar_tmp = new Narr();
                    QWidget *nar_container = QWidget::createWindowContainer(nar_tmp, this);
                    nar_container->setFixedSize(containerSide, containerSide+10);
                    nar_tmp->setPropType(0); // show "area"
                    //nar_tmp->setPropType(1); // show "perimeter"
                    //nar_tmp->setPropType(2); // show "bleb"
                    nar_tmp->setBeginFrm(idxMin);
                    nar_tmp->setMaxFrm(maxFrm);


                    Coord *cod_tmp = new Coord();
                    QWidget *cod_container = QWidget::createWindowContainer(cod_tmp, this);
                    cod_container->setFixedSize(containerSide, containerSide);
                    cod_tmp->setBeginFrm(idxMin);
                    cod_tmp->setMaxFrm(maxFrm);
                    cod_tmp->setMaxSize(QSize(/*640, 480*/800, 600));

                    visGLayout->addWidget(nameLabel, 0, i);
                    visGLayout->addWidget(nar_container, 1, i);
                    visGLayout->addWidget(cod_container, 2, i);

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

                    // bleb size
                    if(readBlebsFile(datafilename) && readContoursFile(datafilename)){
                        int SIZE = contours[i].size() >= blebs[i].size() ? blebs[i].size() : contours[i].size();
                            Shape *shp_tmp = new Shape();
                            QWidget *shp_container = QWidget::createWindowContainer(shp_tmp, this);
                            shp_container->setFixedSize(containerSide, containerSide);
                            shp_tmp->setBeginFrm(idxMin);
                            shp_tmp->setMaxFrm(maxFrm);

                            visGLayout->addWidget(shp_container, 3, i);

                            for(int n = 0; n < /*blebs[i].size()*/SIZE; n++){
                                //emit readContourNBlebs(blebs[i][n], contours[i][n], centers[i][n]);
                                shp_tmp->updateContourNBleb(blebs[i][n], contours[i][n], centers[i][n]);
                            }
                            shp_tmp->setNeedUpdate();
                    }


                    //parameter
                    if(readExpParaFile(datafilename)){

                    }

                    else qDebug() << "Read Bleb / Contour / Parameter Data files EROOR!";

                }
                if(cellDataSize <= 20){
                    qDebug() << "Cell Data Size ERROR!";
                    continue;
                }
            }
        }
        else{
            qDebug() << i << "th data file is Empty.";
        }

    } // for loop end

    qDebug() << "All files reading done."; // until here: fast enough
    //qDebug() << pressure;
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
//            prop.push_back(float(row[5])); // shape
//            prop.push_back(float(row[6])); // blebNum
            prop.push_back(float(row[5])); // speed.dist
            prop.push_back(float(row[6])); // speed.theta
            prop.push_back(float(row[7])); // shape
            prop.push_back(float(row[8])); // blebNum

            //            for(unsigned int n = 0; n < prop.size(); n++)
            //                std::cout << prop[n] << " ";
            //            std::cout << std::endl;
            cellData1.push_back(prop);
            //emit readProperties(prop);
        }
        f.close();
        cellData.push_back(cellData1);
        return true;
    }

}

bool MultiView::readBlebsFile(QString &filename)
{

    QString tmp = /**dataFilename*/filename;
    QString fn_b;
    if(tmp.contains("_compressed"))
        fn_b = tmp.remove(tmp.length()-15, 15) + "_b_compressed.dat" ;
    else if(tmp.contains("_control"))
        fn_b = tmp.remove(tmp.length()-12, 12) + "_b_control.dat" ;

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
    QString fn_c;
    if(tmp.contains("_compressed"))
        fn_c = tmp.remove(tmp.length()-15, 15) + "_c_compressed.dat" ;
    else if(tmp.contains("_control"))
        fn_c = tmp.remove(tmp.length()-12, 12) + "_c_control.dat" ;

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

bool MultiView::readExpParaFile(QString &filename)
{
    QString tmp = /**dataFilename*/filename;
    QString fn_e;
    if(tmp.contains("_compressed"))
        fn_e = tmp.remove(tmp.length()-15, 15) + "_e_compressed.data" ;
    else if(tmp.contains("_control"))
        fn_e = tmp.remove(tmp.length()-12, 12) + "_e_control.data" ;

    QFile f(fn_e);
    if(!f.open(QIODevice::ReadOnly)){
        qDebug() << "Reading experiment parameter file not found.";
        return false;
    }else{
        QTextStream in(&f);
//        while(!in.atEnd()) { // each row
//            QString line = in.readLine();
//                qDebug() << line;
//        }
//        qDebug() << "";
        QString line1 = in.readLine();
        QString line2 = in.readLine();
        QString line3 = in.readLine();
        QString line4 = in.readLine();
        //qDebug() << line1.remove("     pressure(pa)");
        qreal pres = line1.remove("     pressure(pa)").toFloat();
        pressure.push_back(pres);
        return true;
    }
}

QImage MultiView::readImgFile(QString fp, unsigned int idx) // filepath, index
{
    QImage img;
    QString str = fp+"/cell"+QString::number(int(idx))+".png";
    //qDebug() << str;
    img.load(str);
    return img;
}

