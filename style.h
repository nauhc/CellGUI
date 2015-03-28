#ifndef STYLE_H
#define STYLE_H
#include <QString>

#endif // STYLE_H

const int    MAXFRAMELEN            = 3000;

//const QString BUTTON_PRESSED        = "color:rgb(200,200,200); font: bold 16px; border-style:inset; border-width:7px; \
//                                         border-color:rgb(0,0,0); border-radius:4px; background-color:rgb(20,20,20)";
//const QString BUTTON_RELEASED_ON    = "color:rgb(255,255,255); font: bold 16px; border-style:outset; border-width:2px; \
//                                         border-color:rgb(150,150,150); border-radius:4px; background-color:rgb(38,42,43)";
//const QString BUTTON_RELEASED_OFF   = "color:rgb(80,80,80); font: bold 16px; border-style:outset; border-width:2px; \
//                                         border-color:rgb(80,80,80); border-radius:4px; background-color:rgb(38,42,43)";

const QString BUTTON_PRESSED        = "color:rgb(81,85,96); font: bold 16px; border-style:inset; border-width:2px; \
                                       border-color:rgb(186,192,206); border-radius:4px; background-color:rgb(163,203,215)";
const QString BUTTON_RELEASED_ON    = "color:rgb(82,89,99); font: bold 16px; border-style:outset; border-width:2px; \
                                       border-color:rgb(217,217,219); border-radius:4px; background-color:rgb(162,191,216)";
const QString BUTTON_RELEASED_OFF   = "color:rgb(193,194,199); font: bold 16px; border-style:outset; border-width:2px; \
                                       border-color:rgb(217,217,219); border-radius:4px; background-color:rgb(239,245,248)";
const QString FRM_LABEL             = "color:rgb(82,89,99); font:12px; background-color:rgba(0,0,0,0%) ";
const QString TRANS_BKGRD           = "background-color: rgba(0,0,0,0%);";
const QString HALFTRANS_BKGRD       = "background-color: rgba(128,128,128,80%);";
const QString FORGRD_WHITE          = "color:white;";
const QString FORGRD_BLUE           = "color:rgb(28, 120, 159);";
const QString FORGRD_GRAY           = "color:rgb(82,89,99);";
const QString FORGRD_GREEN          = "color:rgb(153, 204, 49);"; //(79, 193, 131)
const QString FORGRD_ORAGE          = "color:rgb(251, 172, 81);"; //(238, 122, 83)
const QString VIS                   = "color:rgb(239,240,244); border: 2px solid; border-color:rgb(217,217,219)"; //color:rgb(54,58,59)
const QString VIDEO_DISPLAY         = "background-color:rgb(216,222,224)";
const QString FONT20                = "font: 20px";
const QString FONT16                = "font: 16px";
const QString FONT16BLD             = "font: bold 16px";

const QString CHECKBOX              = "color:rgb(82,89,99); font-size: 14px; background-color:rgba(0,0,0,0%)";
const QString GROUPBOX              = "QGroupBox { background-color: \
                                                    qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #FFFFFF, stop: 1  #E0E0E0); \
                                                    border: 1px solid gray; border-radius: 5px; margin-top: 1ex;} \
                                                    QGroupBox::title { subcontrol-origin: margin; subcontrol-position: top center; \
                                                    padding: 0 3px; \
                                                    background-color: #FFFFFF;}";

const QString SCROLLBAR             = "QScrollBar:vertical { \
                                                                border: 1px solid #999999; \
                                                                background:white; \
                                                                width:10px; \
                                                                margin: 0px 0px 0px 0px; \
                                                            } \
                                                            QScrollBar::handle:vertical { \
                                                                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, \
                                                                stop: 0  rgba(28, 120, 159, 50%), stop: 0.5 rgba(28, 120, 159, 50%),  stop:1 rgba(28, 120, 159, 50%));\
                                                                min-height: 0px; \
                                                            } \
                                                            QScrollBar::add-line:vertical { \
                                                                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, \
                                                                stop: 0  rgba(28, 120, 159, 50%), stop: 0.5 rgba(28, 120, 159, 50%),  stop:1 rgba(28, 120, 159, 50%)); \
                                                                height: px; \
                                                                subcontrol-position: bottom; \
                                                                subcontrol-origin: margin; \
                                                            } \
                                                            QScrollBar::sub-line:vertical { \
                                                                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, \
                                                                stop: 0  rgba(28, 120, 159, 50%), stop: 0.5 rgba(28, 120, 159, 50%),  stop:1 rgba(28, 120, 159, 50%)); \
                                                                height: 0px; \
                                                                subcontrol-position: top;\
                                                                subcontrol-origin: margin;\
                                                            } \
                                        QScrollBar:horizontal { \
                                                                border: 1px solid #999999; \
                                                                background:white; \
                                                                height:10px; \
                                                                margin: 0px 0px 0px 0px; \
                                                            } \
                                                            QScrollBar::handle:horizontal { \
                                                                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, \
                                                                stop: 0  rgba(28, 120, 159, 50%), stop: 0.5 rgba(28, 120, 159, 50%),  stop:1 rgba(28, 120, 159, 50%));\
                                                                min-width: 0px; \
                                                            } \
                                                            QScrollBar::add-line:horizontal { \
                                                                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, \
                                                                stop: 0  rgba(28, 120, 159, 50%), stop: 0.5 rgba(28, 120, 159, 50%),  stop:1 rgba(28, 120, 159, 50%)); \
                                                                width: px; \
                                                                subcontrol-position: right; \
                                                                subcontrol-origin: margin; \
                                                            } \
                                                            QScrollBar::sub-line:horizontal { \
                                                                background: qlineargradient(x1:0, y1:0, x2:1, y2:0, \
                                                                stop: 0  rgba(28, 120, 159, 50%), stop: 0.5 rgba(28, 120, 159, 50%),  stop:1 rgba(28, 120, 159, 50%)); \
                                                                width: 0px; \
                                                                subcontrol-position: left;\
                                                                subcontrol-origin: margin;\
                                                            }";


#define PI 3.14159265
