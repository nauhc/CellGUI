#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "controller.h"

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
    void updateVideoplayerUI(QImage img);

    void on_playVideoButton_clicked();

    void on_pauseVideoButton_clicked();

    void on_loadVideoButton_clicked();

private:
    Ui::MainWindow *ui;
    Controller *myController;
};

#endif // MAINWINDOW_H
