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

private:
    //vector
//    QVector<QWidget>  container_nar;
//    QVector<QWidget>  container_cod;
//    QVector<QWidget>  container_shp;
//    QVector<Narr>     nar_list;
//    QVector<Coord>    cod_list;
//    QVector<Shape>    shp_list;
//    QVector<QString>  file_list;


    QVector<QString>                        datafileNames; //stores all the filenames
    bool                                    filenamesLoaded;
    QWidget                                 visContainer; // large conainer storing all the vis info
    QImage                                  vis; // large canvas for drawing all vis once
    std::vector<std::vector<floatArray> >   cellData;
    QVector<QVector<QVector<Bleb> > >       blebs;
    QVector<QVector<QVector<QPoint> > >     contours;
    QVector<QVector<QPoint> >               centers;

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
    void    createSpacers();
};

#endif // MULTIVIEW_H
