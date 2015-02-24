#ifndef MULTIVIEW_H
#define MULTIVIEW_H

#include <QWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "style.h"
#include "narr.h"
#include "coord.h"

class MultiView : public QWidget
{
    Q_OBJECT
public:
    explicit MultiView(QWidget *parent = 0);

signals:
    void readProperties(floatArray prop);
    void readCellImg(QImage img);

public slots:
    // load files button
    void loadFilesButton_pressed();
    void loadFilesButton_released();
    void loadFilesButton_clicked();
    void updatePropsVisUI(floatArray property);
    void updateCellImg(QImage);
    void updateCellImg(QImage, QVector<QPoint> smoothContour);

private:
//    QVector<Narr>     nar_list;
//    QVector<Coord>    cod_list;
//    QVector<QString>  file_list;

    QWidget                 *nar_container1, *nar_container2;
    QWidget                 *cod_container1, *cod_container2;
    Narr                    *nar_tmp1, *nar_tmp2;
    Coord                   *cod_tmp1, *cod_tmp2;

    std::vector<floatArray>  cellData;
    QString                 *dataFilename;

    QVBoxLayout *mainVLayout;
    QGridLayout *visGLayout;

    QPushButton *loadFilesButton;

    void    createLoadFilesButton();
    bool    readDataFile();
    QImage  readImgFile(QString fp, unsigned int idx);

    void createNarVis();
    void createCodVis();
    void clearAll();
};

#endif // MULTIVIEW_H
