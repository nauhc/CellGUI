#ifndef SHAPE_H
#define SHAPE_H
#include "OpenGLWindow.h"
#include "bleb.h"

class Shape : public OpenGLWindow
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = 0);
    ~Shape();

    void setBeginFrm(int beginFrame);
    void setMaxFrm(int maxFrame);
    void updateContourNBleb(QVector<Bleb> &bleb, QVector<QPoint> &smoothContour, QPoint &cent);
    void clear();

protected:
    void initializeGL();
    void render(QPainter *painter);
    //void resizeGL(int w, int h);

private:
    qreal                       halfW, halfH; // halfWidth, halfHeight of the OpenGLWindow

    unsigned int                begin; // beggining frame index
    unsigned int                curr; // current frame index
    unsigned int                max; // total number of frame

    QVector<QPoint>             centroids; // the values of a particular feature
    QVector<QPolygon>           contours; // smoothed contours
    QVector<QVector<Bleb> >     blebs; // blebs

signals:


public slots:


};

#endif // SHAPE_H
