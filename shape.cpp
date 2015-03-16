#include "shape.h"
#include <QPainter>
#include <QTimer>
#include <qdebug.h>
//#include "cubehelix.h"
//#include "colormap.h"
#include "cubicYFColormap.h"

const int Shape_COLOR_START = /*30*/0;
const int Shape_COLOR_RANGE = /*90*/120;


Shape::Shape(QObject *parent)
{
    clear();

//    QTimer* timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(1);
    bufferSize = width() * height() * 4;
    buffer = new unsigned char[bufferSize]();
    //    for (unsigned int i = 0; i < bufferSize; i++)
    //        buffer[i] = 255;
}

Shape::~Shape()
{
//    delete [] buffer;
}

void Shape::clear()
{
    begin   = 0;
    curr    = 0;
    max     = 1;
    range   = 5000;
//    blebs.clear();

//    contours.clear();
//    blebPoints.clear();

//    if(buffer != NULL)
//        delete[] buffer;


}

//void Shape::setNeedUpdate()
//{
//    needUpdate = true;
//}

void Shape::setBeginFrm(int beginFrame)
{
    begin = beginFrame;
    //std::cout << "SHAPE VIS BEGIN frame number " << begin << std::endl;
}

void Shape::setMaxFrm(int maxFrame)
{
    max = maxFrame;
    //std::cout << "SHAPE VIS MAX frame number " << max << std::endl;
}

void Shape::updateContourNBleb(QVector<Bleb> &bleb, QVector<QPoint> &smoothContour, QPoint &cent)
{

    qreal scl = 0.6;
//    QPolygon contour;
    for(int n = 0; n < smoothContour.size(); n++){
//        contour << QPoint((smoothContour[n].x()-cent.x()), (smoothContour[n].y()-cent.y()));/**0.5*/
        // x and y coordincates exchanged for drawing purpose
        int x = (smoothContour[n].y()-cent.y())*scl+this->width()/2;
        int y = (smoothContour[n].x()-cent.x())*scl+this->height()/2;
//        contour << QPoint(x, y);/**0.5*/
        unsigned int pixel = (y*width()+x)*4;
        if(pixel< 0 || pixel+3 >= bufferSize)
            continue;
        buffer[ pixel + 3 ] = 64;
        buffer[ pixel + 2 ] = 32;
        buffer[ pixel + 1 ] = 32;
        buffer[ pixel + 0 ] = 32;
    }

//    QVector<QPoint> contour;
//    for(int n = 0; n < smoothContour.size(); n++)
//        contour.push_back(QPoint((smoothContour[n].y()-cent.y()), (smoothContour[n].x()-cent.x())));/**0.5*/
//    //qDebug() << contour;

//    contours.push_back(contour);

    if (curr > range) curr = range;
    CubicYFColorMap colormap;
    QColor c = colormap.cubicYFmap(Shape_COLOR_START, Shape_COLOR_RANGE, 0, range, curr); // 5000 !!!!!

//    QVector<QPoint> points_in1frm;
    for(int k = 0; k < bleb.size(); k++){ // one bleb
        vector<polarPoint>  polarPBunch = bleb[k].bunch_polar;
        int num = polarPBunch.size();
        Point center = bleb[k].center;
        for(int l = 0; l < num; l++){ // one point
            polarPoint polarP = polarPBunch[l];
//            int x = center.x + polarP.r * cos(polarP.theta);
//            int y = center.y + polarP.r * sin(polarP.theta);
            int x = (center.x + polarP.r * cos(polarP.theta))+this->width()/2;
            int y = (center.y + polarP.r * sin(polarP.theta))+this->height()/2;
//            points_in1frm.push_back(QPoint(x,y));
            unsigned int pixel = (y*width()+x)*4;
            if(pixel< 0 || pixel+3 >= bufferSize)
                continue;
            buffer[ pixel + 3] = c.alpha();
            buffer[ pixel + 2] = c.red();
            buffer[ pixel + 1] = c.green();
            buffer[ pixel + 0] = c.blue();
        }
    }
//    blebPoints.push_back(points_in1frm);

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



    curr++; // local counter
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


void Shape::drawColorBar(QPainter *painter){ //

    halfW = this->width()/2;
    halfH = this->height()/2;

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
        CubicYFColorMap colorMap;
        QColor c = colorMap.cubicYFmap(Shape_COLOR_START, Shape_COLOR_RANGE, 0, bar_len, n);
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

    // clean canvas
    painter->eraseRect(0, 0, width(), height());
    painter->setRenderHint(QPainter::Antialiasing);

    img = QImage(buffer, width(), height(), QImage::Format_ARGB32);
    painter->drawImage(0, 0, img);

    halfW = this->width()/2;
    halfH = this->height()/2;

    // set (0,0) to the center of the canvas
    QPointF center(halfW, halfH);

    painter->translate(center.x(), center.y());
    painter->rotate(-90); //***x->up, y->right***
    drawColorBar(painter);

//    this->needUpdate = false;

}
