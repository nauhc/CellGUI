#ifndef AREAVIS_H
#define AREAVIS_H

#include <QWidget>
#include <QPainter>

class DataVis : public QWidget
{
    Q_OBJECT
public:
    explicit DataVis(QWidget *parent = 0);
    ~DataVis();

    void turnVisOn();
    void turnVisOff();
    void turnTrackOn(int fn, int f);
    void turnTrackOff();
    void updateData(int a, int perimtr, int currFrame);
    void releaseAreaVis();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int     area;
    int     startArea;
    int     perimeter;
    int     startPerimeter;
    int     currFrm;
    int     startFrm;
    float   step;
    bool    on; // if visualization is activated
    bool    track; // if tracking is activated

    int gridStepX = 20;
    int gridStepY = 20;

    QPoint      currPoint_area;
    QPolygon    polyline_area;
    QPoint      currPoint_perimeter;
    QPolygon    polyline_perimeter;

};

#endif // AREAVIS_H
