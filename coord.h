#ifndef COORD_H
#define COORD_H

#include "OpenGLWindow.h"

class Coord : public OpenGLWindow
{
    Q_OBJECT

public:
    Coord(QWidget *parent = 0);
    ~Coord();

protected:
    void initializeGL();
    void render(QPainter *painter);
    void resizeGL(int w, int h);
    QPointF translateCoord(QPointF p);
    QColor gradColor(QColor color, qreal percent);

private:
    QVector<QPointF>    centroid; // the values of a particular feature
    QPointF             max;
    QPointF             min;
    QPointF             avg;
    int                 cnt;

};

#endif // COORD_H
