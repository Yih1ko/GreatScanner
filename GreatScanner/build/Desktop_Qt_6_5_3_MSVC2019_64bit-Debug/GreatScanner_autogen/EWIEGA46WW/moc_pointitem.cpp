/****************************************************************************
** Meta object code from reading C++ file 'pointitem.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../pointitem.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pointitem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSPointItemENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSPointItemENDCLASS = QtMocHelpers::stringData(
    "PointItem",
    "positionChanged",
    "",
    "newPos",
    "selected",
    "PointItem*",
    "item",
    "state",
    "dragEnded"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSPointItemENDCLASS_t {
    uint offsetsAndSizes[18];
    char stringdata0[10];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[7];
    char stringdata4[9];
    char stringdata5[11];
    char stringdata6[5];
    char stringdata7[6];
    char stringdata8[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSPointItemENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSPointItemENDCLASS_t qt_meta_stringdata_CLASSPointItemENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "PointItem"
        QT_MOC_LITERAL(10, 15),  // "positionChanged"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 6),  // "newPos"
        QT_MOC_LITERAL(34, 8),  // "selected"
        QT_MOC_LITERAL(43, 10),  // "PointItem*"
        QT_MOC_LITERAL(54, 4),  // "item"
        QT_MOC_LITERAL(59, 5),  // "state"
        QT_MOC_LITERAL(65, 9)   // "dragEnded"
    },
    "PointItem",
    "positionChanged",
    "",
    "newPos",
    "selected",
    "PointItem*",
    "item",
    "state",
    "dragEnded"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSPointItemENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   32,    2, 0x06,    1 /* Public */,
       4,    2,   35,    2, 0x06,    3 /* Public */,
       8,    0,   40,    2, 0x06,    6 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPointF,    3,
    QMetaType::Void, 0x80000000 | 5, QMetaType::Bool,    6,    7,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject PointItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSPointItemENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSPointItemENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSPointItemENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PointItem, std::true_type>,
        // method 'positionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPointF &, std::false_type>,
        // method 'selected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<PointItem *, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'dragEnded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void PointItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PointItem *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->positionChanged((*reinterpret_cast< std::add_pointer_t<QPointF>>(_a[1]))); break;
        case 1: _t->selected((*reinterpret_cast< std::add_pointer_t<PointItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 2: _t->dragEnded(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< PointItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PointItem::*)(const QPointF & );
            if (_t _q_method = &PointItem::positionChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PointItem::*)(PointItem * , bool );
            if (_t _q_method = &PointItem::selected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PointItem::*)();
            if (_t _q_method = &PointItem::dragEnded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *PointItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PointItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSPointItemENDCLASS.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QAbstractGraphicsShapeItem"))
        return static_cast< QAbstractGraphicsShapeItem*>(this);
    return QObject::qt_metacast(_clname);
}

int PointItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void PointItem::positionChanged(const QPointF & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PointItem::selected(PointItem * _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PointItem::dragEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
