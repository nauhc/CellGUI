#include "encircle.h"
#include "QPainter"
#include "QMouseEvent"
#include "math.h"
#include "qdebug.h"
#include <iostream>

Encircle::Encircle(QWidget *parent) : QWidget(parent)
{
    std::cout << "encircler initialized.\n" << std::endl;
    penWidth = 3;
    penColor = QColor(153, 204, 49, 200);
    encircling = false;
    encircleMode = false;
    image = QImage(QSize(500, 500), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
}

Encircle::~Encircle(){
    std::cout << "Encircler deleted." << std::endl;
}

void Encircle::turnOnEncircleMode()
{
    encircleMode = true;
//    std::cout << "encircle mode: on" << std::endl;
}

void Encircle::turnOffEncircleMode()
{
    encircleMode = false;
//    std::cout << "encircle mode: off" << std::endl;
}

void Encircle::getRegion(QVector<QPoint> &circle)
{
    circle = points;
}

void Encircle::clearCircle()
{
    points.clear();
    encircleMode = false;
    encircling = false;
}


void Encircle::mousePressEvent(QMouseEvent *event)
{
    if (encircleMode && event->button() == Qt::LeftButton) {
//        qDebug() << 'mouse pressed';
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
//    qDebug() << 'mouse moved';
    if(encircleMode && (event->buttons() & Qt::LeftButton) && encircling){
        currPoint = event->pos();
        drawLineTo(currPoint);
        lastPoint = currPoint;
        points.push_back(currPoint);
    }
}

void Encircle::mouseReleaseEvent(QMouseEvent *event)
{
    if (encircleMode && event->button() == Qt::LeftButton && encircling) {
        currPoint = event->pos();
        drawLineTo(currPoint);
        encircling = false;
        drawLineTo(startPoint);
    }
//    for(int i = 0; i < points.size(); i++)
//        qDebug() << points[i];
}

void Encircle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    if(!encircleMode){
        image.fill(Qt::transparent);
    }
    painter.drawImage(dirtyRect, image, dirtyRect);

}

void Encircle::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(penColor, penWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    lastPoint = endPoint;
    this->update();
}


