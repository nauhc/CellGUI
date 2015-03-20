#ifndef COORD_H
#define COORD_H

#include "OpenGLWindow.h"

typedef std::vector<float> floatArray;

class Coord : public RenderWidget
{
    Q_OBJECT

public:
    Coord(QWidget *parent = 0, bool multv = true);
    ~Coord();

    void updateCoord(QPointF point, int currFrame);
    void setMaxFrm(unsigned int im, unsigned int fm);
    void setBeginFrm(unsigned int m);
    void setMaxSize(QSize s);
    void setValue(float v);
    void clear();

protected:
    void    initializeGL();
    void    render(QPainter *painter);
    void    resizeGL(int w, int h);
    QPointF translateCoord(QPointF p, qreal scale);
    QPointF translate_canvas2image(QPointF p);
    QPointF translate_image2canvas_center(QPointF p, bool XGreaterThanY);
    QPointF translate_image2canvas_center(QPointF p, qreal ratio);
    QColor  gradColor(QColor color, qreal percent);
    void    drawColorBarText(QPainter *painter);

private:
    unsigned int        maxFrm;
    unsigned int        maxIdx;
    unsigned int        minFrm;
    unsigned int        currFrm;
    //unsigned int        range;

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
    qreal               coordScale;

    float               value; // pressure/force/offset

    double              pixel;
    double              micMeter;
    double              micMtr_Pixel;

    unsigned char       *buffer;
    unsigned int        bufferSize;
    QImage              img;

    void                drawPoint(QPointF p, QColor c);
    void                drawHLine(QPointF p1, QPointF p2, QColor c, int w);
    void                drawVLine(QPointF p1, QPointF p2, QColor c, int w);
    void                drawRuler();
    void                fillRect(QPointF p1, QPointF p2, QColor c);
    void                drawColorBar();

private slots:
    void    setPixel(QString text);
    void    setMicMeter(QString text);
    void    setScale(qreal scale);

};



#endif // COORD_H
