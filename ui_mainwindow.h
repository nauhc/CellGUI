/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *loadVideoButton;
    QLabel *roiVideo;
    QLabel *orgVideo;
    QPushButton *playVideoButton;
    QPushButton *stopVideoButton;
    QSlider *adaptThreshSlider;
    QSlider *blkSizeSlider;
    QLabel *differenceLabel;
    QLabel *blkSizeLabel;
    QLabel *cellVis;
    QLabel *coordVis;
    QSlider *horizontalSlider;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1715, 904);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        loadVideoButton = new QPushButton(centralWidget);
        loadVideoButton->setObjectName(QStringLiteral("loadVideoButton"));
        loadVideoButton->setGeometry(QRect(640, 110, 151, 51));
        roiVideo = new QLabel(centralWidget);
        roiVideo->setObjectName(QStringLiteral("roiVideo"));
        roiVideo->setGeometry(QRect(40, 460, 491, 361));
        roiVideo->setStyleSheet(QStringLiteral("Background-color:rgb(0, 0, 0)"));
        orgVideo = new QLabel(centralWidget);
        orgVideo->setObjectName(QStringLiteral("orgVideo"));
        orgVideo->setGeometry(QRect(40, 30, 491, 361));
        orgVideo->setAutoFillBackground(false);
        orgVideo->setStyleSheet(QStringLiteral("Background-color: rgb(0, 0, 0)"));
        playVideoButton = new QPushButton(centralWidget);
        playVideoButton->setObjectName(QStringLiteral("playVideoButton"));
        playVideoButton->setGeometry(QRect(640, 170, 151, 51));
        stopVideoButton = new QPushButton(centralWidget);
        stopVideoButton->setObjectName(QStringLiteral("stopVideoButton"));
        stopVideoButton->setGeometry(QRect(640, 230, 151, 51));
        adaptThreshSlider = new QSlider(centralWidget);
        adaptThreshSlider->setObjectName(QStringLiteral("adaptThreshSlider"));
        adaptThreshSlider->setGeometry(QRect(870, 150, 251, 22));
        adaptThreshSlider->setSingleStep(2);
        adaptThreshSlider->setOrientation(Qt::Horizontal);
        blkSizeSlider = new QSlider(centralWidget);
        blkSizeSlider->setObjectName(QStringLiteral("blkSizeSlider"));
        blkSizeSlider->setGeometry(QRect(870, 220, 251, 22));
        blkSizeSlider->setOrientation(Qt::Horizontal);
        differenceLabel = new QLabel(centralWidget);
        differenceLabel->setObjectName(QStringLiteral("differenceLabel"));
        differenceLabel->setGeometry(QRect(870, 130, 81, 16));
        blkSizeLabel = new QLabel(centralWidget);
        blkSizeLabel->setObjectName(QStringLiteral("blkSizeLabel"));
        blkSizeLabel->setGeometry(QRect(870, 200, 81, 16));
        cellVis = new QLabel(centralWidget);
        cellVis->setObjectName(QStringLiteral("cellVis"));
        cellVis->setGeometry(QRect(560, 460, 1121, 361));
        cellVis->setAutoFillBackground(false);
        cellVis->setStyleSheet(QStringLiteral("Background-color:rgb(255, 255, 255)"));
        coordVis = new QLabel(centralWidget);
        coordVis->setObjectName(QStringLiteral("coordVis"));
        coordVis->setGeometry(QRect(1200, 20, 481, 401));
        coordVis->setAutoFillBackground(false);
        coordVis->setStyleSheet(QStringLiteral("Background-color:rgb(255, 255, 255)"));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(50, 410, 471, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1715, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        loadVideoButton->setText(QApplication::translate("MainWindow", "Open", 0));
        roiVideo->setText(QString());
        orgVideo->setText(QString());
        playVideoButton->setText(QApplication::translate("MainWindow", "Play", 0));
        stopVideoButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        differenceLabel->setText(QApplication::translate("MainWindow", "Difference:", 0));
        blkSizeLabel->setText(QApplication::translate("MainWindow", "Block Sizes:", 0));
        cellVis->setText(QString());
        coordVis->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
