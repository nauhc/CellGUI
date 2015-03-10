#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QWidget>
#include <QPainter>

//class OpenGLWindow : public QWindow, protected QOpenGLFunctions
//{
//    Q_OBJECT
//public:
//    explicit OpenGLWindow(QWindow *parent = 0);
//    ~OpenGLWindow();

//    //QSize   sizeHint() const;
//    void setAnimating(bool animating);
//    void makeCurrent();
//    void setNeedUpdate();

//protected:
//    bool event(QEvent *event);
//    void exposeEvent(QExposeEvent *event);
//    void resizeEvent(QResizeEvent *event);

//    virtual void initializeGL();
//    virtual void render();
//    virtual void render(QPainter *painter);
//    virtual void resizeGL(int w, int h);

//    bool needUpdate;

//private:
//    bool _updatePending;
//    bool _animating;

//    QOpenGLContext *_context;
//    QOpenGLPaintDevice *_paintDevice;

//public slots:
//    void renderLater();
//    void renderNow();
//    void update();
//};

class OpenGLWindow : public QWidget
{
    Q_OBJECT
protected:
    virtual void render(QPainter *painter) = 0;
    virtual void paintEvent(QPaintEvent *e) { QPainter painter(this); render(&painter); }
};

#endif // OPENGLWINDOW_H
