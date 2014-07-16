#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
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
    void updateVideoplayerUI(QImage img, QImage ROIimg);

    void on_playVideoButton_clicked();
    void on_loadVideoButton_clicked();
    void on_stopVideoButton_clicked();
    void on_drawROIButton_clicked();

    void updateAreaVisUI(int area);

private:
    Ui::MainWindow *ui;
    Controller *myController;
    bool continueDrawing;
    Encircle *encircle;
    AreaVis  *areaVis;
};

#endif // MAINWINDOW_H
