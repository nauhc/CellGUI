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
        contour << QPoint((smoothContour[n].x()-bleb[n].center.x), (smoothContour[n].y()-bleb[n].center.y));/**0.5*/

    contours.push_back(contour);
    blebs.push_back(bleb);

    /*
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
//    } */

}

void Shape::initializeGL()
{

}

QColor _mapNumToHue_(int start, int range, int min, int max, int v){
    // start:   starting hue (0-360)
    // end:     ending hue (0-360)
    // min:     min value
    // max:     max value
    // v:       value
    QColor color;
    color.setHsv(start+range*v/(max-min), 255, 200);
    return color;
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

    qreal size  = contours.size();
    if(size > 20){ // draw when data is valid
        // draw contours

        qreal opacity = 1/size;
        //qDebug() << size << trans;

        QPen myPen(QColor(0,0,0));
        myPen.setWidth(1);
        for(int i = 0; i < int(size); i++){ // one frame
            // contours
            painter->setOpacity(opacity*10);
            painter->setPen(myPen);
            for(int j = 0; j < contours[i].size(); j++) // one contour
                painter->drawPoint(contours[i][j]);

            // blebs
            QColor c = _mapNumToHue_(60, 300, 0, int(size), i);
            painter->setOpacity(1);
            painter->setPen(QPen(c));
            for(int j = 0; j < blebs[i].size(); j++){ // one set of blebs
                for(int k = 0; k < blebs[i][j].size; k++){ // one bleb
                    polarPoint  polarP = blebs[i][j].bunch_polar[k];
                    Point       center = blebs[i][j].center;
                    int x = center.x + polarP.r * cos(polarP.theta);
                    int y = center.y + polarP.r * sin(polarP.theta);
                    painter->drawPoint(QPoint(x, y));
                }
            }
        }



    }

}
