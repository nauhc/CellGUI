#include "multiview.h"
#include <QDebug>
#include <iostream>
#include <QDirIterator>
#include <QtAlgorithms>
#include <iostream>
#include <QGraphicsItem>
#include <QScroller>

struct QPairFirstComparer
{
    template<typename T1, typename T2>
    bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const
    {
        return a.first < b.first;
    }
};

MultiView::MultiView(QWidget *parent) :
    QWidget(parent)
  , minFrm(0)
  , maxFrm(5000)
{

    setStyleSheet("background-color:rgb(251,251,251)");

    mainVLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);


    visGLayout = new QGridLayout();

//    createLoadFilesButton();
    loadFilesButton_clicked();
    //setShowProps();

//    mainVLayout->addWidget(loadFilesButton);

    QWidget *temp = new QWidget();
    temp->setLayout(visGLayout);
    scrollArea->setWidget(temp);

//    QScroller *scroller = QScroller::scroller(scrollArea);


//    QGraphicsView *view = new QGraphicsView();
//    view->setLayout(visGLayout);
//    scrollArea->setWidget(view);


    mainVLayout->addWidget(scrollArea);
    //    mainVLayout->addLayout(visGLayout);

}

void MultiView::pushProps(int i)
{
//    showProps.push_back(0); // area
//    showProps.push_back(1); // perimeter
//    showProps.push_back(2); // blebs number and size
//    showProps.push_back(3); // centroid trajectory
//    showProps.push_back(4); // shape
//    showProps.clear();
    showProps.push_back(i); // shape
    //qDebug() << showProps;
    show();
}

void MultiView::clearProps()
{
    showProps.clear();
//    QLayoutItem *child;
//    while ((child = visGLayout->takeAt(0)) != 0)
//        delete child;

}

void MultiView::clearData()
{
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

void MultiView::sortbyParameter(int i)
{
    qDebug() << "para" << i << "is checked";
    if (i == 0){
        for(int n = 0; n < datafileInfos.size(); n++){
//            index_sort.push_back(pressure[n].second);
            index_sort.replace(n, pressure[n].second);
            value_sort.replace(n, pressure[n].first);
        }
    }
    else if (i == 1){
        for(int n = 0; n < datafileInfos.size(); n++){
            //index_sort.push_back(force[n].second);
            index_sort.replace(n, force[n].second);
            value_sort.replace(n, force[n].first*1000000);
        }
    }
    else if (i == 2){
        for(int n = 0; n < datafileInfos.size(); n++){
            //index_sort.push_back(pressure[n].second);
            //index_sort.replace(n, pressure[n].second);
            index_sort.replace(n, n);
            value_sort.replace(n, 0);
        }
    }
    else{
        for(int n = 0; n < datafileInfos.size(); n++){
            index_sort.replace(n, n);
            value_sort.replace(n, 0);
        }
    }
    //qDebug() << index_sort;
    show();

}

void MultiView::loadFilesButton_clicked() // first round
{
    clearData();
//    pushProps();

    if(loadFiles()){ // datafileNames prepared
        //show();
        std::cout << "Cell Data loaded." << std::endl;
//        prop0 = new QCheckBox("Area"); // 0
//        prop1 = new QCheckBox("Perimeter"); // 1
//        prop2 = new QCheckBox("Blebs Number and Size"); // 2
//        prop3 = new QCheckBox("Centroid Trajectory"); // 3
//        prop4 = new QCheckBox("Shape"); // 4
        pushProps(0);
        sortbyParameter(3);
     }

}

bool MultiView::loadFiles()
{
//    QString folderPath = "../../../video/ExtractedData/";
    QString folderPath = "/Users/chuanwang/Sourcecode/CellGUI/video/ExtractedData";

    QDirIterator dirIt(folderPath, QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
            if (QFileInfo(dirIt.filePath()).suffix() == "csv")
                datafileInfos.push_back(dirIt.fileInfo());
    }

    // read files :
    int fileNum = datafileInfos.size();
    for(int n = 0; n < fileNum; n++){

        QString datafilename = datafileInfos[n].absoluteFilePath();
        index_sort.push_back(n);
        value_sort.push_back(0);
        if(!datafilename.isEmpty()){
            if(readDataFile(datafilename)){
                //std::cout << "File " << datafilename.toUtf8().constData() << " read." << std::endl;
                if(readBlebsFile(datafilename) &&
                        readContoursFile(datafilename) &&
                        readExpParaFile(datafilename, n)){
                    //std::cout << "blebs, contours and experiment-parameter files read." << std::endl;
                }
                else{
                    qDebug() << "blebs and contours files reading ERROR.";
                    return false;
                }
            }
            else {
                qDebug() << "csv files read ERROR." ;
                return false;
            }
        }
    }

    // sort on pressure with index together
    qSort(pressure.begin(), pressure.end(), QPairFirstComparer());
    //qDebug() << pressure;

    qSort(force.begin(), force.end(), QPairFirstComparer());
    //qDebug() << force;

    return true;
}


void MultiView::createVisCanvas()
{
    //    visWindow = new VisWindow();
    //    visContainer = QWidget::createWindowContainer(visWindow, this);
    //    visContainer->setMinimumSize(this->width()*0.8, this->height()*0.8);
    //    visContainer->setBaseSize(this->width()*0.8, this->height()*0.8);
    //    visWindow->resize(/*visContainer->size()*/ QSize(1024, 768));
}

void MultiView::showCircularProp(int index, int size, int i, int j, int propTp, float value)
{
    unsigned int cellDataSize = cellData[index].size();
    if(cellDataSize > 20){
        int idxMin = cellData[index][0][0];
        int idxMax = cellData[index][cellDataSize-2][0];

        idxMax = idxMin + maxFrm;

        QString file = datafileInfos[index].fileName();
        QLabel *nameLabel = new QLabel(file.remove(file.length()-4, 4));
        nameLabel->setFixedSize(size, 15);
        nameLabel->setAlignment(Qt::AlignCenter);

        visGLayout->addWidget(nameLabel, 2*j, i);

        Narr *nar_tmp = new Narr();
        nar_tmp->setPropertyType(propTp); // 0:"area", 1:"perimeter", 2:"bleb"
        nar_tmp->setFixedSize(size, size/*+20*/);
        nar_tmp->initialize();
        nar_tmp->setBeginFrm(idxMin);
        nar_tmp->setMaxFrm(idxMax);
        nar_tmp->setValue(value);

        for(unsigned int n = 0; n < cellDataSize; n++){
            //nar_tmp->clear();
            nar_tmp->updateProperty(cellData[index][n], cellData[index][n][0]);
//            for(int x = 0; x < cellData[index][n].size(); x++){
//                std::cout << cellData[index][n][x] << " ";
//            }
//            std::cout << std::endl;
        }

        visGLayout->addWidget(nar_tmp, 2*j+1, i);

        //shp_tmp->setNeedUpdate();
    }
    else {
        qDebug( ) << "Cell Data Size ERROR!";
    }
}

void MultiView::showTrajectory(int index, int size, int i, int j, float value)
{
    unsigned int cellDataSize = cellData[index].size();
    if(cellDataSize > 20){
        int idxMin = cellData[index][0][0];
        int idxMax = cellData[index][cellDataSize-2][0];

        int frmMax = idxMin + maxFrm;

        QString file = datafileInfos[index].fileName();
        QLabel *nameLabel = new QLabel(file.remove(file.length()-4, 4));
        nameLabel->setFixedSize(size, 15);
        nameLabel->setAlignment(Qt::AlignCenter);

        visGLayout->addWidget(nameLabel, 2*j, i);

        Coord *cod_tmp = new Coord();
        cod_tmp->clear();
        cod_tmp->setFixedSize(size, size);
        cod_tmp->setBeginFrm(idxMin);
        cod_tmp->setMaxFrm(idxMax, frmMax);
        cod_tmp->setValue(value);
        //cod_tmp->setMaxSize(QSize(800, 600));

        for(unsigned int n = 0; n < cellDataSize; n++){
            cod_tmp->updateCoord(QPointF(cellData[index][n][3], cellData[index][n][4]), cellData[index][n][0]);
        }
        //qDebug() << cellData[index].size();

        visGLayout->addWidget(cod_tmp, 2*j+1, i);

        //shp_tmp->setNeedUpdate();
    }
    else {
        qDebug( ) << "Cell Data Size ERROR!";
    }
}

void MultiView::showShape(int index, int size, int i, int j, float value)
{
    unsigned int cellDataSize = cellData[index].size();
    if(cellDataSize > 20){
        int idxMin = cellData[index][0][0];
        int idxMax = cellData[index][cellDataSize-2][0];

        int frmMax = idxMin + maxFrm;

        QString file = datafileInfos[index].fileName();
        QLabel *nameLabel = new QLabel(file.remove(file.length()-4, 4));
        nameLabel->setFixedSize(size, 15);
        nameLabel->setAlignment(Qt::AlignCenter);

        visGLayout->addWidget(nameLabel, 2*j, i);

        int SIZE = contours[index].size() >= blebs[index].size() ? blebs[index].size() : contours[index].size();
        Shape *shp_tmp = new Shape();
        shp_tmp->setFixedSize(size, size);
        shp_tmp->setBeginFrm(idxMin);
        shp_tmp->setMaxFrm(idxMax, frmMax);
        shp_tmp->setValue(value);

        visGLayout->addWidget(shp_tmp, 2*j+1, i);

        for(int n = 0; n < SIZE; n++){
            //emit readContourNBlebs(blebs[index][n], contours[index][n], centers[index][n]);
            //shp_tmp->clear();
            shp_tmp->updateContourNBleb(blebs[index][n], contours[index][n], centers[index][n]); //[movie][frm]
        }
        //shp_tmp->setNeedUpdate();
    }
    else {
        qDebug( ) << "Cell Data Size ERROR!";
    }
}

void MultiView::visPropbyIdx(int fileIdx, int size, int i, int j, int PropIdx, float v)
{
    switch (PropIdx){
    case 0:
        //std::cout << "Showing Property 'Area'. \n" << std::endl;
        showCircularProp(fileIdx, size, i, j, PropIdx, v);
        break;
    case 1:
        //std::cout << "Showing Property 'Perimeter'. \n" << std::endl;
        showCircularProp(fileIdx, size, i, j, PropIdx, v);
        break;
    case 2:
        //std::cout << "Showing Property 'Bleb size and number'. \n" << std::endl;
        showCircularProp(fileIdx, size, i, j, PropIdx, v);
        break;
    case 3:
        //std::cout << "Showing Property 'Centroid Trajectory'. \n" << std::endl;
        showTrajectory(fileIdx, size, i, j, v);
        break;
    case 4:
        //std::cout << "Showing Property 'Shape'. \n" << std::endl;
        showShape(fileIdx, size, i, j, v);
        break;
    default:
        //std::cout << "Showing Property 'Shape'. \n" << std::endl;
        showShape(fileIdx, size, i, j, v);
        break;
    }
}

void MultiView::show()
{


    // file reading succeed and draw vis
    maxFrm = 5000;
    int containerSide = 300;
    int space = 5;
    //    if (fileNum > 4)
    //        containerSide = (this->width()-space*((fileNum/7)-1))/(fileNum/7);
    //    else
    //        containerSide = this->height()/4-space;
    //containerSide = (this->height()-space*6)/7;
    containerSide = (/*height()*/parentWidget()->height()-space*3 - 20*4)/4 - 20;

    int fileNum = datafileInfos.size();

    if(showProps.size() == 1){ // show all the movies in one property
        //visGLayout->setGeometry(QRect(QPoint(0,0), QPoint(fileNum*350, showProps.size()*350)));
        for(int j = 0; j <= int((fileNum-1)/7); j++){
            int numtemp = fileNum <= 7 ? fileNum : 7;
            for(int i = 0; i < /*fileNum*/numtemp; i++){
                int   idx   = j * 7 + i;
                if((idx > fileNum - 1) || (index_sort[idx] > fileNum-1))
                    continue;
                //qDebug() << idx << i << j;
                visPropbyIdx(index_sort[idx], containerSide, i, j, showProps[0], value_sort[idx]);
            }
        }
    }

    else { // multiple properties showed at one time
        visGLayout->setGeometry(QRect(QPoint(0,0), QPoint(fileNum*350, showProps.size()*350)));
        for(int p = 0; p < showProps.size(); p++){
            for (int n = 0; n < fileNum; n++){
                visPropbyIdx(index_sort[n], containerSide, n, p, showProps[p], value_sort[n]);
            }
        }
    }

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
            prop.push_back(float(row[9])); // blebSize

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

bool MultiView::readExpParaFile(QString &filename, int n)
{
    QString tmp = /**dataFilename*/filename;
    QString fn_e;
    if(tmp.contains("_compressed"))
        fn_e = tmp.remove(tmp.length()-15, 15) + "_e_compressed.data" ;
    else if(tmp.contains("_control"))
        fn_e = tmp.remove(tmp.length()-12, 12) + "_e_control.data" ;

    QFile f(fn_e);
    if(!f.open(QIODevice::ReadOnly)){
        qDebug() << "Reading experiment parameter file not found: " << fn_e;
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
        qreal forc = line2.remove("     ForceOffset(N)").toFloat();
        //qDebug() << forc;
        pressure.append(qMakePair(pres, n));
        force.append(qMakePair(forc, n));
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

