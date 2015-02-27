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
    void readProperties(floatArray prop);
    void readCellImg(QImage img);
    void readContourNBlebs(QVector<Bleb>, QVector<QPoint>, QPoint);

public slots:
    // load files button
    void loadFilesButton_pressed();
    void loadFilesButton_released();
    void loadFilesButton_clicked();
    void updatePropsVisUI(floatArray property);
    void updateCellImg(QImage);
    void updateCellImg(QImage, QVector<QPoint> /*smoothContour*/);
    void updateContourNBlebs(QVector<Bleb>, QVector<QPoint>, QPoint);

private:
    //vector
    QVector<QWidget>  container_nar;
    QVector<QWidget>  container_cod;
    QVector<QWidget>  container_shp;
    QVector<Narr>     nar_list;
    QVector<Coord>    cod_list;
    QVector<Shape>    shp_list;
    QVector<QString>  file_list;


    QVector<QString>    datafileNames;
    std::vector<std::vector<floatArray> >   cellData;
    QVector<QVector<QVector<Bleb> > >       blebs;
    QVector<QVector<QVector<QPoint> > >     contours;
    QVector<QVector<QPoint> >               centers;

    // one by one
    QWidget                 *nar_container1, *nar_container2, *nar_container3;
    QWidget                 *cod_container1, *cod_container2, *cod_container3;
    QWidget                 *shp_container1, *shp_container2, *shp_container3;
    Narr                    *nar_tmp1, *nar_tmp2, *nar_tmp3;
    Coord                   *cod_tmp1, *cod_tmp2, *cod_tmp3;
    Shape                   *shp_tmp1, *shp_tmp2, *shp_tmp3;


    //std::vector<floatArray>             cellData1;
//    QVector<QVector<Bleb> >             blebs1;
//    QVector<QVector<QPoint> >           contours1;
//    QVector<QPoint>                     centers1;
//    QString                             *dataFilename1;

    QVBoxLayout *mainVLayout;
    QGridLayout *visGLayout;

    QPushButton *loadFilesButton;

    void    getFileNames();
    bool    readFiles();
    bool    readDataFile(QString &filename);
    bool    readBlebsFile(QString &filename);
    bool    readContoursFile(QString &filename);
    QImage  readImgFile(QString fp, unsigned int idx);
    void    clearVis();
    void    clearData();


    void    createLoadFilesButton();
    void    createNarVis();
    void    createCodVis();
    void    createShpVis();
    void    createSpacers();
};

#endif // MULTIVIEW_H
