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
//    QVector<Narr>     nar_list;
//    QVector<Coord>    cod_list;
//    QVector<QString>  file_list;

    QWidget                 *nar_container1, *nar_container2;
    QWidget                 *cod_container1, *cod_container2;
    QWidget                 *shp_container1, *shp_container2;
    Narr                    *nar_tmp1, *nar_tmp2;
    Coord                   *cod_tmp1, *cod_tmp2;
    Shape                   *shp_tmp1, *shp_tmp2;


    std::vector<floatArray>             cellData;
    QVector<QVector<Bleb> >             blebs;
    QVector<QVector<QPoint> >           contours;
    QVector<QPoint>                     centers;
    QString                             *dataFilename;

    QVBoxLayout *mainVLayout;
    QGridLayout *visGLayout;

    QPushButton *loadFilesButton;

    void    createLoadFilesButton();
    bool    readDataFile();
    bool    readBlebsFile();
    bool    readContoursFile();
    QImage  readImgFile(QString fp, unsigned int idx);
    void    clearAll();


    void    createNarVis();
    void    createCodVis();
    void    createShpVis();
    void    createSpacers();
};

#endif // MULTIVIEW_H
