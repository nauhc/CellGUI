#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QVector>
#include <QString>
#include <QDebug>
#include "controller.h"
#include "encircle.h"
#include "areavis.h"
#include "narr.h"
#include "coord.h"
#include "multiview.h"
//#include "dockmenu.h"

typedef vector<float> floatArray;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //read temporal data from file
    void loadMultiView();
    void loadSingleView();
    void propCheckBox_checked(int state);
    void paraCheckBox_checked(int state);
    void visSideLenSlider_valueChanged(int v);
    void timeEndSlider_valueChanged(int v);
    void timeSrtSlider_valueChanged(int v);
    void fileCheckBox_checked(int state);

private:
    Ui::MainWindow      *ui;
    QHBoxLayout         *centralLayout;


    //read temporal data from file
    QMenu               *fileMenu;
    QAction             *loadMultiViewAct;
    QAction             *loadSigleViewAct;

    // for multiview only
    MultiView           *multiview;
    QDockWidget         *dock;
    QWidget             *dockWidget;
    QVBoxLayout         *dockMainVLayout;

    // time Slider
    QGroupBox           *timeGroup;
    QVBoxLayout         *timeRangeLayout;
    QVBoxLayout         *timeSttLayout;
    QVBoxLayout         *timeEndLayout;
    QLabel              *timeSttLabel;
    QLabel              *timeEndLabel;
    QSlider             *timeSttSlider;
    QSlider             *timeEndSlider;

    // vis side lenth SLider
    QVBoxLayout         *visSideLenLayout;
    QLabel              *visSideLenLabel;
    QSlider             *visSideLenSlider;

    // property to show
    QGroupBox           *propGroup;
    QVBoxLayout         *propVLayout;
    QCheckBox           *prop0;
    QCheckBox           *prop1;
    QCheckBox           *prop2;
    QCheckBox           *prop3;
    QCheckBox           *prop4;
    QCheckBox           *prop5;
    QVector<QString>     checkedBoxes;

    // sorting parameter
    QGroupBox           *paraGroup;
    QVBoxLayout         *paraVLayout;
    QCheckBox           *para0;
    QCheckBox           *para1;
    QCheckBox           *para2;
    QCheckBox           *para3;

    // filelist checkboxes
    QGroupBox           *fileGroup;
    QVBoxLayout         *fileVLayout;
    QList<QWidget*>      fileCheckBoxes;

    void createTimeSlider();
    void createvisSizeSlider();
    void createProptyCheckbox();
    void createParaCheckbox();
    void createFileCheckBoxes();

signals:
    void readProperties(floatArray prop);
    void readCellImg(QImage img);

};

#endif // MAINWINDOW_H
