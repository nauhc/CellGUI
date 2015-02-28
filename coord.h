#ifndef COORD_H
#define COORD_H

#include "OpenGLWindow.h"

typedef std::vector<float> floatArray;

class Coord : public OpenGLWindow
{
    Q_OBJECT

public:
    Coord(QWidget *parent = 0);
    ~Coord();

    void updateCoord(QPointF point, int currFrame);
    void getMaxFrm(unsigned int m);
    void getMaxSize(QSize s);
    void clear();

protected:
    void    initializeGL();
    void    render(QPainter *painter);
    void    resizeGL(int w, int h);
    QPointF translateCoord(QPointF p, qreal scale);
    QPointF translate_canvas2image(QPointF p);
    QPointF translate_image2canvas_center(QPointF p, bool XGreaterThanY);
    QColor  gradColor(QColor color, qreal percent);
    void    drawColorBar(QPainter *painter);

private:
    unsigned int        maxFrm;
    unsigned int        currFrm;

    QVector<QPointF>    centroid;       // the values of a particular feature
    QPointF             centroid_min;   // the minimum x,y coordinates of centroid
    QPointF             centroid_max;   // the maximum x,y coordinates of centroid
    QPointF             origin;         // the first centroid position
    QPointF             max;            // video size
    QPointF             min;            // (0,0)
    QPointF             win;            // size of ruler window
    QPointF             win_off;        // offset of ruler window
    QPointF             currCent;
    int                 cnt = 0;

    double              pixel;
    double              micMeter;
    double              micMtr_Pixel;

private slots:
    void    setPixel(QString text);
    void    setMicMeter(QString text);

};



#endif // COORD_H
