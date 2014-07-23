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
    QLabel *frameLabel;
    QPushButton *drawROIButton;
    QLabel *roiVideo2;
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
        loadVideoButton->setGeometry(QRect(1010, 80, 151, 51));
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
        playVideoButton->setGeometry(QRect(1010, 140, 151, 51));
        stopVideoButton = new QPushButton(centralWidget);
        stopVideoButton->setObjectName(QStringLiteral("stopVideoButton"));
        stopVideoButton->setGeometry(QRect(1010, 200, 151, 51));
        adaptThreshSlider = new QSlider(centralWidget);
        adaptThreshSlider->setObjectName(QStringLiteral("adaptThreshSlider"));
        adaptThreshSlider->setGeometry(QRect(1000, 420, 181, 22));
        adaptThreshSlider->setSingleStep(2);
        adaptThreshSlider->setOrientation(Qt::Horizontal);
        blkSizeSlider = new QSlider(centralWidget);
        blkSizeSlider->setObjectName(QStringLiteral("blkSizeSlider"));
        blkSizeSlider->setGeometry(QRect(1000, 490, 181, 22));
        blkSizeSlider->setOrientation(Qt::Horizontal);
        differenceLabel = new QLabel(centralWidget);
        differenceLabel->setObjectName(QStringLiteral("differenceLabel"));
        differenceLabel->setGeometry(QRect(1000, 400, 81, 16));
        blkSizeLabel = new QLabel(centralWidget);
        blkSizeLabel->setObjectName(QStringLiteral("blkSizeLabel"));
        blkSizeLabel->setGeometry(QRect(1000, 470, 81, 16));
        cellVis = new QLabel(centralWidget);
        cellVis->setObjectName(QStringLiteral("cellVis"));
        cellVis->setGeometry(QRect(40, 610, 1171, 501));
        cellVis->setAutoFillBackground(false);
        cellVis->setStyleSheet(QStringLiteral("Background-color:rgb(255, 255, 255)"));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(50, 550, 481, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        frameLabel = new QLabel(centralWidget);
        frameLabel->setObjectName(QStringLiteral("frameLabel"));
        frameLabel->setGeometry(QRect(230, 570, 111, 21));
        QFont font;
        font.setFamily(QStringLiteral("Helvetica"));
        font.setPointSize(12);
        frameLabel->setFont(font);
        frameLabel->setLayoutDirection(Qt::LeftToRight);
        frameLabel->setStyleSheet(QStringLiteral("qproperty-alignment: AlignCenter"));
        drawROIButton = new QPushButton(centralWidget);
        drawROIButton->setObjectName(QStringLiteral("drawROIButton"));
        drawROIButton->setGeometry(QRect(1010, 270, 151, 51));
        roiVideo2 = new QLabel(centralWidget);
        roiVideo2->setObjectName(QStringLiteral("roiVideo2"));
        roiVideo2->setGeometry(QRect(650, 290, 240, 240));
        roiVideo2->setStyleSheet(QStringLiteral("Background-color:rgb(0, 0, 0)"));
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
        loadVideoButton->setText(QApplication::translate("MainWindow", "Open", 0));
        roiVideo1->setText(QString());
        orgVideo->setText(QString());
        playVideoButton->setText(QApplication::translate("MainWindow", "Play", 0));
        stopVideoButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        differenceLabel->setText(QApplication::translate("MainWindow", "Difference:", 0));
        blkSizeLabel->setText(QApplication::translate("MainWindow", "Block Sizes:", 0));
        cellVis->setText(QString());
        frameLabel->setText(QString());
        drawROIButton->setText(QApplication::translate("MainWindow", "Encircle Cell", 0));
        roiVideo2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
