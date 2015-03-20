#ifndef NARR_H
#define NARR_H

#include "OpenGLWindow.h"

typedef std::vector<float> floatArray;

class Narr : public RenderWidget
{
    Q_OBJECT

public:
    Narr(QWidget *parent = 0, bool multv = true);
    ~Narr();
    void setBeginFrm(int beginFrame);
    void setMaxFrm(unsigned int m);
    void setPropertyType(int);
    void setValue(float v);
    void initialize();
    void updateProperty(floatArray prop, int currFrame);
    void updateCellImg(QImage &cell);
    void updateCellImg(QImage &cell, QVector<QPoint> &smoothContour); // when need the cell image contours
    void updateStage(unsigned int index);
    void clear();

protected:
    void initializeGL();
    void render(QPainter *painter);
    //void resizeGL(int w, int h);

private:
    qreal                       halfW, halfH; // halfWidth, halfHeight of the OpenGLWindow
    unsigned int                propType; // property type for displaying 0:area 1:perimeter 2:bleb
    qreal                       ringArcInnerRadius;
    qreal                       ringArcThickness;
    qreal                       propBarInnerRadius;
    qreal                       propBarThickness;
    QPointF                     center;

    QVector<unsigned int>       stage; // frame index that separates stages
    std::vector<floatArray>     propSeq;
    std::vector<float>          frameIdx;
    std::vector<float>          area; // the values of a particular feature
    std::vector<float>          perimeter; // the values of a particular feature
    std::vector<float>          blebNum; // the values of a particular feature
    std::vector<float>          blebAvgSize; // the values of a particular feature
    QVector<QPoint>             centroid; // the values of a particular feature
    QVector<QPolygon>           contours; // smoothed contours
    QVector<QImage>             cellImg; // all the cell imgs

    unsigned int                begin; // beggining frame index
    unsigned int                curr; // current frame index
    unsigned int                max; // total number of frame
    QVector<QImage>             cells; // keyframe of cells

    float                       angle; // mouse position angle
    unsigned int                mouseIndex;

    float                       value; // pressure/force/offset

    unsigned char               *bufferr;
    unsigned int                bufferSize;
    QImage                      img;

    void printAreaData();
    void drawPoint(QPointF p, QColor c);
    void drawRingArc(QPointF    center,
                     qreal      innerRadius,
                     qreal      thickness,
                     QColor     color);

    void drawCircularBarChart(QPainter *painter, std::vector<float> feature,
                              qreal innerRadius, qreal thickness,
                              qreal strtRto, QColor color);

    void drawCircularLineChart(QPainter *painter, std::vector<float> feature,
                               qreal innerRadius, qreal thickness,
                               qreal strtRto, QColor color);

    void drawCircularBarChart_fixMax(QPainter *painter,
                                     std::vector<float> feature,
                                     qreal minV, qreal maxV,
                                     qreal innerRadius, qreal thickness,
                                     QColor color);

    void drawCircularBarChart_bleb(QPainter *painter,
                                   std::vector<float> feature, qreal maxV,
                                   qreal innerRadius,
                                   qreal thickness,
                                   QColor color);

    void drawCircularLineChart_fixMax(QPainter *painter,
                               std::vector<float> feature, qreal maxV,
                               qreal innerRadius, qreal thickness,
                               QColor color);

    void mouseMoveEvent(QMouseEvent *ev);

private slots:
    void setPropType(int);

};

#endif // NARR_H
