/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.1
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
    QLabel *roiVideo1;
    QLabel *orgVideo;
    QPushButton *playVideoButton;
    QPushButton *stopVideoButton;
    QSlider *adaptThreshSlider;
    QSlider *blkSizeSlider;
    QLabel *differenceLabel;
    QLabel *blkSizeLabel;
    QLabel *cellVis;
    QSlider *horizontalSlider;
    QPushButton *drawROIButton;
    QLabel *roiVideo2;
    QLabel *videoDisplayerLabel;
    QLabel *contourDisplayerLabel;
    QLabel *cellDetectionDisplayerLabel;
    QLabel *frameLabelLeft;
    QLabel *frameLabelRight;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1251, 1199);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        loadVideoButton = new QPushButton(centralWidget);
        loadVideoButton->setObjectName(QStringLiteral("loadVideoButton"));
        loadVideoButton->setGeometry(QRect(1000, 30, 181, 51));
        roiVideo1 = new QLabel(centralWidget);
        roiVideo1->setObjectName(QStringLiteral("roiVideo1"));
        roiVideo1->setGeometry(QRect(650, 30, 240, 240));
        roiVideo1->setStyleSheet(QStringLiteral("Background-color:rgb(0, 0, 0)"));
        orgVideo = new QLabel(centralWidget);
        orgVideo->setObjectName(QStringLiteral("orgVideo"));
        orgVideo->setGeometry(QRect(40, 30, 500, 500));
        orgVideo->setAutoFillBackground(false);
        orgVideo->setStyleSheet(QStringLiteral("Background-color: rgb(0, 0, 0)"));
        playVideoButton = new QPushButton(centralWidget);
        playVideoButton->setObjectName(QStringLiteral("playVideoButton"));
        playVideoButton->setGeometry(QRect(1000, 100, 181, 51));
        stopVideoButton = new QPushButton(centralWidget);
        stopVideoButton->setObjectName(QStringLiteral("stopVideoButton"));
        stopVideoButton->setGeometry(QRect(1000, 170, 181, 51));
        adaptThreshSlider = new QSlider(centralWidget);
        adaptThreshSlider->setObjectName(QStringLiteral("adaptThreshSlider"));
        adaptThreshSlider->setGeometry(QRect(1000, 440, 181, 22));
        adaptThreshSlider->setSingleStep(2);
        adaptThreshSlider->setOrientation(Qt::Horizontal);
        blkSizeSlider = new QSlider(centralWidget);
        blkSizeSlider->setObjectName(QStringLiteral("blkSizeSlider"));
        blkSizeSlider->setGeometry(QRect(1000, 510, 181, 22));
        blkSizeSlider->setOrientation(Qt::Horizontal);
        differenceLabel = new QLabel(centralWidget);
        differenceLabel->setObjectName(QStringLiteral("differenceLabel"));
        differenceLabel->setGeometry(QRect(1000, 420, 121, 16));
        blkSizeLabel = new QLabel(centralWidget);
        blkSizeLabel->setObjectName(QStringLiteral("blkSizeLabel"));
        blkSizeLabel->setGeometry(QRect(1000, 490, 121, 16));
        cellVis = new QLabel(centralWidget);
        cellVis->setObjectName(QStringLiteral("cellVis"));
        cellVis->setGeometry(QRect(40, 610, 1171, 501));
        cellVis->setAutoFillBackground(false);
        cellVis->setStyleSheet(QStringLiteral(""));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(120, 550, 341, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        drawROIButton = new QPushButton(centralWidget);
        drawROIButton->setObjectName(QStringLiteral("drawROIButton"));
        drawROIButton->setGeometry(QRect(1000, 240, 181, 51));
        roiVideo2 = new QLabel(centralWidget);
        roiVideo2->setObjectName(QStringLiteral("roiVideo2"));
        roiVideo2->setGeometry(QRect(650, 290, 240, 240));
        roiVideo2->setStyleSheet(QStringLiteral("Background-color:rgb(0, 0, 0)"));
        videoDisplayerLabel = new QLabel(centralWidget);
        videoDisplayerLabel->setObjectName(QStringLiteral("videoDisplayerLabel"));
        videoDisplayerLabel->setGeometry(QRect(60, 40, 81, 31));
        contourDisplayerLabel = new QLabel(centralWidget);
        contourDisplayerLabel->setObjectName(QStringLiteral("contourDisplayerLabel"));
        contourDisplayerLabel->setGeometry(QRect(660, 40, 101, 21));
        cellDetectionDisplayerLabel = new QLabel(centralWidget);
        cellDetectionDisplayerLabel->setObjectName(QStringLiteral("cellDetectionDisplayerLabel"));
        cellDetectionDisplayerLabel->setGeometry(QRect(660, 300, 121, 21));
        frameLabelLeft = new QLabel(centralWidget);
        frameLabelLeft->setObjectName(QStringLiteral("frameLabelLeft"));
        frameLabelLeft->setGeometry(QRect(60, 550, 51, 21));
        frameLabelLeft->setAlignment(Qt::AlignCenter);
        frameLabelRight = new QLabel(centralWidget);
        frameLabelRight->setObjectName(QStringLiteral("frameLabelRight"));
        frameLabelRight->setGeometry(QRect(470, 550, 61, 21));
        frameLabelRight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1251, 22));
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
        loadVideoButton->setText(QApplication::translate("MainWindow", "File", 0));
        roiVideo1->setText(QString());
        orgVideo->setText(QString());
        playVideoButton->setText(QApplication::translate("MainWindow", "Play", 0));
        stopVideoButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        differenceLabel->setText(QApplication::translate("MainWindow", "Difference:", 0));
        blkSizeLabel->setText(QApplication::translate("MainWindow", "Block Sizes:", 0));
        cellVis->setText(QString());
        drawROIButton->setText(QApplication::translate("MainWindow", "Encircle Cell", 0));
        roiVideo2->setText(QString());
        videoDisplayerLabel->setText(QString());
        contourDisplayerLabel->setText(QString());
        cellDetectionDisplayerLabel->setText(QString());
        frameLabelLeft->setText(QString());
        frameLabelRight->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
