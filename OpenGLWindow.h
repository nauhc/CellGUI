#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    //QSize   sizeHint() const;
    void setAnimating(bool animating);
    void makeCurrent();

protected:
    bool event(QEvent *event);
    void exposeEvent(QExposeEvent *event);
    void resizeEvent(QResizeEvent *event);

    virtual void initializeGL();
    virtual void render();
    virtual void render(QPainter *painter);
    virtual void resizeGL(int w, int h);

private:
    bool _updatePending;
    bool _animating;

    QOpenGLContext *_context;
    QOpenGLPaintDevice *_paintDevice;

public slots:
    void renderLater();
    void renderNow();
    void update();
};

#endif // OPENGLWINDOW_H