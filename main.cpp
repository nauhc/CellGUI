#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include "narr.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon("./cell.icns"));
    w.show();

//    QGLFormat qglFormat;
//    qglFormat.setDepthBufferSize(24);
//    qglFormat.setVersion(3, 3);
//    qglFormat.setSamples(16);
//    qglFormat.setProfile(QGLFormat::CoreProfile); // CompatibilityProfile does NOT work

//    Narrative narrative(qglFormat);
//    narrative.show();

    Narr narrative;
    narrative.resize(1024, 1024);
    narrative.show();

    return a.exec();
}
