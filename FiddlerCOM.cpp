/****************************************************************************
**
** Metadata for FiddlerCOM generated by dumpcpp from type library
** e:\QTProject_GIT\a86b43fca89c33a2f703c0c6c7221cb8\FiddlerCOM\FiddlerCOM\bin\Release\FiddlerCOM.tlb
**
****************************************************************************/

#define QAX_DUMPCPP_FIDDLERCOM_NOINLINES
#include "fiddlercom.h"

using namespace FiddlerCOM;

struct qt_meta_stringdata_all_t {
    QByteArrayData data[35];
    char stringdata0[401];
};
#define QT_MOC_LITERAL(idx, ofs, len, table) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_all_t, stringdata##table) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_all_t qt_meta_stringdata_all = {
    {
QT_MOC_LITERAL(0, 0, 28, 0),
QT_MOC_LITERAL(1, 29, 9, 0),
QT_MOC_LITERAL(2, 39, 0, 0),
QT_MOC_LITERAL(3, 40, 4, 0),
QT_MOC_LITERAL(4, 45, 6, 0),
QT_MOC_LITERAL(5, 52, 4, 0),
QT_MOC_LITERAL(6, 57, 4, 0),
QT_MOC_LITERAL(7, 62, 15, 0),
QT_MOC_LITERAL(8, 78, 4, 0),
QT_MOC_LITERAL(9, 83, 6, 0),
QT_MOC_LITERAL(10, 90, 4, 0),
QT_MOC_LITERAL(11, 95, 4, 0),
QT_MOC_LITERAL(12, 100, 23, 0),
QT_MOC_LITERAL(13, 124, 1, 0),
QT_MOC_LITERAL(14, 126, 16, 0),
QT_MOC_LITERAL(15, 143, 17, 0),
QT_MOC_LITERAL(16, 161, 14, 0),
QT_MOC_LITERAL(17, 176, 17, 0),
QT_MOC_LITERAL(18, 194, 8, 0),
QT_MOC_LITERAL(19, 203, 7, 0),
QT_MOC_LITERAL(20, 211, 11, 0),
QT_MOC_LITERAL(21, 223, 22, 0),
QT_MOC_LITERAL(22, 246, 11, 0),
QT_MOC_LITERAL(23, 258, 14, 0),
QT_MOC_LITERAL(24, 273, 4, 0),
QT_MOC_LITERAL(25, 278, 7, 0),
QT_MOC_LITERAL(26, 286, 27, 0),
QT_MOC_LITERAL(27, 314, 11, 0),
QT_MOC_LITERAL(28, 326, 16, 0),
QT_MOC_LITERAL(29, 343, 6, 0),
QT_MOC_LITERAL(30, 350, 3, 0),
QT_MOC_LITERAL(31, 354, 11, 0),
QT_MOC_LITERAL(32, 366, 7, 0),
QT_MOC_LITERAL(33, 374, 16, 0),
QT_MOC_LITERAL(34, 391, 8, 0)
    },
    "FiddlerCOM::IFiddlerComClass\0exception\0\0code\0source\0disc\0help\0propertyChanged\0"
    "name\0signal\0argc\0argv\0SetAfterSessionComplete\0f\0SetBeforeRequest\0SetBeforeResponse\0"
    "SetOnLogString\0SetOnNotification\0Shutdown\0Startup\0iListenPort\0bRegisterAsSystemProxy\0"
    "bDecryptSSL\0StartupWithLog\0bLog\0control\0FiddlerCOM::FiddlerCOMClass\0Interface 1\0"
    "IFiddlerComClass\0Equals\0obj\0GetHashCode\0GetType\0mscorlib::_Type*\0ToString\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FiddlerCOM__IFiddlerComClass[] = {

 // content:
    7, // revision
    0,  // classname
    0, 0, // classinfo
    11, 14, // methods
    1, 120, // properties
    0, 0, // enums/sets
    0, 0, // constructors
    0, // flags
    3, // signal count

// signal: name, argc, parameters, tag, flags
    1, 4, 69, 2, 5,
    7, 1, 78, 2, 5,
    9, 3, 81, 2, 5,

// slot: name, argc, parameters, tag, flags
    12, 1, 88, 2, 9,
    14, 1, 91, 2, 9,
    15, 1, 94, 2, 9,
    16, 1, 97, 2, 9,
    17, 1, 100, 2, 9,
    18, 0, 103, 2, 9,
    19, 3, 104, 2, 9,
    23, 4, 111, 2, 9,

// signal: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, 3, 4, 5, 6,
    QMetaType::Void, QMetaType::QString, 8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::VoidStar, 8, 10, 11,

// slot: parameters
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool, 20, 21, 22,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, 20, 21, 22, 24,

 // properties: name, type, flags
    25, QMetaType::QString, 0x0a055003, 		 // QString control

    0 // eod
};

const QMetaObject IFiddlerComClass::staticMetaObject = {
{ &QObject::staticMetaObject,
qt_meta_stringdata_all.data,
qt_meta_data_FiddlerCOM__IFiddlerComClass, 0, 0, 0 }
};

void *IFiddlerComClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, "FiddlerCOM::IFiddlerComClass"))
        return static_cast<void*>(const_cast<IFiddlerComClass*>(this));
    return QAxObject::qt_metacast(_clname);
}
static const uint qt_meta_data_FiddlerCOM__FiddlerCOMClass[] = {

 // content:
    7, // revision
    26,  // classname
    1, 14, // classinfo
    14, 16, // methods
    1, 142, // properties
    0, 0, // enums/sets
    0, 0, // constructors
    0, // flags
    3, // signal count

 // classinfo: key, value
    27, 28, 

// signal: name, argc, parameters, tag, flags
    1, 4, 86, 2, 5,
    7, 1, 95, 2, 5,
    9, 3, 98, 2, 5,

// slot: name, argc, parameters, tag, flags
    29, 1, 105, 2, 9,
    31, 0, 108, 2, 9,
    32, 0, 109, 2, 9,
    12, 1, 110, 2, 9,
    14, 1, 113, 2, 9,
    15, 1, 116, 2, 9,
    16, 1, 119, 2, 9,
    17, 1, 122, 2, 9,
    18, 0, 125, 2, 9,
    19, 3, 126, 2, 9,
    23, 4, 133, 2, 9,

// signal: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, 3, 4, 5, 6,
    QMetaType::Void, QMetaType::QString, 8,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::VoidStar, 8, 10, 11,

// slot: parameters
    QMetaType::Bool, QMetaType::QVariant, 30,
    QMetaType::Int,
    0x80000000 | 33,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void, QMetaType::Int, 13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool, 20, 21, 22,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool, QMetaType::Bool, QMetaType::Bool, 20, 21, 22, 24,

 // properties: name, type, flags
    34, QMetaType::QString, 0x0a005001, 		 // QString ToString

    0 // eod
};

const QMetaObject FiddlerCOMClass::staticMetaObject = {
{ &QObject::staticMetaObject,
qt_meta_stringdata_all.data,
qt_meta_data_FiddlerCOM__FiddlerCOMClass, 0, 0, 0 }
};

void *FiddlerCOMClass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, "FiddlerCOM::FiddlerCOMClass"))
        return static_cast<void*>(const_cast<FiddlerCOMClass*>(this));
    return QAxObject::qt_metacast(_clname);
}

