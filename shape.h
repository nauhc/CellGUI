#ifndef SHAPE_H
#define SHAPE_H
#include "OpenGLWindow.h"
#include "bleb.h"

class Shape : public RenderWidget
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = 0);
    ~Shape();

    void setBeginFrm(int beginFrame);
    void setMaxFrm(int maxIndex, int maxFrame);
    void updateContourNBleb(QVector<Bleb> &bleb, QVector<QPoint> &smoothContour, QPoint &cent);
    void clear();

    //void setNeedUpdate();

protected:
    void initializeGL();
    void render(QPainter *painter);
    void drawColorBar(QPainter *painter);
    //void resizeGL(int w, int h);

private:
    //bool                        needUpdate;
    qreal                       halfW, halfH; // halfWidth, halfHeight of the OpenGLWindow

    unsigned int                begin; // beggining frame index
    unsigned int                curr; // current frame index
    unsigned int                maxIndex; // total number of frame
    unsigned int                maxFrame; // total number of frame
    unsigned int                range; // how many frames to show

    QVector<QPolygon>           contours; // smoothed contours
//    QVector<QVector<QPoint> >   contours; // smoothed contours
    QVector<QVector<QPoint> >   blebPoints; // blebs

    unsigned char               *buffer;
    unsigned int                bufferSize;
    QImage                      img;

signals:


public slots:


};

#endif // SHAPE_H
