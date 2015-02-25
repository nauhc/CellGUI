#ifndef SHAPE_H
#define SHAPE_H
#include "OpenGLWindow.h"

class Shape : public OpenGLWindow
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = 0);

protected:
    void initializeGL();
    void render(QPainter *painter);
    //void resizeGL(int w, int h);

private:


signals:


public slots:


};

#endif // SHAPE_H
