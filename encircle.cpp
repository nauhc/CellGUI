#include "encircle.h"
#include "QPainter"
#include "QMouseEvent"
#include "math.h"
#include "qdebug.h"

Encircle::Encircle(bool enabled, QWidget *parent) : QWidget(parent)
//Encircle::Encircle(QWidget *parent) : (parent)*/
{
    qDebug() << "initialize a new encircler.";
    penWidth = 3;
    penColor = QColor(0, 225, 255, 127);
    encircling = false;
    encircleMode = enabled;
    image = QImage(QSize(500, 500), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    //image.fill(QColor(qRgba(0, 0, 255, 127)));
}

Encircle::~Encircle(){
}

void Encircle::setEncircle(bool mode)
{
    encircleMode = mode;
}

void Encircle::getRegion(QVector<QPoint> &circle)
{
    circle = points;
}


void Encircle::clearCircle()
{
    points.clear();
}

void Encircle::saveCircle()
{

}

void Encircle::mousePressEvent(QMouseEvent *event)
{
    if (encircleMode && event->button() == Qt::LeftButton) {
        //qDebug() << 'mouse pressed';
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
    if(encircleMode && (event->buttons() & Qt::LeftButton) && encircling){
        //qDebug() << 'mouse moved';
        currPoint = event->pos();
        drawLineTo(currPoint);
        lastPoint = currPoint;
        points.push_back(currPoint);
    }
}

void Encircle::mouseReleaseEvent(QMouseEvent *event)
{
    if (encircleMode && event->button() == Qt::LeftButton && encircling) {
        //qDebug() << 'mouse released';
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

//void Encircle::clearCircle(){
//    image.fill(QColor(qRgba(0, 0, 0, 255)));
//}

