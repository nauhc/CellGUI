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

#include "narr.h"
#include "coord.h"

class SingleView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleView(QWidget *parent = 0);


signals:

public slots:

private:
    QVBoxLayout *mainVLayout;

    //--- Original Video ---
    QVBoxLayout *orgVideoVLayout;
    QLabel *_orgVideo;

    //--- ROI Video ---
    QVBoxLayout *roiVideoVLayout;
    QLabel *roiVdo1;
    QLabel *roiVdo2;

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

};

#endif // SINGLEVIEW_H
