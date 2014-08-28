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
    void updateVideoplayerUI(QImage img, QImage ROIimg1, QImage ROIimg2);

    void on_playVideoButton_clicked();
    void on_loadVideoButton_clicked();
    void on_stopVideoButton_clicked();
    void on_drawROIButton_clicked();

    void updateDataVisUI(int prop1, int prop2);

    void on_loadVideoButton_pressed();

    void on_loadVideoButton_released();

    void on_playVideoButton_pressed();

    void on_playVideoButton_released();

    void on_stopVideoButton_pressed();

    void on_stopVideoButton_released();

    void on_drawROIButton_pressed();

    void on_drawROIButton_released();

    void on_checkbox_checked(int state);

//    void on_checkBox_area_stateChanged(int state);

private:
    Ui::MainWindow  *ui;
    Controller      *myController;
    Encircle        *encircler;
    DataVis         *prop1Vis;
    DataVis         *prop2Vis;
    bool            encircled;
    QVector<QString> checkedBoxes;
};

#endif // MAINWINDOW_H
