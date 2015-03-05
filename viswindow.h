#ifndef VISWINDOW_H
#define VISWINDOW_H
#include "OpenGLWindow.h"
#include <QImage>

class VisWindow: public OpenGLWindow
{
    Q_OBJECT

public:
    QImage  *vis;
    VisWindow(QWidget *parent = 0);

protected:
    void initializeGL();
    void render(QPainter *painter);
    void resizeGL(int w, int h);

};

#endif // VISWINDOW_H
