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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_existing_cell_data;
    QWidget *centralWidget;
    QLabel *prop1Vis;
    QLabel *videoDisplayerLabel;
    QLabel *contourDisplayerLabel;
    QLabel *cellDetectionDisplayerLabel;
    QLabel *prop2Vis;
    QLabel *prop1VisLabel;
    QLabel *prop2VisLabel;
    QMenuBar *menuBar;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *roiVideo_vLayout;
    QLabel *roiVideo1;
    QLabel *roiVideo2;
    QWidget *layoutWidget;
    QVBoxLayout *control_vLayout;
    QHBoxLayout *playerBarLayout;
    QLabel *frameLabelLeft;
    QSlider *horizontalSlider;
    QLabel *frameLabelRight;
    QHBoxLayout *playerButtonLayout;
    QPushButton *loadVideoButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *playVideoButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *stopVideoButton;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *videoType_hLayout;
    QComboBox *typeComboBox;
    QHBoxLayout *cellRole_hLayout;
    QCheckBox *checkBox_compressed;
    QCheckBox *checkBox_control;
    QGridLayout *prmtr_gridLayout;
    QLabel *dilSizeNum;
    QLabel *blebSizeRatioLabel;
    QLabel *blkSizeLabel;
    QLabel *differenceLabel;
    QSlider *blkSizeSlider;
    QSlider *adaptThreshSlider;
    QLabel *blebSizeRatioNum;
    QSlider *blebSizeRatioSlider;
    QLabel *blkSizeNum;
    QSlider *dilSizeSlider;
    QLabel *dilSizeLabel;
    QLabel *diffrerenceNum;
    QHBoxLayout *encircleButton_hLayout;
    QPushButton *drawROIButton;
    QHBoxLayout *horizontalLayout;
    QLineEdit *pixelLineEdit;
    QLabel *pixelLabel;
    QLineEdit *mmeterLineEdit;
    QLabel *mmeterLabel;
    QHBoxLayout *property_hLayout;
    QLabel *label;
    QComboBox *propComboBox;
    QLabel *orgVideo;
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
        prop1Vis = new QLabel(centralWidget);
        prop1Vis->setObjectName(QStringLiteral("prop1Vis"));
        prop1Vis->setGeometry(QRect(40, 610, 1170, 270));
        prop1Vis->setAutoFillBackground(false);
        prop1Vis->setStyleSheet(QStringLiteral(""));
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
        menuBar = new QMenuBar(centralWidget);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1301, 22));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(600, 50, 261, 491));
        roiVideo_vLayout = new QVBoxLayout(verticalLayoutWidget);
        roiVideo_vLayout->setSpacing(6);
        roiVideo_vLayout->setContentsMargins(11, 11, 11, 11);
        roiVideo_vLayout->setObjectName(QStringLiteral("roiVideo_vLayout"));
        roiVideo_vLayout->setContentsMargins(0, 0, 0, 0);
        roiVideo1 = new QLabel(verticalLayoutWidget);
        roiVideo1->setObjectName(QStringLiteral("roiVideo1"));
        roiVideo1->setStyleSheet(QStringLiteral(""));

        roiVideo_vLayout->addWidget(roiVideo1);

        roiVideo2 = new QLabel(verticalLayoutWidget);
        roiVideo2->setObjectName(QStringLiteral("roiVideo2"));
        roiVideo2->setStyleSheet(QStringLiteral(""));

        roiVideo_vLayout->addWidget(roiVideo2);

        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(890, 50, 381, 491));
        control_vLayout = new QVBoxLayout(layoutWidget);
        control_vLayout->setSpacing(6);
        control_vLayout->setContentsMargins(11, 11, 11, 11);
        control_vLayout->setObjectName(QStringLiteral("control_vLayout"));
        control_vLayout->setContentsMargins(0, 0, 0, 0);
        playerBarLayout = new QHBoxLayout();
        playerBarLayout->setSpacing(0);
        playerBarLayout->setObjectName(QStringLiteral("playerBarLayout"));
        frameLabelLeft = new QLabel(layoutWidget);
        frameLabelLeft->setObjectName(QStringLiteral("frameLabelLeft"));
        frameLabelLeft->setMinimumSize(QSize(80, 0));
        frameLabelLeft->setMaximumSize(QSize(16777215, 30));
        QFont font1;
        font1.setPointSize(15);
        frameLabelLeft->setFont(font1);
        frameLabelLeft->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        playerBarLayout->addWidget(frameLabelLeft);

        horizontalSlider = new QSlider(layoutWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        playerBarLayout->addWidget(horizontalSlider);

        frameLabelRight = new QLabel(layoutWidget);
        frameLabelRight->setObjectName(QStringLiteral("frameLabelRight"));
        frameLabelRight->setMinimumSize(QSize(80, 0));
        frameLabelRight->setMaximumSize(QSize(16777215, 30));
        frameLabelRight->setFont(font1);
        frameLabelRight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        playerBarLayout->addWidget(frameLabelRight);


        control_vLayout->addLayout(playerBarLayout);

        playerButtonLayout = new QHBoxLayout();
        playerButtonLayout->setSpacing(0);
        playerButtonLayout->setObjectName(QStringLiteral("playerButtonLayout"));
        loadVideoButton = new QPushButton(layoutWidget);
        loadVideoButton->setObjectName(QStringLiteral("loadVideoButton"));
        loadVideoButton->setMinimumSize(QSize(125, 45));
        loadVideoButton->setFont(font1);

        playerButtonLayout->addWidget(loadVideoButton);

        horizontalSpacer = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playerButtonLayout->addItem(horizontalSpacer);

        playVideoButton = new QPushButton(layoutWidget);
        playVideoButton->setObjectName(QStringLiteral("playVideoButton"));
        playVideoButton->setMinimumSize(QSize(125, 45));
        playVideoButton->setFont(font1);

        playerButtonLayout->addWidget(playVideoButton);

        horizontalSpacer_2 = new QSpacerItem(5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playerButtonLayout->addItem(horizontalSpacer_2);

        stopVideoButton = new QPushButton(layoutWidget);
        stopVideoButton->setObjectName(QStringLiteral("stopVideoButton"));
        stopVideoButton->setMinimumSize(QSize(125, 45));
        stopVideoButton->setFont(font1);

        playerButtonLayout->addWidget(stopVideoButton);


        control_vLayout->addLayout(playerButtonLayout);

        verticalSpacer = new QSpacerItem(20, 5, QSizePolicy::Minimum, QSizePolicy::Fixed);

        control_vLayout->addItem(verticalSpacer);

        videoType_hLayout = new QHBoxLayout();
        videoType_hLayout->setSpacing(6);
        videoType_hLayout->setObjectName(QStringLiteral("videoType_hLayout"));
        videoType_hLayout->setContentsMargins(5, -1, 5, -1);
        typeComboBox = new QComboBox(layoutWidget);
        typeComboBox->setObjectName(QStringLiteral("typeComboBox"));
        typeComboBox->setMinimumSize(QSize(0, 30));

        videoType_hLayout->addWidget(typeComboBox);


        control_vLayout->addLayout(videoType_hLayout);

        cellRole_hLayout = new QHBoxLayout();
        cellRole_hLayout->setSpacing(5);
        cellRole_hLayout->setObjectName(QStringLiteral("cellRole_hLayout"));
        cellRole_hLayout->setContentsMargins(5, -1, -1, -1);
        checkBox_compressed = new QCheckBox(layoutWidget);
        checkBox_compressed->setObjectName(QStringLiteral("checkBox_compressed"));
        checkBox_compressed->setFont(font1);
        checkBox_compressed->setCursor(QCursor(Qt::PointingHandCursor));
        checkBox_compressed->setStyleSheet(QStringLiteral("background-color: transparent"));

        cellRole_hLayout->addWidget(checkBox_compressed);

        checkBox_control = new QCheckBox(layoutWidget);
        checkBox_control->setObjectName(QStringLiteral("checkBox_control"));
        checkBox_control->setMaximumSize(QSize(110, 16777215));
        QFont font2;
        font2.setPointSize(15);
        font2.setStyleStrategy(QFont::PreferAntialias);
        checkBox_control->setFont(font2);
        checkBox_control->setCursor(QCursor(Qt::PointingHandCursor));
        checkBox_control->setLayoutDirection(Qt::LeftToRight);
        checkBox_control->setAutoFillBackground(false);
        checkBox_control->setStyleSheet(QStringLiteral("background-color: transparent"));

        cellRole_hLayout->addWidget(checkBox_control);


        control_vLayout->addLayout(cellRole_hLayout);

        prmtr_gridLayout = new QGridLayout();
        prmtr_gridLayout->setSpacing(6);
        prmtr_gridLayout->setObjectName(QStringLiteral("prmtr_gridLayout"));
        prmtr_gridLayout->setHorizontalSpacing(30);
        prmtr_gridLayout->setVerticalSpacing(0);
        prmtr_gridLayout->setContentsMargins(5, -1, -1, -1);
        dilSizeNum = new QLabel(layoutWidget);
        dilSizeNum->setObjectName(QStringLiteral("dilSizeNum"));
        dilSizeNum->setMaximumSize(QSize(16777215, 30));

        prmtr_gridLayout->addWidget(dilSizeNum, 2, 1, 1, 1);

        blebSizeRatioLabel = new QLabel(layoutWidget);
        blebSizeRatioLabel->setObjectName(QStringLiteral("blebSizeRatioLabel"));
        blebSizeRatioLabel->setFont(font1);

        prmtr_gridLayout->addWidget(blebSizeRatioLabel, 2, 2, 1, 1);

        blkSizeLabel = new QLabel(layoutWidget);
        blkSizeLabel->setObjectName(QStringLiteral("blkSizeLabel"));
        blkSizeLabel->setMaximumSize(QSize(100, 30));
        blkSizeLabel->setFont(font1);

        prmtr_gridLayout->addWidget(blkSizeLabel, 0, 2, 1, 1);

        differenceLabel = new QLabel(layoutWidget);
        differenceLabel->setObjectName(QStringLiteral("differenceLabel"));
        differenceLabel->setMaximumSize(QSize(100, 30));
        differenceLabel->setFont(font1);

        prmtr_gridLayout->addWidget(differenceLabel, 0, 0, 1, 1);

        blkSizeSlider = new QSlider(layoutWidget);
        blkSizeSlider->setObjectName(QStringLiteral("blkSizeSlider"));
        blkSizeSlider->setMaximumSize(QSize(180, 30));
        blkSizeSlider->setOrientation(Qt::Horizontal);

        prmtr_gridLayout->addWidget(blkSizeSlider, 1, 2, 1, 2);

        adaptThreshSlider = new QSlider(layoutWidget);
        adaptThreshSlider->setObjectName(QStringLiteral("adaptThreshSlider"));
        adaptThreshSlider->setMaximumSize(QSize(180, 30));
        adaptThreshSlider->setSingleStep(2);
        adaptThreshSlider->setOrientation(Qt::Horizontal);

        prmtr_gridLayout->addWidget(adaptThreshSlider, 1, 0, 1, 2);

        blebSizeRatioNum = new QLabel(layoutWidget);
        blebSizeRatioNum->setObjectName(QStringLiteral("blebSizeRatioNum"));
        blebSizeRatioNum->setMaximumSize(QSize(100, 30));

        prmtr_gridLayout->addWidget(blebSizeRatioNum, 2, 3, 1, 1);

        blebSizeRatioSlider = new QSlider(layoutWidget);
        blebSizeRatioSlider->setObjectName(QStringLiteral("blebSizeRatioSlider"));
        blebSizeRatioSlider->setMaximumSize(QSize(180, 30));
        blebSizeRatioSlider->setOrientation(Qt::Horizontal);

        prmtr_gridLayout->addWidget(blebSizeRatioSlider, 3, 2, 1, 2);

        blkSizeNum = new QLabel(layoutWidget);
        blkSizeNum->setObjectName(QStringLiteral("blkSizeNum"));
        blkSizeNum->setMaximumSize(QSize(100, 30));

        prmtr_gridLayout->addWidget(blkSizeNum, 0, 3, 1, 1);

        dilSizeSlider = new QSlider(layoutWidget);
        dilSizeSlider->setObjectName(QStringLiteral("dilSizeSlider"));
        dilSizeSlider->setMaximumSize(QSize(180, 30));
        dilSizeSlider->setOrientation(Qt::Horizontal);

        prmtr_gridLayout->addWidget(dilSizeSlider, 3, 0, 1, 2);

        dilSizeLabel = new QLabel(layoutWidget);
        dilSizeLabel->setObjectName(QStringLiteral("dilSizeLabel"));
        dilSizeLabel->setFont(font1);

        prmtr_gridLayout->addWidget(dilSizeLabel, 2, 0, 1, 1);

        diffrerenceNum = new QLabel(layoutWidget);
        diffrerenceNum->setObjectName(QStringLiteral("diffrerenceNum"));
        diffrerenceNum->setMaximumSize(QSize(100, 30));

        prmtr_gridLayout->addWidget(diffrerenceNum, 0, 1, 1, 1);


        control_vLayout->addLayout(prmtr_gridLayout);

        encircleButton_hLayout = new QHBoxLayout();
        encircleButton_hLayout->setSpacing(6);
        encircleButton_hLayout->setObjectName(QStringLiteral("encircleButton_hLayout"));
        drawROIButton = new QPushButton(layoutWidget);
        drawROIButton->setObjectName(QStringLiteral("drawROIButton"));
        drawROIButton->setMinimumSize(QSize(0, 45));
        drawROIButton->setFont(font1);

        encircleButton_hLayout->addWidget(drawROIButton);


        control_vLayout->addLayout(encircleButton_hLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, -1, 5, -1);
        pixelLineEdit = new QLineEdit(layoutWidget);
        pixelLineEdit->setObjectName(QStringLiteral("pixelLineEdit"));
        pixelLineEdit->setFont(font1);

        horizontalLayout->addWidget(pixelLineEdit);

        pixelLabel = new QLabel(layoutWidget);
        pixelLabel->setObjectName(QStringLiteral("pixelLabel"));
        pixelLabel->setMaximumSize(QSize(16777215, 30));
        pixelLabel->setFont(font1);

        horizontalLayout->addWidget(pixelLabel);

        mmeterLineEdit = new QLineEdit(layoutWidget);
        mmeterLineEdit->setObjectName(QStringLiteral("mmeterLineEdit"));
        mmeterLineEdit->setFont(font1);

        horizontalLayout->addWidget(mmeterLineEdit);

        mmeterLabel = new QLabel(layoutWidget);
        mmeterLabel->setObjectName(QStringLiteral("mmeterLabel"));
        mmeterLabel->setMaximumSize(QSize(16777215, 30));
        mmeterLabel->setFont(font1);
        mmeterLabel->setLayoutDirection(Qt::LeftToRight);
        mmeterLabel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(mmeterLabel);


        control_vLayout->addLayout(horizontalLayout);

        property_hLayout = new QHBoxLayout();
        property_hLayout->setSpacing(6);
        property_hLayout->setObjectName(QStringLiteral("property_hLayout"));
        property_hLayout->setContentsMargins(5, -1, 5, -1);
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(80, 30));
        label->setFont(font1);

        property_hLayout->addWidget(label);

        propComboBox = new QComboBox(layoutWidget);
        propComboBox->setObjectName(QStringLiteral("propComboBox"));
        propComboBox->setMinimumSize(QSize(0, 30));

        property_hLayout->addWidget(propComboBox);


        control_vLayout->addLayout(property_hLayout);

        orgVideo = new QLabel(centralWidget);
        orgVideo->setObjectName(QStringLiteral("orgVideo"));
        orgVideo->setGeometry(QRect(50, 50, 509, 489));
        orgVideo->setAutoFillBackground(false);
        orgVideo->setStyleSheet(QStringLiteral(""));
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
        prop1Vis->setText(QString());
        videoDisplayerLabel->setText(QString());
        contourDisplayerLabel->setText(QString());
        cellDetectionDisplayerLabel->setText(QString());
        prop2Vis->setText(QString());
        prop1VisLabel->setText(QString());
        prop2VisLabel->setText(QString());
        roiVideo1->setText(QString());
        roiVideo2->setText(QString());
        frameLabelLeft->setText(QString());
        frameLabelRight->setText(QString());
        loadVideoButton->setText(QApplication::translate("MainWindow", "File", 0));
        playVideoButton->setText(QApplication::translate("MainWindow", "Play", 0));
        stopVideoButton->setText(QApplication::translate("MainWindow", "Stop", 0));
        checkBox_compressed->setText(QApplication::translate("MainWindow", "Compressed Cell", 0));
        checkBox_control->setText(QApplication::translate("MainWindow", "Control Cell", 0));
        dilSizeNum->setText(QString());
        blebSizeRatioLabel->setText(QApplication::translate("MainWindow", "Bleb Size Ratio:", 0));
        blkSizeLabel->setText(QApplication::translate("MainWindow", "Block Size:", 0));
        differenceLabel->setText(QApplication::translate("MainWindow", "Difference:", 0));
        blebSizeRatioNum->setText(QString());
        blkSizeNum->setText(QString());
        dilSizeLabel->setText(QApplication::translate("MainWindow", "Dilation Size:", 0));
        diffrerenceNum->setText(QString());
        drawROIButton->setText(QApplication::translate("MainWindow", "Encircle Cell", 0));
        pixelLabel->setText(QApplication::translate("MainWindow", "pixel as", 0));
        mmeterLabel->setText(QApplication::translate("MainWindow", "\316\274m", 0));
        label->setText(QApplication::translate("MainWindow", "Property", 0));
        orgVideo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
