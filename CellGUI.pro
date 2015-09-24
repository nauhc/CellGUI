#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T13:37:36
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = CellGUI
TEMPLATE    = app
ICON        = cell.icns

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
#opencv
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.8.2/include/opencv2/
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.8.2/include/opencv/
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.8.2/include/

LIBS+= -L/usr/local/lib/ \
    -lopencv_core.2.4.8 \
    -lopencv_highgui.2.4.8 \
    -lopencv_features2d.2.4.8 \
    -lopencv_imgproc.2.4.8 \
    -lopencv_video.2.4.8 \
    -lopencv_calib3d.2.4.8 \


SOURCES += main.cpp\
        mainwindow.cpp \
    controller.cpp \
    findcontour.cpp \
    encircle.cpp \
    areavis.cpp \
    bleb.cpp \
    narr.cpp \
    OpenGLWindow.cpp \
    coord.cpp \
    multiview.cpp \
    singleview.cpp \
    shape.cpp \
    colormap.cpp \
    viswindow.cpp \
    cubicYFColormap.cpp \
    binvec.cpp

HEADERS  += mainwindow.h \
    controller.h \
    findcontour.h \
    encircle.h \
    areavis.h \
    bleb.h \
    narr.h \
    OpenGLWindow.h \
    coord.h \
    multiview.h \
    singleview.h \
    style.h \
    shape.h \
    colormap.h \
    viswindow.h \
    cubicYFColormap.h \
    binvec.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    cell.icns \
    fragmentShader.frag \
    vertexShader.vert
