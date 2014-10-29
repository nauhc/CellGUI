#ifndef NARR_H
#define NARR_H

#include "OpenGLWindow.h"

class Narr : public OpenGLWindow
{
    Q_OBJECT
public:
    Narr();

protected:
    void initializeGL();
    void render(QPainter *painter);
    void resizeGL(int w, int h);

};

#endif // NARR_H
