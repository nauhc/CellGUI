/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
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
#include <QtWidgets/QLineEdit>
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
    QAction *actionLoad_existing_cell_data;
    QWidget *centralWidget;
    QLabel *roiVideo1;
    QLabel *orgVideo;
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
    QMenuBar *menuBar;
    QGroupBox *groupBox;
    QCheckBox *checkBox_area;
    QCheckBox *checkBox_perimeter;
    QCheckBox *checkBox_centroid;
    QCheckBox *checkBox_shape;
    QCheckBox *checkBox_blebbing;
    QCheckBox *checkBox_speed;
    QLabel *diffrerenceNum;
    QLabel *blkSizeNum;
    QSlider *dilSizeSlider;
    QLabel *dilSizeLabel;
    QLabel *dilSizeNum;
    QSlider *blebSizeRatioSlider;
    QLabel *blebSizeRatioLabel;
    QLabel *blebSizeRatioNum;
    QLineEdit *pixelLineEdit;
    QLabel *pixelLabel;
    QLineEdit *mmeterLineEdit;
    QLabel *mmeterLabel;
    QLabel *convertLabel;
    QGroupBox *groupBox_cellRole;
    QCheckBox *checkBox_compressed;
    QCheckBox *checkBox_control;
    QPushButton *stopVideoButton;
    QPushButton *playVideoButton;
    QPushButton *loadVideoButton;
    QComboBox *propComboBox;
    QLabel *label;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1301, 1250);
        actionLoad_existing_cell_data = new QAction(MainWindow);
        actionLoad_existing_cell_data->setObjectName(QStringLiteral("actionLoad_existing_cell_data"));
        QFont font;
        font.setFamily(QStringLiteral("Lucida Grande"));
        font.setPointSize(14);
        actionLoad_existing_cell_data->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        roiVideo1 = new QLabel(centralWidget);
        roiVideo1->setObjectName(QStringLiteral("roiVideo1"));
        roiVideo1->setGeometry(QRect(590, 30, 240, 240));
        roiVideo1->setStyleSheet(QStringLiteral(""));
        orgVideo = new QLabel(centralWidget);
        orgVideo->setObjectName(QStringLiteral("orgVideo"));
        orgVideo->setGeometry(QRect(40, 30, 500, 500));
        orgVideo->setAutoFillBackground(false);
        orgVideo->setStyleSheet(QStringLiteral(""));
        adaptThreshSlider = new QSlider(centralWidget);
        adaptThreshSlider->setObjectName(QStringLiteral("adaptThreshSlider"));
        adaptThreshSlider->setGeometry(QRect(881, 270, 161, 22));
        adaptThreshSlider->setSingleStep(2);
        adaptThreshSlider->setOrientation(Qt::Horizontal);
        blkSizeSlider = new QSlider(centralWidget);
        blkSizeSlider->setObjectName(QStringLiteral("blkSizeSlider"));
        blkSizeSlider->setGeometry(QRect(1091, 270, 161, 22));
        blkSizeSlider->setOrientation(Qt::Horizontal);
        differenceLabel = new QLabel(centralWidget);
        differenceLabel->setObjectName(QStringLiteral("differenceLabel"));
        differenceLabel->setGeometry(QRect(881, 250, 91, 20));
        QFont font1;
        font1.setPointSize(15);
        differenceLabel->setFont(font1);
        blkSizeLabel = new QLabel(centralWidget);
        blkSizeLabel->setObjectName(QStringLiteral("blkSizeLabel"));
        blkSizeLabel->setGeometry(QRect(1091, 250, 101, 20));
        blkSizeLabel->setFont(font1);
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
        drawROIButton->setGeometry(QRect(880, 370, 371, 41));
        drawROIButton->setFont(font1);
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
        cellDetectionDisplayerLabel->setGeometry(QRect(610, 295, 121, 21));
        cellDetectionDisplayerLabel->setAlignment(Qt::AlignCenter);
        frameLabelLeft = new QLabel(centralWidget);
        frameLabelLeft->setObjectName(QStringLiteral("frameLabelLeft"));
        frameLabelLeft->setGeometry(QRect(30, 550, 81, 21));
        frameLabelLeft->setAlignment(Qt::AlignCenter);
        frameLabelRight = new QLabel(centralWidget);
        frameLabelRight->setObjectName(QStringLiteral("frameLabelRight"));
        frameLabelRight->setGeometry(QRect(470, 550, 81, 21));
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
        typeComboBox->setGeometry(QRect(880, 110, 371, 31));
        menuBar = new QMenuBar(centralWidget);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1301, 22));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(890, 550, 381, 141));
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
        checkBox_shape->setGeometry(QRect(230, 30, 87, 20));
        checkBox_blebbing = new QCheckBox(groupBox);
        checkBox_blebbing->setObjectName(QStringLiteral("checkBox_blebbing"));
        checkBox_blebbing->setGeometry(QRect(230, 70, 87, 20));
        checkBox_speed = new QCheckBox(groupBox);
        checkBox_speed->setObjectName(QStringLiteral("checkBox_speed"));
        checkBox_speed->setGeometry(QRect(230, 110, 141, 20));
        diffrerenceNum = new QLabel(centralWidget);
        diffrerenceNum->setObjectName(QStringLiteral("diffrerenceNum"));
        diffrerenceNum->setGeometry(QRect(970, 250, 41, 20));
        blkSizeNum = new QLabel(centralWidget);
        blkSizeNum->setObjectName(QStringLiteral("blkSizeNum"));
        blkSizeNum->setGeometry(QRect(1200, 260, 41, 20));
        dilSizeSlider = new QSlider(centralWidget);
        dilSizeSlider->setObjectName(QStringLiteral("dilSizeSlider"));
        dilSizeSlider->setGeometry(QRect(880, 320, 161, 22));
        dilSizeSlider->setOrientation(Qt::Horizontal);
        dilSizeLabel = new QLabel(centralWidget);
        dilSizeLabel->setObjectName(QStringLiteral("dilSizeLabel"));
        dilSizeLabel->setGeometry(QRect(880, 300, 101, 20));
        dilSizeLabel->setFont(font1);
        dilSizeNum = new QLabel(centralWidget);
        dilSizeNum->setObjectName(QStringLiteral("dilSizeNum"));
        dilSizeNum->setGeometry(QRect(970, 300, 41, 20));
        blebSizeRatioSlider = new QSlider(centralWidget);
        blebSizeRatioSlider->setObjectName(QStringLiteral("blebSizeRatioSlider"));
        blebSizeRatioSlider->setGeometry(QRect(1091, 320, 161, 22));
        blebSizeRatioSlider->setOrientation(Qt::Horizontal);
        blebSizeRatioLabel = new QLabel(centralWidget);
        blebSizeRatioLabel->setObjectName(QStringLiteral("blebSizeRatioLabel"));
        blebSizeRatioLabel->setGeometry(QRect(1091, 300, 121, 20));
        blebSizeRatioLabel->setFont(font1);
        blebSizeRatioNum = new QLabel(centralWidget);
        blebSizeRatioNum->setObjectName(QStringLiteral("blebSizeRatioNum"));
        blebSizeRatioNum->setGeometry(QRect(1200, 300, 41, 20));
        pixelLineEdit = new QLineEdit(centralWidget);
        pixelLineEdit->setObjectName(QStringLiteral("pixelLineEdit"));
        pixelLineEdit->setGeometry(QRect(890, 440, 81, 21));
        pixelLineEdit->setFont(font1);
        pixelLabel = new QLabel(centralWidget);
        pixelLabel->setObjectName(QStringLiteral("pixelLabel"));
        pixelLabel->setGeometry(QRect(980, 440, 41, 21));
        pixelLabel->setFont(font1);
        mmeterLineEdit = new QLineEdit(centralWidget);
        mmeterLineEdit->setObjectName(QStringLiteral("mmeterLineEdit"));
        mmeterLineEdit->setGeometry(QRect(1080, 440, 71, 21));
        mmeterLineEdit->setFont(font1);
        mmeterLabel = new QLabel(centralWidget);
        mmeterLabel->setObjectName(QStringLiteral("mmeterLabel"));
        mmeterLabel->setGeometry(QRect(1160, 440, 91, 21));
        mmeterLabel->setFont(font1);
        convertLabel = new QLabel(centralWidget);
        convertLabel->setObjectName(QStringLiteral("convertLabel"));
        convertLabel->setGeometry(QRect(1020, 440, 41, 21));
        convertLabel->setFont(font1);
        convertLabel->setAlignment(Qt::AlignCenter);
        groupBox_cellRole = new QGroupBox(centralWidget);
        groupBox_cellRole->setObjectName(QStringLiteral("groupBox_cellRole"));
        groupBox_cellRole->setGeometry(QRect(880, 180, 371, 41));
        groupBox_cellRole->setFont(font1);
        groupBox_cellRole->setCursor(QCursor(Qt::PointingHandCursor));
        checkBox_compressed = new QCheckBox(groupBox_cellRole);
        checkBox_compressed->setObjectName(QStringLiteral("checkBox_compressed"));
        checkBox_compressed->setGeometry(QRect(10, 10, 171, 20));
        checkBox_compressed->setFont(font1);
        checkBox_compressed->setCursor(QCursor(Qt::PointingHandCursor));
        checkBox_compressed->setStyleSheet(QStringLiteral("background-color: transparent"));
        checkBox_control = new QCheckBox(groupBox_cellRole);
        checkBox_control->setObjectName(QStringLiteral("checkBox_control"));
        checkBox_control->setGeometry(QRect(230, 10, 131, 20));
        checkBox_control->setFont(font1);
        checkBox_control->setCursor(QCursor(Qt::PointingHandCursor));
        checkBox_control->setAutoFillBackground(false);
        checkBox_control->setStyleSheet(QStringLiteral("background-color: transparent"));
        stopVideoButton = new QPushButton(centralWidget);
        stopVideoButton->setObjectName(QStringLiteral("stopVideoButton"));
        stopVideoButton->setGeometry(QRect(1126, 40, 110, 50));
        stopVideoButton->setFont(font1);
        playVideoButton = new QPushButton(centralWidget);
        playVideoButton->setObjectName(QStringLiteral("playVideoButton"));
        playVideoButton->setGeometry(QRect(1000, 40, 110, 50));
        playVideoButton->setFont(font1);
        loadVideoButton = new QPushButton(centralWidget);
        loadVideoButton->setObjectName(QStringLiteral("loadVideoButton"));
        loadVideoButton->setGeometry(QRect(880, 40, 110, 50));
        loadVideoButton->setFont(font1);
        propComboBox = new QComboBox(centralWidget);
        propComboBox->setObjectName(QStringLiteral("propComboBox"));
        propComboBox->setGeometry(QRect(970, 480, 281, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(890, 480, 81, 31));
        label->setFont(font1);
        MainWindow->setCentralWidget(centralWidget);
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
        actionLoad_existing_cell_data->setText(QApplication::translate("MainWindow", "Load existing cell data", 0));
        actionLoad_existing_cell_data->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
        roiVideo1->setText(QString());
        orgVideo->setText(QString());
        differenceLabel->setText(QApplication::translate("MainWindow", "Difference:", 0));
        blkSizeLabel->setText(QApplication::translate("MainWindow", "Block Size:", 0));
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
        diffrerenceNum->setText(QString());
        blkSizeNum->setText(QString());
        dilSizeLabel->setText(QApplication::translate("MainWindow", "Dilation Size:", 0));
        dilSizeNum->setText(QString());
        blebSizeRatioLabel->setText(QApplication::translate("MainWindow", "Bleb Size Ratio:", 0));
        blebSizeRatioNum->setText(QString());
        pixelLabel->setText(QApplication::translate("MainWindow", "pixel", 0));
        mmeterLabel->setText(QApplication::translate("MainWindow", "micrometer", 0));
        convertLabel->setText(QApplication::translate("MainWindow", "as", 0));
        groupBox_cellRole->setTitle(QString());
        checkBox_compressed->setText(QApplication::translate("MainWindow", "Compressed Cell", 0));
        checkBox_control->setText(QApplication::translate("MainWindow", "Control Cell", 0));
        stopVideoButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        playVideoButton->setText(QApplication::translate("MainWindow", "Play", 0));
        loadVideoButton->setText(QApplication::translate("MainWindow", "File", 0));
        label->setText(QApplication::translate("MainWindow", "Property", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
