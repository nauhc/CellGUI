#include "shape.h"
#include <QPainter>
#include <QTimer>
#include <qdebug.h>
//#include "cubehelix.h"
#include "colormap.h"

const int Shape_COLOR_START = /*30*/0;
const int Shape_COLOR_RANGE = /*90*/230;


Shape::Shape(QObject *parent)
{
    clear();

//    QTimer* timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(1);
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
//    blebs.clear();
    contours.clear();
    blebPoints.clear();
}

//void Shape::setNeedUpdate()
//{
//    needUpdate = true;
//}

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
        contour << QPoint((smoothContour[n].x()-cent.x()), (smoothContour[n].y()-cent.y()));/**0.5*/

    contours.push_back(contour);

    QVector<QPoint> points_in1frm;
    for(int k = 0; k < bleb.size(); k++){ // one bleb
        vector<polarPoint>  polarPBunch = bleb[k].bunch_polar;
        int num = polarPBunch.size();
        Point center = bleb[k].center;
        for(int l = 0; l < num; l++){ // one point
            polarPoint polarP = polarPBunch[l];
            int x = center.x + polarP.r * cos(polarP.theta);
            int y = center.y + polarP.r * sin(polarP.theta);
            points_in1frm.push_back(QPoint(x,y));
        }
    }
    blebPoints.push_back(points_in1frm);

    /*
//    qDebug() << "centroid " << cent;
//    qDebug() << "contour_org" << smoothContour;
//    qDebug() << "contour_trans " << contour;

//    std::cout << "blebs and contours sizes load to file " << blebs.size() << std::endl;
//    for(unsigned int n = 0; n < blebs.size(); n++){
//        std::cout << "frame  " << n << std::endl;
//        for(int m = 0; m < contours[n].size(); m++){
//            std::cout << "(x "  << contours[n][m].x() << ", y " << contours[n][m].y() << ") ";
//        }

////        for(unsigned int m = 0; m < blebs[n].size(); m++){
////            std::cout << "bleb  " << m << std::endl;
////            for(unsigned int k = 0; k < blebs[n][m].bunch_polar.size(); k++){
////                std::cout << "(r " << blebs[n][m].bunch_polar[k].r
////                          << " , theta " << blebs[n][m].bunch_polar[k].theta << ") ";
////            }
////            std::cout << std::endl;
////        }
//        std::cout << std::endl;
//    }
*/

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


void Shape::drawColorBar(QPainter *painter){ // (center,

    halfW = this->width()/2;
    halfH = this->height()/2;

    // *** draw coordinates of each frame ***
    int COLOR_RANGE = 180;
    // *** draw color map bar indicating time old / new ***

    painter->rotate(90); //***x->right, y->down***

    int bar_txt_y   = halfH*3/4;
    int bar_txt_h   = 20;
    int bar_txt_w   = halfW/4;
    QRect txt_old   = QRect(-halfW,           bar_txt_y, bar_txt_w, bar_txt_h);
    QRect txt_new   = QRect(halfW-bar_txt_w,  bar_txt_y, bar_txt_w, bar_txt_h);

    QPen myPen(QColor(120, 120, 118));
    painter->setPen(myPen);
    painter->drawText(txt_old, Qt::AlignRight, "Old");
    painter->drawText(txt_new, Qt::AlignLeft,  "New");


    painter->rotate(-90); //***x->up, y->right***

    int space   = halfW/8;
    int bar_h   = 10;
    int bar_len = 2*(halfW - bar_txt_w - space);
    int bar_w   = bar_len/bar_len;
    int bar_x   = -(bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2);
    int bar_y   = -(halfW - bar_txt_w - space);

    for(int n = 0; n < bar_len; n++){
        //QColor c = _mapNumToHue_(60, COLOR_RANGE, 0, bar_len, n);
        ColorMap colorMap;
        QColor c = colorMap.cubehelix(Shape_COLOR_START, Shape_COLOR_RANGE, 0, bar_len, n);
        myPen.setWidth(0);
        painter->setPen(c);
        painter->setBrush(QBrush(c));
        QRect rect(QPoint(bar_x, bar_y+bar_w*n), QSize(bar_h, bar_w));
        painter->drawRect(rect);
    }

}


void Shape::render(QPainter *painter)
{
//    if(!this->needUpdate)
//        return;

    halfW = this->width()/2;
    halfH = this->height()/2;

    // set (0,0) to the center of the canvas
    QPointF center(halfW, halfH);

    // set the start angle to 0 o'clock;
    painter->translate(center.x(), center.y());
    painter->rotate(-90); //***x->up, y->right***
    painter->setRenderHint(QPainter::Antialiasing);

    qreal size  = contours.size();
//    qreal size  = contourPoints.size();

    if(size > 1){ // draw when data is valid
        // draw contours

        qreal opacity = 1/size;
        //qDebug() << size << trans;

        QPen myPen(QColor(123, 57, 144));
        myPen.setWidth(1);
        for(int i = 0; i < int(size); i++){ // one frame
            // contours
            //painter->setOpacity(opacity*10);
            painter->setOpacity(0.005);
            painter->setPen(myPen);
            qreal scl = 0.8;
            painter->scale(scl, scl);
            painter->drawPoints(contours[i]);
            painter->scale(1/scl, 1/scl);

            // blebs
            //QColor c = _mapNumToHue_(60, 180, 0, int(size), i);
            ColorMap colorMap;
            QColor c = colorMap.cubehelix(Shape_COLOR_START, Shape_COLOR_RANGE, 0, int(size), i);
            painter->setOpacity(1);
            painter->setPen(QPen(c));


            //for(int i = 0; i < blebPoints.size(); i++){
            painter->drawPoints(blebPoints[i].data(), blebPoints[i].size());
            //}
        }
    }

    drawColorBar(painter);

//    this->needUpdate = false;

}
