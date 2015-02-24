#include "multiview.h"
#include <QDebug>
#include <iostream>

MultiView::MultiView(QWidget *parent) : QWidget(parent), dataFilename(new QString("")){

    setStyleSheet("background-color:rgb(251,251,251)");

    mainVLayout = new QVBoxLayout(this);
    visGLayout = new QGridLayout();


    createNarVis();
    createCodVis();

    //QLabel *label1 = new QLabel("111111");
    //QLabel *label2 = new QLabel("222222");
    QLabel *label3 = new QLabel("333333");
    //QLabel *label4 = new QLabel("444444");
    //QLabel *label5 = new QLabel("555555");
    QLabel *label6 = new QLabel("666666");
    visGLayout->addWidget(nar_container1, 0, 0);
    visGLayout->addWidget(nar_container2, 0, 1);
    visGLayout->addWidget(label3, 0, 2);
    visGLayout->addWidget(cod_container1, 1, 0);
    visGLayout->addWidget(cod_container2, 1, 1);
    visGLayout->addWidget(label6, 1, 2);

    createLoadFilesButton();

    mainVLayout->addWidget(loadFilesButton);
    mainVLayout->addLayout(visGLayout);

}


void MultiView::createNarVis()
{
    nar_tmp1 = new Narr();
    nar_container1 = QWidget::createWindowContainer(nar_tmp1, this);
    nar_container1->setMinimumSize(218, 218);
    nar_tmp2 = new Narr();
    nar_container2 = QWidget::createWindowContainer(nar_tmp2, this);
    nar_container2->setMinimumSize(218, 218);
}

void MultiView::createCodVis()
{
    cod_tmp1 = new Coord();
    cod_container1 = QWidget::createWindowContainer(cod_tmp1, this);
    cod_container1->setMinimumSize(218, 218);
    cod_tmp2 = new Coord();
    cod_container2 = QWidget::createWindowContainer(cod_tmp2, this);
    cod_container2->setMinimumSize(218, 218);

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
            else{
                qDebug() << "Cell Data Size ERROR!";
            }
        }
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("Filename empty!");
        msgBox.exec();
    }

//    delete dataFilename;
//    dataFilename = new QString("");
}

void MultiView::updatePropsVisUI(floatArray property)
{
    nar_tmp1->updateProperty(property, property[0]);
    cod_tmp1->updateCoord(QPointF(property[3], property[4]), property[0]);
}

void MultiView::updateCellImg(QImage cell)
{
    nar_tmp1->updateCellImg(cell);
}

void MultiView::updateCellImg(QImage cell, QVector<QPoint> smoothContour)
{
    nar_tmp1->updateCellImg(cell, smoothContour);
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

