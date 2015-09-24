#include "binvec.h"
#include "cubicYFColormap.h"

const int BINCNT = 45;

const int Vec_COLOR_START = 0;
const int Vec_COLOR_RANGE = 120;

BinVec::BinVec(QObject *parent)
{
    clear();

    bufferSize = width() * height() * 4;
    buffer = new unsigned char[bufferSize]();
    for (unsigned int i = 0; i < bufferSize; i++)
    buffer[i] = 255;
}

BinVec::~BinVec()
{

}

void BinVec::clear()
{
    begin   = 0;
    curr    = 0;
    maxIndex     = 1;
    maxTimeRatio = 1.0;
}


void BinVec::setBeginFrm(int beginFrame)
{
    begin = beginFrame;
    //std::cout << "BIN VEC VIS BEGIN frame number " << begin << std::endl;
}

void BinVec::setMaxFrm(int maxIndex, int maxFrame)
{
    maxIndex = maxIdx;
    maxFrame = maxFrm;
    //std::cout << "BIN VEC MAX frame number " << max << std::endl;
}

void BinVec::setValue(QVector<float> v)
{
    value = v;
}

void BinVec::updateRto(float minR, float maxR)
{
    maxTimeRatio = maxR;
    minTimeRatio = minR;
    //qDebug() << maxTimeRatio;
//    update();
    drawColorBar();
}

void BinVec::setTempType(bool rt)
{
    roomT = rt;
}

void BinVec::updateBinVec(QVector<float> vec)
{
    int offset = 10;
    range = 5000 - begin;
    if (curr > range) curr = range;

    int barW = max((width() - 2 * offset) / range, 1);
    for(int i = 0; i < range; i++){ // each bar (horizental)

        for (int j = 0; j < BINCNT; j++){ // each bin (vertical)

            CubicYFColorMap colorMap;
            int maxR = 400;
            QColor c = colorMap.cubicYFmap(Vec_COLOR_START, Vec_COLOR_RANGE, 0, maxR, vec(j));

            for (int w = i*barW; w < (i+1)*barW; w++){
                unsigned int pixel = (j*width()+w)*4;
                if(pixel< 0 || pixel+3 >= bufferSize)
                    continue;
                buffer[ pixel + 3 ] = c.alpha();
                buffer[ pixel + 2 ] = c.red();
                buffer[ pixel + 1 ] = c.green();
                buffer[ pixel + 0 ] = c.blue();
            }

        }
    }

    curr++;

}


void BinVec::initializeGL()
{

}

void BinVec::drawColorBarText(QPainter *painter)
{
    int bar_txt_y   = height()*6/7;
    int bar_txt_h   = 20;
    int bar_txt_w   = width()/8;
    QRect txt_old   = QRect(0,                      bar_txt_y, bar_txt_w, bar_txt_h);
    QRect txt_new   = QRect(width()-bar_txt_w-10,   bar_txt_y, bar_txt_w, bar_txt_h);

    QPen myPen(QColor(120, 120, 118));
    painter->setPen(myPen);
    painter->drawText(txt_old, Qt::AlignRight, "0");
    painter->drawText(txt_new, Qt::AlignLeft,  QString::number(maxIndex));
}

void BinVec::drawPoint(QPointF p, QColor c)
{
    unsigned int px = (int(p.y()) * width() + int(p.x())) * 4;
    if(px > 0 && px+3 < bufferSize){
/*
        buffer[ px + 3] = c.alpha();
        buffer[ px + 2] = c.red();
        buffer[ px + 1] = c.green();
        buffer[ px + 0] = c.blue(); */
        int tmp = (255 - c.alpha());
        buffer[px + 2] = (c.red()*c.alpha() + buffer[px + 2]*tmp)/255;
        buffer[px + 1] = (c.green()*c.alpha() + buffer[px + 1]*tmp)/255;
        buffer[px + 0] = (c.blue()*c.alpha() + buffer[px + 0]*tmp)/255;
        buffer[px + 3] = min(c.alpha() + buffer[px + 3], 255);

    }
}

void BinVec::drawVLine(QPointF p1, QPointF p2, QColor c, int w)
{
    //int     line_w = 2;
    float   y1, y2;
    if(p1.y() < p2.y()){
        y1 = p1.y();
        y2 = p2.y();
    }
    else{
        y2 = p1.y();
        y1 = p2.y();
    }

    for (int y = y1; y < y2; y++){
        for (int x = 0; x < w; x++)
            drawPoint(QPointF(p1.x()+x,y), c);
    }
}

void BinVec::fillRect(QPointF p1, QPointF p2, QColor c)
{
    for (int x = p1.x(); x < p2.x(); x++){
        QPointF up = QPointF(x, p1.y());
        QPointF dn = QPointF(x, p2.y());
        drawVLine(up, dn, c, 1);
    }
}

void BinVec::drawColorBar()
{
    // *** draw color map bar indicating time old / new ***
    // ***x->right, y->down***
    int space       = width()/16;
    int bar_txt_y   = (height()-space)*7/8;
    int bar_txt_h   = 20;
    int bar_txt_w   = width()/8;
    //float rto   = float(maxIndex)/float(maxFrame) >= 1.0 ? 1.0 : float(maxIndex)/float(maxFrame);
    float rto   = float(maxIndex)/5000. >= 1.0 ? 1.0 : float(maxIndex)/5000.;
    int bar_h   = /*bar_txt_h*/10;
    int bar_len = width() - 2*(bar_txt_w + space)/**rto*/;

    int bar1frm_w   = bar_len/(maxIndex - begin) < 1 ? 1: bar_len/(maxIndex - begin);

    //int bar_x   = -(bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2);
    //int bar_y   = -(halfW - bar_txt_w - space);
    int bar_x   = (width() - bar_len)/2;
    int bar_y   = bar_txt_y + (bar_txt_h-bar_h+bar_txt_h)/2;

//    QRect rect_empty = QRect(QPoint(bar_x+bar_len*rto, bar_y), QSize( bar_len*(1.0-rto), bar_h+1));
    QPointF empty_topleft  = QPoint(bar_x+bar_len*rto, bar_y);
    QPointF empty_botright = QPoint(bar_x+bar_len*rto + bar_len*(1.0-rto), bar_y+bar_h+1);
    fillRect(empty_topleft, empty_botright, QColor(220, 220, 220, 64));

    for(int n = 0; n < bar_len*rto; n+= bar1frm_w){
        CubicYFColorMap colorMap;
        QColor c = colorMap.cubicYFmap(Shape_COLOR_START, Shape_COLOR_RANGE, 0, bar_len, n);
        //QRect rect(QPoint(bar_x+n, bar_y), QSize(1, bar_h));
        drawVLine(QPoint(bar_x+n, bar_y), QPoint(bar_x+n, bar_y+bar_h), c, bar1frm_w);
    }

//    float   posRto = float(maxFrame)/5000. >= 1.0 ? 1.0 : float(maxFrame)/5000.;
    QColor posColor = QColor(128, 128, 128);
//    drawVLine(QPoint(bar_x+bar_len*posRto, bar_y-2), QPoint(bar_x+bar_len*posRto, bar_y+bar_h+2), posColor, 1);
    drawVLine(QPoint(bar_x+bar_len*maxTimeRatio, bar_y-2), QPoint(bar_x+bar_len*maxTimeRatio, bar_y+bar_h+2), posColor, 1);
    drawVLine(QPoint(bar_x+bar_len*minTimeRatio, bar_y-2), QPoint(bar_x+bar_len*minTimeRatio, bar_y+bar_h+2), posColor, 1);

}

void BinVec::render(QPainter *painter)
{
    painter->eraseRect(QRect(0, 0, width(), height()));

    img = QImage(buffer, width(), height(), QImage::Format_ARGB32);
    painter->drawImage(0, 0, img);

    // draw Value
    if(roomT) // 25 - blue
        painter->setPen(QColor(0, 0, 128));
    else // 37 - red
        painter->setPen(QColor(128, 0, 0));
    for (unsigned int k = 0; k < value.size(); k++)
        painter->drawText(width() - 70, 30+k*15, 60, 20, Qt::AlignLeft, QString::number(value[k], 'f', 2));

    drawColorBarText(painter);
}
