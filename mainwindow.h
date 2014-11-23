#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <QString>
#include <QDebug>
#include "controller.h"
#include "encircle.h"
#include "areavis.h"
#include "narr.h"
#include "coord.h"

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
    void updateBlebSizeSliderText(int value);

    void initialVideoPlayerUI(QImage img);
    void updateVideoplayerUI(QImage img, QImage ROIimg1, QImage ROIimg2);

    void on_playVideoButton_clicked();
    void on_loadVideoButton_clicked();
    void on_stopVideoButton_clicked();
    void on_drawROIButton_clicked();

    void updatePropsVisUI(floatArray property); ///*int prop1, int prop2*/
    void updateCellImg(QImage cell);

    void on_loadVideoButton_pressed();
    void on_loadVideoButton_released();

    void on_playVideoButton_pressed();
    void on_playVideoButton_released();

    void on_stopVideoButton_pressed();
    void on_stopVideoButton_released();

    void on_drawROIButton_pressed();
    void on_drawROIButton_released();

    void box_checked(int state);

private:
    Ui::MainWindow  *ui;
    Controller      *myController;
    Encircle        *encircler;
    bool            encircled;

    DataVis         *prop1Vis;
    DataVis         *prop2Vis;
    QVector<QString> checkedBoxes;

    Narr            *narr1Vis;
    Coord           *narr2Vis;

    void setCanvas();
};

#endif // MAINWINDOW_H
