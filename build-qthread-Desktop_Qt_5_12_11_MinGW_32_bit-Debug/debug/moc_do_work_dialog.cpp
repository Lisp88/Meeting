/****************************************************************************
** Meta object code from reading C++ file 'do_work_dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qthread/do_work_dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'do_work_dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_filethread_t {
    QByteArrayData data[4];
    char stringdata0[39];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_filethread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_filethread_t qt_meta_stringdata_filethread = {
    {
QT_MOC_LITERAL(0, 0, 10), // "filethread"
QT_MOC_LITERAL(1, 11, 12), // "signal_hello"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 13) // "slot_filework"

    },
    "filethread\0signal_hello\0\0slot_filework"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_filethread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void filethread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<filethread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_hello(); break;
        case 1: _t->slot_filework(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (filethread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&filethread::signal_hello)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject filethread::staticMetaObject = { {
    &Work_thread::staticMetaObject,
    qt_meta_stringdata_filethread.data,
    qt_meta_data_filethread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *filethread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *filethread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_filethread.stringdata0))
        return static_cast<void*>(this);
    return Work_thread::qt_metacast(_clname);
}

int filethread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Work_thread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void filethread::signal_hello()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
struct qt_meta_stringdata_Do_work_dialog_t {
    QByteArrayData data[6];
    char stringdata0[91];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Do_work_dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Do_work_dialog_t qt_meta_stringdata_Do_work_dialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "Do_work_dialog"
QT_MOC_LITERAL(1, 15, 16), // "signal_file_work"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 12), // "slot_do_work"
QT_MOC_LITERAL(4, 46, 19), // "on_pb_start_clicked"
QT_MOC_LITERAL(5, 66, 24) // "on_pb_start_file_clicked"

    },
    "Do_work_dialog\0signal_file_work\0\0"
    "slot_do_work\0on_pb_start_clicked\0"
    "on_pb_start_file_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Do_work_dialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   35,    2, 0x0a /* Public */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Do_work_dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Do_work_dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_file_work(); break;
        case 1: _t->slot_do_work(); break;
        case 2: _t->on_pb_start_clicked(); break;
        case 3: _t->on_pb_start_file_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Do_work_dialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Do_work_dialog::signal_file_work)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Do_work_dialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_Do_work_dialog.data,
    qt_meta_data_Do_work_dialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Do_work_dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Do_work_dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Do_work_dialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Do_work_dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Do_work_dialog::signal_file_work()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
