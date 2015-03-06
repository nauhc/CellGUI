#ifndef MULTIVIEW_H
#define MULTIVIEW_H

#include <QWidget>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "style.h"
#include "viswindow.h"
#include "narr.h"
#include "coord.h"
#include "shape.h"
#include "bleb.h"

class MultiView : public QWidget
{
    Q_OBJECT
public:
    explicit MultiView(QWidget *parent = 0);

signals:
//    void readProperties(floatArray prop);
//    void readCellImg(QImage img);
//    void readContourNBlebs(QVector<Bleb>, QVector<QPoint>, QPoint);

public slots:
    // load files button
    void loadFilesButton_pressed();
    void loadFilesButton_released();
    void loadFilesButton_clicked();
//    void updatePropsVisUI(floatArray property);
//    void updateCellImg(QImage);
//    void updateCellImg(QImage, QVector<QPoint> /*smoothContour*/);
//    void updateContourNBlebs(QVector<Bleb>, QVector<QPoint>, QPoint);

    void viewByProp1();

private:
    //vector
//    QVector<QWidget>  container_nar;
//    QVector<QWidget>  container_cod;
//    QVector<QWidget>  container_shp;
//    QVector<Narr>     nar_list;
//    QVector<Coord>    cod_list;
//    QVector<Shape>    shp_list;
//    QVector<QString>  file_list;


    //QVector<QString>                        datafileNames; //stores all the filenames
    QVector<QFileInfo>                      datafileInfos; //stores all the filenames
    bool                                    filenamesLoaded;
    QWidget                                 *visContainer; // large conainer storing all the vis info
    VisWindow                               *visWindow; // large canvas for drawing all vis once
    std::vector<std::vector<floatArray> >   cellData;
    QVector<QVector<QVector<Bleb> > >       blebs;
    QVector<QVector<QVector<QPoint> > >     contours;
    QVector<QVector<QPoint> >               centers;
    QVector<qreal>                          pressure;
    QVector<unsigned int>                   index_sort; // sorted index
    unsigned int                            minFrm; // selecting frame range to show (min)
    unsigned int                            maxFrm; // selecting frame range to show (max)


    QVBoxLayout *mainVLayout;
    QGridLayout *visGLayout;

    QPushButton *loadFilesButton;

    void    getFileNames();
    bool    readFiles();
    bool    readDataFile(QString &filename);
    bool    readBlebsFile(QString &filename);
    bool    readContoursFile(QString &filename);
    bool    readExpParaFile(QString &filename);
    QImage  readImgFile(QString fp, unsigned int idx);
    void    clearVis();
    void    clearData();


    void    createLoadFilesButton();
    void    createVisCanvas();
    void    createSpacers();
};

#endif // MULTIVIEW_H
