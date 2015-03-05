#include "viswindow.h"
#include <QPainter>
#include <QTimer>
#include <qdebug.h>

VisWindow::VisWindow(QWidget *parent)
{
    vis = new QImage(this->size()/*QSize(512, 512)*/, QImage::Format_RGB32);
    vis->fill(Qt::black);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);
}

void VisWindow::initializeGL()
{

}

void VisWindow::render(QPainter *painter)
{
    painter->drawImage(QPoint(0,0), *vis);
}

void VisWindow::resizeGL(int w, int h)
{

}


