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
    visSideLen = 320;

    mainVLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    visGLayout = new QGridLayout();

    loadFilesButton_clicked();

    canvas = new canvasWidget();
    canvas->setLayout(visGLayout);
    scrollArea->setWidget(canvas);
    scrollArea->setStyleSheet(SCROLLBAR_WD);

    mainVLayout->addWidget(scrollArea);
    //    mainVLayout->addLayout(visGLayout);

}

void MultiView::pushProps(int i)
{
    showProps.push_back(i); // shape
    display();
}

void MultiView::clearProps()
{
    showProps.clear();
    clearCanvas();
}

void MultiView::clearCanvas()
{
    QLayoutItem *child;
    while ((child = visGLayout->takeAt(0)) != 0)
        delete child;

    delete canvas;
    visGLayout = new QGridLayout();
    canvas = new canvasWidget();
    canvas->setLayout(visGLayout);
    scrollArea->setWidget(canvas);

}

void MultiView::setTimeStt(int stt)
{
    minFrm = stt;
    display();
}

void MultiView::setTimeEnd(int end)
{
    maxFrm = end;
    //clearCanvas();
    display();
}

void MultiView::setVisSideLen(int len)
{
    visSideLen = len;
    clearCanvas();
    display();
}

QVector<QFileInfo> MultiView::getFilenames()
{
    return datafileInfos;
}

QVector<unsigned int> MultiView::getUpdatedIndex()
{
    return index_sort;
}

void MultiView::updateIndexAndValue(int id, int state)
{
    //qDebug() << "updateIndexAndValue called" << id << state;
    if(state == 0){ // uncheck
        index_sort.replace(id, 65534);
    }
    else if(state == 2){ // check
        index_sort.replace(id, index_sort_copy[id]);
    }
//    qDebug() << index_sort;
    clearCanvas();
    display();
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
            index_sort.replace(n, pressure[n].second);
            index_sort_copy.replace(n, pressure[n].second);
            value_sort.replace(n, pressure[n].first);
        }
    }
    else if (i == 1){
        for(int n = 0; n < datafileInfos.size(); n++){
            index_sort.replace(n, force[n].second);
            index_sort_copy.replace(n, force[n].second);
            value_sort.replace(n, force[n].first*1000000);
        }
    }
    else if (i == 2){
        for(int n = 0; n < datafileInfos.size(); n++){
            index_sort.replace(n, temprature[n].second);
            index_sort_copy.replace(n, temprature[n].second);
            float tmp;
            if(temprature[n].first == "37")
                tmp = 37;
            else
                tmp = 25;
            value_sort.replace(n, tmp);
        }
    }
    else{
        for(int n = 0; n < datafileInfos.size(); n++){
            index_sort.replace(n, n);
            index_sort_copy.replace(n, n);
            value_sort.replace(n, 0);
        }
    }
    //qDebug() << index_sort;
    display();

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
//        pushProps(0);
        pushProps(3);
        sortbyParameter(3);
     }

}

bool MultiView::loadFiles()
{
//    QString folderPath = "../../../video/ExtractedData/";
    QString folderPath = "/Users/chuanwang/Sourcecode/CellGUI/video/ExtractedData";
//    QString folderPath = "/Users/chuanwang/Sourcecode/CellGUI/video/ExtractedData_Mar25_RT";
//    QString folderPath = "/Users/chuanwang/Sourcecode/CellGUI/video/BV2_16x_data/Arpad_BV2_37C_16x_hasPara_ajst_pureVisData";

    QDirIterator dirIt(folderPath, QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        dirIt.next();
        if (QFileInfo(dirIt.filePath()).isFile())
            if (QFileInfo(dirIt.filePath()).suffix() == "csv" && (!dirIt.fileName().contains("_pxl")))
                datafileInfos.push_back(dirIt.fileInfo());
    }

    // read files :
    int fileNum = datafileInfos.size();
    for(int n = 0; n < fileNum; n++){

        QString datafilename = datafileInfos[n].absoluteFilePath();
        index_sort.push_back(n);
        index_sort_copy.push_back(n);
        value_sort.push_back(0);

        if(!datafilename.isEmpty()){
            if(readDataFile(datafilename)){
                //std::cout << "File " << datafilename.toUtf8().constData() << " read." << std::endl;
                if(readBlebsFile(datafilename) &&
                   readContoursFile(datafilename) &&
                   readExpParaFile(datafilename, n) &&
                   readClusterFile(datafilename)){
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

    qSort(temprature.begin(), temprature.end(), QPairFirstComparer());
    //qDebug() << force;

    return true;
}

void MultiView::createVisCanvas()
{
}

void MultiView::showCircularProp(int index, int size, int i, int j, int propTp, float value, bool roomT)
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
        nar_tmp->setMaxFrm(idxMax, maxFrm);
        nar_tmp->setValue(value);

        int showSizeMin = minFrm > 0 ? minFrm : 0;
        int showSizeMax = maxFrm < cellDataSize ? maxFrm : cellDataSize;
        nar_tmp->setMaxFrm(idxMax, maxFrm);
//        for(unsigned int n = 0; n < cellDataSize; n++){
        for(unsigned int n = showSizeMin; n < showSizeMax; n++){
            //nar_tmp->clear();
            nar_tmp->updateProperty(cellData[index][n], cellData[index][n][0], clusters[index][n]);
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

void MultiView::showTrajectory(int index, int size, int i, int j, float value, bool roomT)
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
        cod_tmp->setTempType(roomT);
        //cod_tmp->setMaxSize(QSize(800, 600));
        int showSizeMin = minFrm > 0 ? minFrm : 0;
        int showSizeMax = maxFrm < cellDataSize ? maxFrm : cellDataSize;
        cod_tmp->setMaxFrm(idxMax, showSizeMax);
        cod_tmp->updateRto(float(minFrm)/5000., float(maxFrm)/5000.);
//        qDebug() << float(maxFrm)/5000.;
//        for(unsigned int n = 0; n < cellDataSize; n++){
        for(unsigned int n = showSizeMin; n < showSizeMax; n++){
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

void MultiView::showShape(int index, int size, int i, int j, float value, bool roomT)
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

        int showSizeMin = minFrm > 0 ? minFrm : 0;
        int showSizeMax = maxFrm < SIZE ? maxFrm : SIZE;
        shp_tmp->setMaxFrm(idxMax, showSizeMax);
        shp_tmp->updateRto(float(minFrm)/5000., float(maxFrm)/5000.);
        shp_tmp->setTempType(roomT);
        //for(int n = 0; n < SIZE; n++){
        for(int n = showSizeMin; n < showSizeMax; n++){
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

void MultiView::visPropbyIdx(int fileIdx, int size, int i, int j, int PropIdx, float v, bool roomT)
{
    switch (PropIdx){
    case 0:
        //std::cout << "Showing Property 'Area'. \n" << std::endl;
        showCircularProp(fileIdx, size, i, j, PropIdx, v, roomT);
        break;
    case 1:
        //std::cout << "Showing Property 'Perimeter'. \n" << std::endl;
        showCircularProp(fileIdx, size, i, j, PropIdx, v, roomT);
        break;
    case 2:
        //std::cout << "Showing Property 'Bleb size and number'. \n" << std::endl;
        showCircularProp(fileIdx, size, i, j, PropIdx, v, roomT);
        break;
    case 3:
        //std::cout << "Showing Property 'Centroid Trajectory'. \n" << std::endl;
        showTrajectory(fileIdx, size, i, j, v, roomT);
        break;
    case 4:
        //std::cout << "Showing Property 'Shape'. \n" << std::endl;
        showShape(fileIdx, size, i, j, v, roomT);
        break;
    default:
        //std::cout << "Showing Property 'Shape'. \n" << std::endl;
        showShape(fileIdx, size, i, j, v, roomT);
        break;
    }
}

void MultiView::display()
{

    // file reading succeed and draw vis
//    maxFrm = 5000;
    int space = 5;
//    visSideLen = (/*height()*/parentWidget()->height()-space*3 - 20*4)/4 - 20;
    int num1Row = (parentWidget()->width()-space*8-20)/ visSideLen;
    //qDebug() << parentWidget()->width()-space*8 << visSideLen;

    int fileNum = datafileInfos.size();
    if(showProps.size() == 1){ // show all the movies in one property
        for(int j = 0; j <= int((fileNum-1)/num1Row); j++){
            int numtemp = fileNum <= num1Row ? fileNum : num1Row;
            for(int i = 0; i < /*fileNum*/numtemp; i++){
                int   idx   = j * num1Row + i;
                if((idx > fileNum - 1) || (index_sort[idx] > fileNum-1))
                    continue;
                if(index_sort[idx] == 65534)
                    continue;
                //qDebug() << idx << i << j;
                bool rt;
                if(temprature[index_sort[idx]].first == "RT"){
                    rt = true;
                }else{ // "37"
                    rt = false;
                }
                visPropbyIdx(index_sort_copy[idx], visSideLen, i, j, showProps[0], value_sort[idx], rt);
            }
        }
    }

    else { // multiple properties showed at one time
        // clear all rendering
        QLayoutItem *child;
        while ((child = visGLayout->takeAt(0)) != 0)
            delete child;
        delete canvas;
        visGLayout = new QGridLayout();
        canvas = new canvasWidget();
        canvas->setLayout(visGLayout);
        scrollArea->setWidget(canvas);

        // draw all
        for(int p = 0; p < showProps.size(); p++){
            for (int n = 0; n < fileNum; n++){
                if(index_sort[n] == 65534)
                    continue;
                bool rt;
                if(temprature[index_sort[n]].first == "RT"){
                    rt = true;
                }else{ // "37"
                    rt = false;
                }
                visPropbyIdx(index_sort_copy[n], visSideLen, n, p, showProps[p], value_sort[n], rt);
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
        QString line1 = in.readLine();
        QString line2 = in.readLine();
        QString line3 = in.readLine();
        qreal pres = line1.remove("     pressure(pa)").toFloat();
        qreal forc = line2.remove("     Force(N)").toFloat();
        QString temp = line3.remove("     Temperature ");

        pressure.append(qMakePair(pres, n));
        force.append(qMakePair(forc, n));
        temprature.append(qMakePair(temp, n));

        return true;
    }

}

bool MultiView::readClusterFile(QString &filename) // read clustering datafile
{
    QString tmp = filename;
    QString fn_l;
    if(tmp.contains("_compressed.csv"))
        fn_l = tmp.remove(tmp.length()-15, 15) + "_l_compressed.dat" ;
    else if(tmp.contains("_control.csv"))
        fn_l = tmp.remove(tmp.length()-12, 12) + "_l_control.dat" ;
//    qDebug() << fn_l;

    QVector<int>    cluster;
    QFile f(fn_l);
    if(!f.open(QIODevice::ReadOnly)){
        qDebug() << "Reading cluster's dat file not found.";
        return false;
    }
    else{
        QDataStream in(&f);
        while(!in.atEnd()){
            qint32  clusterID;
            in >> clusterID;
            //qDebug() << clusterID;
            cluster.push_back(clusterID);
        }
    }
    clusters.push_back(cluster);

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

canvasWidget::canvasWidget() : clearAll(false)
{
//    clearAll = false;
}

void canvasWidget::clearCanvas()
{
    clearAll = true;
}

void canvasWidget::paintEvent(QPaintEvent *e)
{

//    QPainter painter(this);

//    for(unsigned int i = 0; i < 28; i++){
//        for(unsigned int j = 0; j < 10; j++){
//            painter.drawRect(i*300, j*300, 300, 300);
//        }
//    }

    if(clearAll){
        //qDebug() << "clear!!!!";
        QPainter painter(this);
//        painter.fillRect(0, 0, width(), height(), QBrush(QColor(255, 255, 255)));
        painter.eraseRect(0, 0, width(), height());
        clearAll = false;
    }
}
