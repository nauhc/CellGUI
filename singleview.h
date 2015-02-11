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

class SingleView : public QWidget
{
    Q_OBJECT

public:
    explicit SingleView(QWidget *parent = 0);


signals:

public slots:

private:
    QVBoxLayout *mainVLayout;
    QLabel *_orgVideo;
    QVBoxLayout *roiVideoVLayout;
    QLabel *roiVdo1;
    QLabel *roiVdo2;

    QVBoxLayout *controlVLayout;
    QHBoxLayout *playBarHLayout;
    QHBoxLayout *playerButHLayout;
    QHBoxLayout *videoTypeHLayout;
    QHBoxLayout *cellRoleHLayout;
    QGridLayout *prmtrGridLayout;
    QHBoxLayout *encircleButtonHLayout;
    QHBoxLayout *scaleConvertHLayout;
    QHBoxLayout *propertyHLayout;


    QHBoxLayout *topHLayout;
    QHBoxLayout *butHLayout;


};

#endif // SINGLEVIEW_H
