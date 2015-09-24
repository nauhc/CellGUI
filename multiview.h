#ifndef MULTIVIEW_H
#define MULTIVIEW_H

#include <QWidget>
#include <QSlider>
#include <QComboBox>
#include <QBoxLayout>
#include <QSpacerItem>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QDockWidget>
#include <QScrollArea>
#include "style.h"
#include "viswindow.h"
#include "narr.h"
#include "coord.h"
#include "shape.h"
#include "binvec.h"
#include "bleb.h"

struct expPara{  // experimental parameters
//    int     idx;
    qreal   pressu;
    qreal   foorce;
    qreal   tempra;
};

class canvasWidget : public QWidget{
    Q_OBJECT
public:
    canvasWidget();
    void clearCanvas();
protected:
    bool clearAll;
    virtual void paintEvent(QPaintEvent *e);
};

class MultiView : public QWidget
{
    Q_OBJECT
public:
    explicit MultiView(QWidget *parent = 0);
    void    sortbyParameter(int i);
    void    pushProps(int i);
    void    clearProps();
    void    clearCanvas();
    void    setTimeStt(int stt);
    void    setTimeEnd(int end);
    void    setVisSideLen(int len);
    QVector<QFileInfo>    getFilenames();
    QVector<unsigned int> getUpdatedIndex();
    void    updateIndexAndValue(int id, int state);

signals:
//    void readProperties(floatArray prop);
//    void readCellImg(QImage img);
//    void readContourNBlebs(QVector<Bleb>, QVector<QPoint>, QPoint);

public slots:
    // load files button
//    void loadFilesButton_pressed();
//    void loadFilesButton_released();
    void loadFilesButton_clicked();

private:
    QVector<QFileInfo>                      datafileInfos; //stores all the filenames
    //QVector<bool>                           datafileRead; //stores all the files read or not
    QVector<int>                            showProps; // stores the index of the properties that need to be showed
    //QWidget                                 *visContainer; // large conainer storing all the vis info
    canvasWidget                            *canvas;
    std::vector<std::vector<floatArray> >   cellData;
    QVector<QVector<QVector<Bleb> > >       blebs;
    QVector<QVector<QVector<QPoint> > >     contours;
    QVector<QVector<QPoint> >               centers;
    QVector<QVector<int> >                  clusters;

    QHash<int, expPara>                     expParas; // all experimental parameters
    QList<QPair<qreal,int> >                pressure;
    QList<QPair<qreal,int> >                force;
    QList<QPair<qreal, int> >               temprature;

    QVector<unsigned int>                   index_sort; // sorted index
//    QVector<QVector<float> >                value_sort; // sorted value: pressre/force/temprture
    QVector<unsigned int>                   index_sort_copy; // copy of the index
    unsigned int                            minFrm; // selecting frame range to show (min)
    unsigned int                            maxFrm; // selecting frame range to show (max)
//    unsigned int                            timeStt;
//    unsigned int                            timeEnd;

    int                                     visSideLen;

    QGridLayout                             *visGLayout;
    QScrollArea                             *scrollArea;
    QVBoxLayout                             *mainVLayout;

    QPushButton                             *loadFilesButton;

    bool    loadFiles();
    void    display();
    bool    readDataFile(QString &filename);
    bool    readBlebsFile(QString &filename);
    bool    readContoursFile(QString &filename);
    bool    readExpParaFile(QString &filename, int n);
    bool    readClusterFile(QString &filename);
    QImage  readImgFile(QString fp, unsigned int idx);
    void    clearData();

    void    createLoadFilesButton();
    void    createVisCanvas();

    void    visPropbyIdx(int index, int size, int i, int j, int propTp, QVector<float> v, bool roomT);
    void    showCircularProp(int index, QString filename, int size, int i, int j, int propTp, QVector<float> value, bool roomT);
    void    showTrajectory(int index, QString filename, int size, int i, int j, QVector<float> value, bool roomT);
    void    showShape(int index, QString filename, int size, int i, int j, QVector<float> value, bool roomT);
    void    showBinVec(int index, QString filename, int size, int i, int j, QVector<float> value, bool roomT);

//    bool    clearAll;
//    void    paintEvent(QPaintEvent *e);

};

#endif // MULTIVIEW_H
