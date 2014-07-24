#ifndef AREAVIS_H
#define AREAVIS_H

#include <QWidget>
#include <QPainter>

class DataVis : public QWidget
{
    Q_OBJECT
public:
    explicit DataVis(QWidget *parent = 0, QColor clr = QColor(153, 204, 49));
    ~DataVis();

    void turnVisOn();
    void turnVisOff();
    void turnTrackOn(int fn, int f);
    void turnTrackOff();
    void updateData(int v, int currFrame);
    void releaseDataVis();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int     value;
    int     startValue;
//    int     perimeter;
//    int     startPerimeter;
    int     currFrm;
    int     startFrm;
    float   step;
    bool    on; // if visualization is activated
    bool    track; // if tracking is activated

    int gridStepX = 20;
    int gridStepY = 20;

    QColor   color;
    QPoint   currPoint_value;
    QPolygon polyline_value;
//    QPoint      currPoint_perimeter;
//    QPolygon    polyline_perimeter;

};

#endif // AREAVIS_H
