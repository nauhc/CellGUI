#ifndef SINGLEVIEW_H
#define SINGLEVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>

#include "controller.h"
#include "encircle.h"

#include "narr.h"
#include "coord.h"

class SingleView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleView(QWidget *parent = 0);


signals:

private slots:
    //load video button
    void loadButton_pressed();
    void loadButton_released();
    void loadButton_clicked();

    void initialVideoPlayerUI(QImage img);
    void updateVideoplayerUI_(QImage img, QImage ROIimg1, QImage ROIimg2);
    void updatePropsVisUI_(floatArray property); ///*int prop1, int prop2*/
    void updateCellImg_(QImage cell, QVector<QPoint> smoothContour);
    void updateCellImg_(QImage cell);

    //play video button
    void playButton_pressed();
    void playButton_released();
    void playButton_clicked();

    //stop video button
    void stopButton_pressed();
    void stopButton_released();
    void stopButton_clicked();

private:
    QVBoxLayout *mainVLayout;

    //--- Original Video ---
    QVBoxLayout *orgVideoVLayout;
    QLabel *_orgVideo;
    QLabel *_orgVideoBgd;

    //--- ROI Video ---
    QVBoxLayout *roiVideoVLayout;
    QLabel *roiVdo1Label;
    QLabel *roiVdo2Label;
    QLabel *roiVdo1;
    QLabel *roiVdo2;
    QLabel *roiVdo1Bgd;
    QLabel *roiVdo2Bgd;

    //--- Control Panel ---
    QVBoxLayout *controlVLayout;
    // playBar
    QHBoxLayout *playBarHLayout;
    QLabel      *frmLabelLeft;
    QLabel      *frmLabelRight;
    QSlider     *playerSlider;
    // playerButton
    QHBoxLayout *playerButHLayout;
    QPushButton *loadButton;
    QPushButton *playButton;
    QPushButton *stopButton;
    // videoType
    QHBoxLayout *videoTypeHLayout;
    QComboBox   *typeComBox;
    // cellRole
    QGroupBox   *roleGroup;
    QHBoxLayout *cellRoleHLayout;
    QCheckBox   *compressedCheckBox;
    QCheckBox   *controlCheckBox;
    // prmter sliderbars
    QGroupBox   *prmterGroup;
    QGridLayout *prmtrGridLayout;
    QLabel      *diffValLabel;
    QLabel      *blkSizeValLabel;
    QLabel      *dilSizeValLabel;
    QLabel      *blebSizeValLabel;
    QSlider     *diffValSlidr;
    QSlider     *blkSizeValSlidr;
    QSlider     *dilValSlidr;
    QSlider     *blebSizeValSlidr;
    //encircle button
    QHBoxLayout *encircleButtonHLayout;
    QPushButton *drawROI;
    //scale converter
    QHBoxLayout *scaleConvertHLayout;
    QLineEdit   *mmeter;
    QLineEdit   *pixel;
    // property combobox
    QHBoxLayout *propertyHLayout;
    QComboBox   *propComBox;

    QHBoxLayout *topHLayout;

    // bottom
    QWidget     *nar_container;
    QWidget     *cod_container;
    Narr        *nar;
    Coord       *cod;
    QHBoxLayout *botHLayout;


    // controller
    Controller  *controller;
    Encircle    *encircler;
    bool        encircled_;



    // UI
    void createOrignalVideo();
    void createROIVideo();

    void createPlayerController();
    void createVideoTypeSeletionComBox();
    void createCellRoleCheckBox();
    void createPrmtrPanel();
    void createEncircler();
    void createScaleConverter();
    void createPropertySelector();

    void createNarrVis();
    void createCoordVis();

    //
    void setCanvas();

};

#endif // SINGLEVIEW_H
