#ifndef BINVEC_H
#define BINVEC_H
#include "OpenGLWindow.h"

class BinVec : public RenderWidget
{
    Q_OBJECT
public:
    explicit BinVec(QObject *parent = 0);
    ~BinVec();

    void setBeginFrm(int beginFrame);
    void setMaxFrm(int maxIndex, int maxFrame);
    void setValue(QVector<float> v);
    void updateRto(float minR, float maxR);
    void setTempType(bool rt);

    void updateBinVec(QVector<float> vec);

    void clear();

protected:
    void initializeGL();
    void render(QPainter *painter);
    void drawColorBarText(QPainter *painter);
    //void resizeGL(int w, int h);

private:
    unsigned int                begin; // beggining frame index
    unsigned int                curr; // current frame index
    unsigned int                maxIndex; // total number of frame
    unsigned int                maxFrame; // total number of frame
    unsigned int                range; // how many frames to show

    QVector<float>              value; // pressure/force/offset


    unsigned char               *buffer;
    unsigned int                bufferSize;
    QImage                      img;

    float                       maxTimeRatio;
    float                       minTimeRatio;
    bool                        roomT;

    void                        drawPoint(QPointF p, QColor c);
    void                        drawVLine(QPointF p1, QPointF p2, QColor c, int w);
    void                        fillRect(QPointF p1, QPointF p2, QColor c);
    void                        drawColorBar();
};

#endif // BINVEC_H
