/****************************************************************************
** Meta object code from reading C++ file 'dataprocessing.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../SORGES-sourcecode-20150126/classes/dataprocessing.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataprocessing.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DataProcessing_t {
    QByteArrayData data[21];
    char stringdata[309];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DataProcessing_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DataProcessing_t qt_meta_stringdata_DataProcessing = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 14),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 17),
QT_MOC_LITERAL(4, 49, 8),
QT_MOC_LITERAL(5, 58, 20),
QT_MOC_LITERAL(6, 79, 14),
QT_MOC_LITERAL(7, 94, 14),
QT_MOC_LITERAL(8, 109, 6),
QT_MOC_LITERAL(9, 116, 6),
QT_MOC_LITERAL(10, 123, 13),
QT_MOC_LITERAL(11, 137, 4),
QT_MOC_LITERAL(12, 142, 14),
QT_MOC_LITERAL(13, 157, 18),
QT_MOC_LITERAL(14, 176, 15),
QT_MOC_LITERAL(15, 192, 4),
QT_MOC_LITERAL(16, 197, 23),
QT_MOC_LITERAL(17, 221, 8),
QT_MOC_LITERAL(18, 230, 28),
QT_MOC_LITERAL(19, 259, 24),
QT_MOC_LITERAL(20, 284, 24)
    },
    "DataProcessing\0stationsLoaded\0\0"
    "std::set<Station>\0stations\0"
    "stationColorReceived\0changedStation\0"
    "originReceived\0Origin\0origin\0eventReceived\0"
    "init\0initSimulation\0simulationDateTime\0"
    "fileChangedSlot\0path\0processStationsFromFile\0"
    "namefile\0processColorStationsFromFile\0"
    "processOriginFromFileLog\0"
    "processOriginFromFileXml"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DataProcessing[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,
       7,    1,   75,    2, 0x06 /* Public */,
      10,    1,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   81,    2, 0x0a /* Public */,
      12,    1,   82,    2, 0x0a /* Public */,
      14,    1,   85,    2, 0x08 /* Private */,
      16,    1,   88,    2, 0x08 /* Private */,
      18,    1,   91,    2, 0x08 /* Private */,
      19,    1,   94,    2, 0x08 /* Private */,
      20,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDateTime,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::QString,   17,
    0x80000000 | 3, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   17,

       0        // eod
};

void DataProcessing::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DataProcessing *_t = static_cast<DataProcessing *>(_o);
        switch (_id) {
        case 0: _t->stationsLoaded((*reinterpret_cast< std::set<Station>(*)>(_a[1]))); break;
        case 1: _t->stationColorReceived((*reinterpret_cast< std::set<Station>(*)>(_a[1]))); break;
        case 2: _t->originReceived((*reinterpret_cast< Origin(*)>(_a[1]))); break;
        case 3: _t->eventReceived((*reinterpret_cast< Origin(*)>(_a[1]))); break;
        case 4: _t->init(); break;
        case 5: _t->initSimulation((*reinterpret_cast< QDateTime(*)>(_a[1]))); break;
        case 6: _t->fileChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->processStationsFromFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: { std::set<Station> _r = _t->processColorStationsFromFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< std::set<Station>*>(_a[0]) = _r; }  break;
        case 9: _t->processOriginFromFileLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->processOriginFromFileXml((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DataProcessing::*_t)(std::set<Station> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataProcessing::stationsLoaded)) {
                *result = 0;
            }
        }
        {
            typedef void (DataProcessing::*_t)(std::set<Station> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataProcessing::stationColorReceived)) {
                *result = 1;
            }
        }
        {
            typedef void (DataProcessing::*_t)(Origin );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataProcessing::originReceived)) {
                *result = 2;
            }
        }
        {
            typedef void (DataProcessing::*_t)(Origin );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DataProcessing::eventReceived)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject DataProcessing::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DataProcessing.data,
      qt_meta_data_DataProcessing,  qt_static_metacall, 0, 0}
};


const QMetaObject *DataProcessing::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DataProcessing::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataProcessing.stringdata))
        return static_cast<void*>(const_cast< DataProcessing*>(this));
    return QObject::qt_metacast(_clname);
}

int DataProcessing::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void DataProcessing::stationsLoaded(std::set<Station> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DataProcessing::stationColorReceived(std::set<Station> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DataProcessing::originReceived(Origin _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DataProcessing::eventReceived(Origin _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
