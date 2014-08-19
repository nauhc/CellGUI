#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T13:37:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET      = CellGUI
TEMPLATE    = app
ICON        = cell.icns

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
#opencv
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include/
LIBS+= -L/usr/local/lib/ \
    -lopencv_core.2.4.8 \
    -lopencv_highgui.2.4.8 \
    -lopencv_features2d.2.4.8 \
    -lopencv_imgproc.2.4.8 \
    -lopencv_video.2.4.8


SOURCES += main.cpp\
        mainwindow.cpp \
    controller.cpp \
    findcontour.cpp \
    encircle.cpp \
    areavis.cpp

HEADERS  += mainwindow.h \
    controller.h \
    findcontour.h \
    encircle.h \
    areavis.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    cell.icns
