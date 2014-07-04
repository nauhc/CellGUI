#include "encircle.h"
#include "QPainter"
#include "QMouseEvent"
#include "math.h"

Encircle::Encircle(QWidget *parent) : QWidget(parent)
//Encircle::Encircle(QWidget *parent) : (parent)*/
{
    penWidth = 3;
    penColor = QColor(0, 225, 255, 127);
    encircling = false;
    encircled = false;
    image = QImage(QSize(500, 500), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    //image.fill(QColor(qRgba(0, 0, 255, 127)));
    pixmap = new QPixmap(this->width(), this->height());
    //pixmap->fill(Qt::/*transparent*/white);

}

Encircle::~Encircle()
{
    delete pixmap;
}

void Encircle::setCirclingArea(QRect rect)
{

}

void Encircle::clearCircle()
{

}

void Encircle::saveCircle()
{

}

void Encircle::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        currPoint = event->pos();
        if(points.size() == 0){
            startPoint = currPoint;
        }
        lastPoint = currPoint;
        encircling = true;
        points.push_back(currPoint);
    }

}

void Encircle::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && encircling){
        currPoint = event->pos();
        drawLineTo(currPoint);
        lastPoint = currPoint;
        points.push_back(currPoint);
    }
}

void Encircle::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && encircling) {
        currPoint = event->pos();
        drawLineTo(currPoint);
        encircling = false;
        drawLineTo(startPoint);
    }
    points.clear();
}

void Encircle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
//    painter.drawRect(15, 15, 200, 200);
//    painter.fillRect(this->rect(), QBrush(*pixmap));

}

void Encircle::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    lastPoint = endPoint;
}

//void Encircle::clearCircle(){
//    image.fill(QColor(qRgba(0, 0, 0, 255)));
//}

