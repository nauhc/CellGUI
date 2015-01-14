#ifndef NARR_H
#define NARR_H

#include "OpenGLWindow.h"

typedef std::vector<float> floatArray;

class Narr : public OpenGLWindow
{
    Q_OBJECT
public:
    Narr(QWidget *parent = 0);
    ~Narr();
    void updateProperty(floatArray prop, int currFrame);
    void updateCellImg(QImage &cell, QVector<QPoint> &smoothContour);
    void updateStage(unsigned int index);
    void getMaxFrm(unsigned int m);
    void clear();

protected:
    void initializeGL();
    void render(QPainter *painter);
    //void resizeGL(int w, int h);

private:
    QVector<unsigned int>       stage; // frame index that separates stages
    std::vector<floatArray>     propSeq;
    std::vector<float>          frameIdx;
    std::vector<float>          area; // the values of a particular feature
    std::vector<float>          perimeter; // the values of a particular feature
    std::vector<float>          blebNum; // the values of a particular feature
    QVector<QPoint>             centroid; // the values of a particular feature
    QVector<QPolygon>           contours; // smoothed contours
    QVector<QImage>             cellImg; // all the cell imgs

    unsigned int                curr; // current frame index
    unsigned int                max; // total number of frame
    QVector<QImage>             cells; // keyframe of cells

    void printAreaData();
    void drawCircularBarChart(QPainter *painter, std::vector<float> feature,
                              qreal innerRadius, qreal thickness,
                              qreal strtRto, QColor color);
    void drawCircularLineChart(QPainter *painter, std::vector<float> feature,
                               qreal innerRadius, qreal thickness,
                               qreal strtRto, QColor color);
};

#endif // NARR_H
