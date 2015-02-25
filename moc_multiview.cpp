/****************************************************************************
** Meta object code from reading C++ file 'multiview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "multiview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MultiView_t {
    QByteArrayData data[17];
    char stringdata[239];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiView_t qt_meta_stringdata_MultiView = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 14),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 10),
QT_MOC_LITERAL(4, 37, 4),
QT_MOC_LITERAL(5, 42, 11),
QT_MOC_LITERAL(6, 54, 3),
QT_MOC_LITERAL(7, 58, 17),
QT_MOC_LITERAL(8, 76, 13),
QT_MOC_LITERAL(9, 90, 15),
QT_MOC_LITERAL(10, 106, 23),
QT_MOC_LITERAL(11, 130, 24),
QT_MOC_LITERAL(12, 155, 23),
QT_MOC_LITERAL(13, 179, 16),
QT_MOC_LITERAL(14, 196, 8),
QT_MOC_LITERAL(15, 205, 13),
QT_MOC_LITERAL(16, 219, 19)
    },
    "MultiView\0readProperties\0\0floatArray\0"
    "prop\0readCellImg\0img\0readContourNBlebs\0"
    "QVector<Bleb>\0QVector<QPoint>\0"
    "loadFilesButton_pressed\0"
    "loadFilesButton_released\0"
    "loadFilesButton_clicked\0updatePropsVisUI\0"
    "property\0updateCellImg\0updateContourNBlebs"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       5,    1,   67,    2, 0x06 /* Public */,
       7,    3,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   77,    2, 0x0a /* Public */,
      11,    0,   78,    2, 0x0a /* Public */,
      12,    0,   79,    2, 0x0a /* Public */,
      13,    1,   80,    2, 0x0a /* Public */,
      15,    1,   83,    2, 0x0a /* Public */,
      15,    2,   86,    2, 0x0a /* Public */,
      16,    3,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QImage,    6,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 9, QMetaType::QPoint,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,   14,
    QMetaType::Void, QMetaType::QImage,    2,
    QMetaType::Void, QMetaType::QImage, 0x80000000 | 9,    2,    2,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 9, QMetaType::QPoint,    2,    2,    2,

       0        // eod
};

void MultiView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultiView *_t = static_cast<MultiView *>(_o);
        switch (_id) {
        case 0: _t->readProperties((*reinterpret_cast< floatArray(*)>(_a[1]))); break;
        case 1: _t->readCellImg((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->readContourNBlebs((*reinterpret_cast< QVector<Bleb>(*)>(_a[1])),(*reinterpret_cast< QVector<QPoint>(*)>(_a[2])),(*reinterpret_cast< QPoint(*)>(_a[3]))); break;
        case 3: _t->loadFilesButton_pressed(); break;
        case 4: _t->loadFilesButton_released(); break;
        case 5: _t->loadFilesButton_clicked(); break;
        case 6: _t->updatePropsVisUI((*reinterpret_cast< floatArray(*)>(_a[1]))); break;
        case 7: _t->updateCellImg((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 8: _t->updateCellImg((*reinterpret_cast< QImage(*)>(_a[1])),(*reinterpret_cast< QVector<QPoint>(*)>(_a[2]))); break;
        case 9: _t->updateContourNBlebs((*reinterpret_cast< QVector<Bleb>(*)>(_a[1])),(*reinterpret_cast< QVector<QPoint>(*)>(_a[2])),(*reinterpret_cast< QPoint(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPoint> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPoint> >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPoint> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MultiView::*_t)(floatArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MultiView::readProperties)) {
                *result = 0;
            }
        }
        {
            typedef void (MultiView::*_t)(QImage );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MultiView::readCellImg)) {
                *result = 1;
            }
        }
        {
            typedef void (MultiView::*_t)(QVector<Bleb> , QVector<QPoint> , QPoint );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MultiView::readContourNBlebs)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject MultiView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MultiView.data,
      qt_meta_data_MultiView,  qt_static_metacall, 0, 0}
};


const QMetaObject *MultiView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MultiView.stringdata))
        return static_cast<void*>(const_cast< MultiView*>(this));
    return QWidget::qt_metacast(_clname);
}

int MultiView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MultiView::readProperties(floatArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MultiView::readCellImg(QImage _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MultiView::readContourNBlebs(QVector<Bleb> _t1, QVector<QPoint> _t2, QPoint _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
