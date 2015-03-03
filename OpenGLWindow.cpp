#include <QCoreApplication>
#include <QPainter>
#include <QResizeEvent>
#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
    , _updatePending(false)
    , _animating(false)
    , needUpdate(false)
    , _context(0)
    , _paintDevice(0)
{
    setSurfaceType(QWindow::OpenGLSurface);

    QSurfaceFormat format;
    format.setVersion(3, 3);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setSamples(4);
    //GLWindow win;
    setFormat(format);
}

OpenGLWindow::~OpenGLWindow()
{
}

//QSize OpenGLWindow::sizeHint() const
//{
//    return QSize(1024, 1024);
//}

void OpenGLWindow::initializeGL()
{
}

void OpenGLWindow::render()
{
    if (!_paintDevice)
        _paintDevice = new QOpenGLPaintDevice();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);     //!!
    glClearColor(1.f, 1.f, 1.f, 0.f);

    _paintDevice->setSize(/*cnt*/size());

    QPainter painter(_paintDevice);
    render(&painter);
}

void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLWindow::makeCurrent()
{
    _context->makeCurrent(this);
}

void OpenGLWindow::setNeedUpdate()
{
    needUpdate = true;
}

void OpenGLWindow::setAnimating(bool animating)
{
    _animating = animating;
    if (_animating)
        renderLater();
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        _updatePending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::resizeEvent(QResizeEvent *event)
{
    if (_context != 0)
    {
        int w = event->size().width();
        int h = event->size().height();
        makeCurrent();
        resizeGL(w, h);
        update();
    }
}

void OpenGLWindow::resizeGL(int w, int h)
{
    Q_UNUSED(w);
    Q_UNUSED(h);
}

void OpenGLWindow::renderLater()
{
    if (!_updatePending)
    {
        _updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!_context)
    {
        _context = new QOpenGLContext(this);
        _context->setFormat(requestedFormat());
        _context->create();
        needsInitialize = true;
    }

    _context->makeCurrent(this);

    if (needsInitialize)
    {
        initializeOpenGLFunctions();
        initializeGL();
        QCoreApplication::postEvent(this, new QResizeEvent(/*cnt(), cnt()*/ size(), size()));
    }

    render();

    _context->swapBuffers(this);

    if (_animating)
        renderLater();
}

void OpenGLWindow::update()
{
    renderNow();
}
