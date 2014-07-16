#ifndef AREAVIS_H
#define AREAVIS_H

#include <QWidget>
#include <QPainter>

class AreaVis : public QWidget
{
    Q_OBJECT
public:
    explicit AreaVis(QWidget *parent = 0);
    ~AreaVis();

    void turnVisOn();
    void turnVisOff();
    void turnTrackOn(int fn, int f);
    void turnTrackOff();
    void updateArea(int a, int currFrame);
    //void clearLineChart();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int     area;
    int     currFrm;
    int     startArea;
    int     startFrm;
    float   step;
    bool    on;
    bool    track;

    int gridStepX = 20;
    int gridStepY = 20;

    QPoint  currPoint;
    QPolygon polyline;

};

#endif // AREAVIS_H
