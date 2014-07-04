#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "controller.h"
#include "encircle.h"

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

private:
    Ui::MainWindow *ui;
    Controller *myController;
    bool drawMode;
    Encircle *encircle;
};

#endif // MAINWINDOW_H
