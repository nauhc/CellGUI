#ifndef AREAVIS_H
#define AREAVIS_H

#include <QWidget>
#include <QPainter>

class AreaVis : public QWidget
{
    Q_OBJECT
public:
    explicit AreaVis(QWidget *parent = 0);
    void turnVisOn();
    void turnVisOff();
    void turnTrackOn();
    void turnTrackOff();

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool on;
    bool track;
};

#endif // AREAVIS_H
