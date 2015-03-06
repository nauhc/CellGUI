/****************************************************************************
** Meta object code from reading C++ file 'multiview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "multiview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multiview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MultiView_t {
    QByteArrayData data[6];
    char stringdata[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiView_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiView_t qt_meta_stringdata_MultiView = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MultiView"
QT_MOC_LITERAL(1, 10, 23), // "loadFilesButton_pressed"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 24), // "loadFilesButton_released"
QT_MOC_LITERAL(4, 60, 23), // "loadFilesButton_clicked"
QT_MOC_LITERAL(5, 84, 11) // "viewByProp1"

    },
    "MultiView\0loadFilesButton_pressed\0\0"
    "loadFilesButton_released\0"
    "loadFilesButton_clicked\0viewByProp1"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MultiView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MultiView *_t = static_cast<MultiView *>(_o);
        switch (_id) {
        case 0: _t->loadFilesButton_pressed(); break;
        case 1: _t->loadFilesButton_released(); break;
        case 2: _t->loadFilesButton_clicked(); break;
        case 3: _t->viewByProp1(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MultiView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MultiView.data,
      qt_meta_data_MultiView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MultiView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
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
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
