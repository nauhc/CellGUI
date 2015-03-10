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
#include "multiview.h"
#include "dockmenu.h"

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
    void loadSigleView();

private:
    Ui::MainWindow  *ui;
    QHBoxLayout     *centralLayout;


    //read temporal data from file
    bool             fileMode;
    QMenu           *fileMenu;
    QAction         *loadMultiViewAct;
    QAction         *loadSigleViewAct;

    // for multiview only
    QDockWidget     *dock;
    DockMenu        *dockMenu;

    vector<floatArray>  cellData;
    //QString             *dataFilename;

signals:
    void readProperties(floatArray prop);
    void readCellImg(QImage img);

};

#endif // MAINWINDOW_H
