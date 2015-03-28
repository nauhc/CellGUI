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
    void setValue(float v);
    void updateContourNBleb(QVector<Bleb> &bleb, QVector<QPoint> &smoothContour, QPoint &cent);
    void updateRto(float minR, float maxR);
    void setTempType(bool rt);
    void clear();

    //void setNeedUpdate();

protected:
    void initializeGL();
    void render(QPainter *painter);
    void drawColorBarText(QPainter *painter);
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

    float                       value; // pressure/force/offset

    unsigned char               *buffer;
    unsigned int                bufferSize;
    QImage                      img;

    float                       maxTimeRatio;
    float                       minTimeRatio;
    bool                        roomT;

    void                drawPoint(QPointF p, QColor c);
    void                drawVLine(QPointF p1, QPointF p2, QColor c, int w);
    void                fillRect(QPointF p1, QPointF p2, QColor c);
    void                drawColorBar();


signals:


public slots:


};

#endif // SHAPE_H
