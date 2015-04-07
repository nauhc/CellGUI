#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include "coord.h"
#include "cubicYFColormap.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    MainWindow w;
//    w.setWindowIcon(QIcon("./cell.icns"));
//    w.setGeometry( QStyle::alignedRect(Qt::LeftToRight,Qt::AlignCenter,
//                        w.size(), qApp->desktop()->availableGeometry()));
//    w.showFullScreen();
//    w.showMaximized();
    w.show();


    return a.exec();
}
