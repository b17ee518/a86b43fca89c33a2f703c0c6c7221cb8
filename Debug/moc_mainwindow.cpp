/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[24];
    char stringdata[304];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 8),
QT_MOC_LITERAL(2, 20, 0),
QT_MOC_LITERAL(3, 21, 12),
QT_MOC_LITERAL(4, 34, 11),
QT_MOC_LITERAL(5, 46, 12),
QT_MOC_LITERAL(6, 59, 12),
QT_MOC_LITERAL(7, 72, 8),
QT_MOC_LITERAL(8, 81, 1),
QT_MOC_LITERAL(9, 83, 9),
QT_MOC_LITERAL(10, 93, 25),
QT_MOC_LITERAL(11, 119, 8),
QT_MOC_LITERAL(12, 128, 29),
QT_MOC_LITERAL(13, 158, 9),
QT_MOC_LITERAL(14, 168, 14),
QT_MOC_LITERAL(15, 183, 18),
QT_MOC_LITERAL(16, 202, 21),
QT_MOC_LITERAL(17, 224, 28),
QT_MOC_LITERAL(18, 253, 7),
QT_MOC_LITERAL(19, 261, 20),
QT_MOC_LITERAL(20, 282, 4),
QT_MOC_LITERAL(21, 287, 6),
QT_MOC_LITERAL(22, 294, 4),
QT_MOC_LITERAL(23, 299, 4)
    },
    "MainWindow\0sigParse\0\0PathAndQuery\0"
    "requestBody\0responseBody\0slotActivate\0"
    "QWidget*\0w\0bActivate\0slotToggleRestoreMinimize\0"
    "bRestore\0slotSelfToggleRestoreMinimize\0"
    "slotParse\0slotSoundEnded\0on_pbClose_clicked\0"
    "on_pbMinimize_clicked\0"
    "on_pbCheckTrasparent_toggled\0checked\0"
    "slotWebViewException\0code\0source\0desc\0"
    "help"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   71,    2, 0x0a /* Public */,
      10,    1,   76,    2, 0x0a /* Public */,
      12,    1,   79,    2, 0x0a /* Public */,
      13,    3,   82,    2, 0x0a /* Public */,
      14,    0,   89,    2, 0x0a /* Public */,
      15,    0,   90,    2, 0x08 /* Private */,
      16,    0,   91,    2, 0x08 /* Private */,
      17,    1,   92,    2, 0x08 /* Private */,
      19,    4,   95,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7, QMetaType::Bool,    8,    9,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString,   20,   21,   22,   23,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->sigParse((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: _t->slotActivate((*reinterpret_cast< QWidget*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->slotToggleRestoreMinimize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->slotSelfToggleRestoreMinimize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slotParse((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 5: _t->slotSoundEnded(); break;
        case 6: _t->on_pbClose_clicked(); break;
        case 7: _t->on_pbMinimize_clicked(); break;
        case 8: _t->on_pbCheckTrasparent_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->slotWebViewException((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MainWindow::*_t)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MainWindow::sigParse)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &MainWindowBase::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return MainWindowBase::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MainWindowBase::qt_metacall(_c, _id, _a);
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
void MainWindow::sigParse(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
