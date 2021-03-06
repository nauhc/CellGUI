#ifndef ENCIRCLE_H
#define ENCIRCLE_H
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QLabel>
#include <vector>

//class Encircle : public QWidget
class Encircle : public QWidget
{
    Q_OBJECT
public:
    //explicit Encircle(QWidget *parent = 0);
    explicit Encircle(QWidget *parent = 0);
    ~Encircle();
    bool isEncircled() const { return encircleMode; }
    void turnOnEncircleMode();
    void turnOffEncircleMode();
    void getRegion(QVector<QPoint> &circle);
    void clearCircle();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool    encircleMode; // when the encicling is done, it changes to true
    bool    encircling; // when the encicling is started, it changes to true
    int     penWidth;
    QColor  penColor;
    QImage  image;

    QPoint  startPoint;
    QPoint  lastPoint;
    QPoint  currPoint;
    QVector<QPoint> points;

    void drawLineTo(const QPoint &endPoint);

};


#endif // ENCIRCLE_H
