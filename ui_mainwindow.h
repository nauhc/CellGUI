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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
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
    QLabel *prop1Vis;
    QSlider *horizontalSlider;
    QPushButton *drawROIButton;
    QLabel *roiVideo2;
    QLabel *videoDisplayerLabel;
    QLabel *contourDisplayerLabel;
    QLabel *cellDetectionDisplayerLabel;
    QLabel *frameLabelLeft;
    QLabel *frameLabelRight;
    QLabel *prop2Vis;
    QLabel *prop1VisLabel;
    QLabel *prop2VisLabel;
    QComboBox *typeComboBox;
    QGroupBox *groupBox;
    QCheckBox *checkBox_area;
    QCheckBox *checkBox_perimeter;
    QCheckBox *checkBox_centroid;
    QCheckBox *checkBox_shape;
    QCheckBox *checkBox_blebbing;
    QCheckBox *checkBox_speed;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1232, 1250);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        loadVideoButton = new QPushButton(centralWidget);
        loadVideoButton->setObjectName(QStringLiteral("loadVideoButton"));
        loadVideoButton->setGeometry(QRect(880, 30, 151, 51));
        roiVideo1 = new QLabel(centralWidget);
        roiVideo1->setObjectName(QStringLiteral("roiVideo1"));
        roiVideo1->setGeometry(QRect(590, 30, 240, 240));
        roiVideo1->setStyleSheet(QStringLiteral(""));
        orgVideo = new QLabel(centralWidget);
        orgVideo->setObjectName(QStringLiteral("orgVideo"));
        orgVideo->setGeometry(QRect(40, 30, 500, 500));
        orgVideo->setAutoFillBackground(false);
        orgVideo->setStyleSheet(QStringLiteral(""));
        playVideoButton = new QPushButton(centralWidget);
        playVideoButton->setObjectName(QStringLiteral("playVideoButton"));
        playVideoButton->setGeometry(QRect(1040, 30, 151, 51));
        stopVideoButton = new QPushButton(centralWidget);
        stopVideoButton->setObjectName(QStringLiteral("stopVideoButton"));
        stopVideoButton->setGeometry(QRect(1040, 100, 151, 51));
        adaptThreshSlider = new QSlider(centralWidget);
        adaptThreshSlider->setObjectName(QStringLiteral("adaptThreshSlider"));
        adaptThreshSlider->setGeometry(QRect(890, 450, 291, 22));
        adaptThreshSlider->setSingleStep(2);
        adaptThreshSlider->setOrientation(Qt::Horizontal);
        blkSizeSlider = new QSlider(centralWidget);
        blkSizeSlider->setObjectName(QStringLiteral("blkSizeSlider"));
        blkSizeSlider->setGeometry(QRect(890, 510, 291, 22));
        blkSizeSlider->setOrientation(Qt::Horizontal);
        differenceLabel = new QLabel(centralWidget);
        differenceLabel->setObjectName(QStringLiteral("differenceLabel"));
        differenceLabel->setGeometry(QRect(890, 430, 181, 16));
        blkSizeLabel = new QLabel(centralWidget);
        blkSizeLabel->setObjectName(QStringLiteral("blkSizeLabel"));
        blkSizeLabel->setGeometry(QRect(890, 490, 181, 16));
        prop1Vis = new QLabel(centralWidget);
        prop1Vis->setObjectName(QStringLiteral("prop1Vis"));
        prop1Vis->setGeometry(QRect(40, 610, 1170, 270));
        prop1Vis->setAutoFillBackground(false);
        prop1Vis->setStyleSheet(QStringLiteral(""));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(120, 550, 341, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        drawROIButton = new QPushButton(centralWidget);
        drawROIButton->setObjectName(QStringLiteral("drawROIButton"));
        drawROIButton->setGeometry(QRect(880, 100, 151, 51));
        roiVideo2 = new QLabel(centralWidget);
        roiVideo2->setObjectName(QStringLiteral("roiVideo2"));
        roiVideo2->setGeometry(QRect(590, 290, 240, 240));
        roiVideo2->setStyleSheet(QStringLiteral(""));
        videoDisplayerLabel = new QLabel(centralWidget);
        videoDisplayerLabel->setObjectName(QStringLiteral("videoDisplayerLabel"));
        videoDisplayerLabel->setGeometry(QRect(60, 40, 81, 31));
        contourDisplayerLabel = new QLabel(centralWidget);
        contourDisplayerLabel->setObjectName(QStringLiteral("contourDisplayerLabel"));
        contourDisplayerLabel->setGeometry(QRect(610, 40, 111, 21));
        contourDisplayerLabel->setAlignment(Qt::AlignCenter);
        cellDetectionDisplayerLabel = new QLabel(centralWidget);
        cellDetectionDisplayerLabel->setObjectName(QStringLiteral("cellDetectionDisplayerLabel"));
        cellDetectionDisplayerLabel->setGeometry(QRect(610, 300, 121, 21));
        cellDetectionDisplayerLabel->setAlignment(Qt::AlignCenter);
        frameLabelLeft = new QLabel(centralWidget);
        frameLabelLeft->setObjectName(QStringLiteral("frameLabelLeft"));
        frameLabelLeft->setGeometry(QRect(40, 550, 81, 21));
        frameLabelLeft->setAlignment(Qt::AlignCenter);
        frameLabelRight = new QLabel(centralWidget);
        frameLabelRight->setObjectName(QStringLiteral("frameLabelRight"));
        frameLabelRight->setGeometry(QRect(470, 550, 61, 21));
        frameLabelRight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        prop2Vis = new QLabel(centralWidget);
        prop2Vis->setObjectName(QStringLiteral("prop2Vis"));
        prop2Vis->setGeometry(QRect(40, 880, 1170, 270));
        prop2Vis->setAutoFillBackground(false);
        prop2Vis->setStyleSheet(QStringLiteral(""));
        prop1VisLabel = new QLabel(centralWidget);
        prop1VisLabel->setObjectName(QStringLiteral("prop1VisLabel"));
        prop1VisLabel->setGeometry(QRect(50, 590, 62, 16));
        prop2VisLabel = new QLabel(centralWidget);
        prop2VisLabel->setObjectName(QStringLiteral("prop2VisLabel"));
        prop2VisLabel->setGeometry(QRect(60, 840, 62, 16));
        typeComboBox = new QComboBox(centralWidget);
        typeComboBox->setObjectName(QStringLiteral("typeComboBox"));
        typeComboBox->setGeometry(QRect(890, 180, 291, 26));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(880, 240, 311, 151));
        checkBox_area = new QCheckBox(groupBox);
        checkBox_area->setObjectName(QStringLiteral("checkBox_area"));
        checkBox_area->setGeometry(QRect(20, 30, 87, 20));
        checkBox_perimeter = new QCheckBox(groupBox);
        checkBox_perimeter->setObjectName(QStringLiteral("checkBox_perimeter"));
        checkBox_perimeter->setGeometry(QRect(20, 70, 87, 20));
        checkBox_centroid = new QCheckBox(groupBox);
        checkBox_centroid->setObjectName(QStringLiteral("checkBox_centroid"));
        checkBox_centroid->setGeometry(QRect(20, 110, 87, 20));
        checkBox_shape = new QCheckBox(groupBox);
        checkBox_shape->setObjectName(QStringLiteral("checkBox_shape"));
        checkBox_shape->setGeometry(QRect(140, 30, 87, 20));
        checkBox_blebbing = new QCheckBox(groupBox);
        checkBox_blebbing->setObjectName(QStringLiteral("checkBox_blebbing"));
        checkBox_blebbing->setGeometry(QRect(140, 70, 87, 20));
        checkBox_speed = new QCheckBox(groupBox);
        checkBox_speed->setObjectName(QStringLiteral("checkBox_speed"));
        checkBox_speed->setGeometry(QRect(140, 110, 141, 20));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1232, 22));
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
        prop1Vis->setText(QString());
        drawROIButton->setText(QApplication::translate("MainWindow", "Encircle Cell", 0));
        roiVideo2->setText(QString());
        videoDisplayerLabel->setText(QString());
        contourDisplayerLabel->setText(QString());
        cellDetectionDisplayerLabel->setText(QString());
        frameLabelLeft->setText(QString());
        frameLabelRight->setText(QString());
        prop2Vis->setText(QString());
        prop1VisLabel->setText(QString());
        prop2VisLabel->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "Properties", 0));
        checkBox_area->setText(QApplication::translate("MainWindow", "Area", 0));
        checkBox_perimeter->setText(QApplication::translate("MainWindow", "Perimeter", 0));
        checkBox_centroid->setText(QApplication::translate("MainWindow", "Centroid", 0));
        checkBox_shape->setText(QApplication::translate("MainWindow", "Shape", 0));
        checkBox_blebbing->setText(QApplication::translate("MainWindow", "Blebbing", 0));
        checkBox_speed->setText(QApplication::translate("MainWindow", "Deforming Speed", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
