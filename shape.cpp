#include "shape.h"
#include <QPainter>
#include <QTimer>
#include <qdebug.h>

Shape::Shape(QObject *parent)
{
    clear();
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);

}

Shape::~Shape()
{

}

void Shape::clear()
{
    begin   = 0;
    curr    = 0;
    max     = 1;
    centroids.clear();
    blebs.clear();
    contours.clear();
}

void Shape::setBeginFrm(int beginFrame)
{
    begin = beginFrame;
    std::cout << "SHAPE VIS BEGIN frame number " << begin << std::endl;
}

void Shape::setMaxFrm(int maxFrame)
{
    max = maxFrame;
    std::cout << "SHAPE VIS MAX frame number " << max << std::endl;
}

void Shape::updateContourNBleb(QVector<Bleb> &bleb, QVector<QPoint> &smoothContour, QPoint &cent)
{
    QPolygon contour;
    for(int n = 0; n < smoothContour.size(); n++)
        contour << QPoint((smoothContour[n].x()-cent.x())*0.5, (smoothContour[n].y()-cent.y())*0.5);

    contours.push_back(contour);
    blebs.push_back(bleb);

//    std::cout << "blebs sizes load to file " << blebs.size() << std::endl;
//    for(unsigned int n = 0; n < blebs.size(); n++){
//        std::cout << "frame  " << n << std::endl;
//        for(unsigned int m = 0; m < blebs[n].size(); m++){
//            std::cout << "bleb  " << m << std::endl;
//            for(unsigned int k = 0; k < blebs[n][m].bunch_polar.size(); k++){
//                std::cout << "(r " << blebs[n][m].bunch_polar[k].r
//                          << " , theta " << blebs[n][m].bunch_polar[k].theta << ") ";
//            }
//            std::cout << std::endl;
//        }
//        std::cout << std::endl;
//    }

//    std::cout << "contours sizes load to file " << contours.size() << std::endl;
//    for(int n = 0; n < contours.size(); n++){
//        std::cout << "frame  " << n << std::endl;
//        for(int m = 0; m < contours[n].size(); m++){
//            std::cout << "(x "  << contours[n][m].x() << ", y " << contours[n][m].y() << ") ";
//        }
//        std::cout << std::endl;
//    }

        for(int m = 0; m < contours[0].size(); m++){
            std::cout << "(x "  << contours[0][m].x() << ", y " << contours[0][m].y() << ") ";
        }
        std::cout << std::endl;
}

void Shape::initializeGL()
{

}

void Shape::render(QPainter *painter)
{
    qreal halfW = this->width()/2;
    qreal halfH = this->height()/2;

    // set (0,0) to the center of the canvas
    QPointF center(halfW, halfH);

    // set the start angle to 0 o'clock;
    painter->translate(center.x(), center.y());
    painter->rotate(-90); //***x->up, y->right***
    painter->setRenderHint(QPainter::Antialiasing);


    //qDebug() << contours.size() << blebs.size();

//    QPen myPen(QColor(255,0,0));
//    myPen.setWidth(5);
//    painter->setPen(myPen);
//    painter->drawPoint(-72, -73);

    qreal size  = contours.size();
    if(size > 20){ // draw when data is valid
        // draw contours

        qreal opacity = 1/size;
        //qDebug() << size << trans;

        painter->setOpacity(opacity*10);
        QPen myPen(QColor(255,0,0));
        myPen.setWidth(1);
        painter->setPen(myPen);

        painter->drawPolyline(contours[0]);

//        for(int i = 0; i < int(size); i++){
//            //painter->drawPolygon(contours[i]);
//        }
//        painter->drawPolygon(contours[0]);



    }

}
