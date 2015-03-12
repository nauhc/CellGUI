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
#include "bleb.h"

class MultiView : public QWidget
{
    Q_OBJECT
public:
    explicit MultiView(QWidget *parent = 0);
    void    createDialogMenu(/*QDockWidget *dockWidget*/);

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
    //QVector<QString>                        datafileNames; //stores all the filenames
    QVector<QFileInfo>                      datafileInfos; //stores all the filenames
    QVector<bool>                           datafileRead; //stores all the filenames
    QVector<int>                            showProps; // stores the index of the properties that need to be showed
    bool                                    filenamesLoaded;
    QWidget                                 *visContainer; // large conainer storing all the vis info
    //VisWindow                               *visWindow; // large canvas for drawing all vis once
    std::vector<std::vector<floatArray> >   cellData;
    QVector<QVector<QVector<Bleb> > >       blebs;
    QVector<QVector<QVector<QPoint> > >     contours;
    QVector<QVector<QPoint> >               centers;
    QVector<qreal>                          pressure;
    QVector<unsigned int>                   index_sort; // sorted index
    unsigned int                            minFrm; // selecting frame range to show (min)
    unsigned int                            maxFrm; // selecting frame range to show (max)

    QGridLayout                             *visGLayout;
    QScrollArea                             *scrollArea;
    QVBoxLayout                             *mainVLayout;

    QPushButton                             *loadFilesButton;

    QDialog                                 *dialog; // UI for manipulating vis

    void    getFileNames();
    bool    show();
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

    void    setShowProps();
    void    visPropbyIdx(int index, int size, int i, int j, int propTp);
    void    showCircularProp(int index, int size, int i, int j, int propTp);
    void    showTrajectory(int index, int size, int i, int j);
    void    showShape(int index, int size, int i, int j);
};

#endif // MULTIVIEW_H
