/****************************************************************************
**
** Namespace FiddlerCOM generated by dumpcpp from type library
** e:\QTProject_GIT\a86b43fca89c33a2f703c0c6c7221cb8\FiddlerCOM\FiddlerCOM\bin\Release\FiddlerCOM.tlb
**
****************************************************************************/

#ifndef QAX_DUMPCPP_FIDDLERCOM_H
#define QAX_DUMPCPP_FIDDLERCOM_H

// Define this symbol to __declspec(dllexport) or __declspec(dllimport)
#ifndef FIDDLERCOM_EXPORT
#define FIDDLERCOM_EXPORT
#endif

#include <qaxobject.h>
#include <qaxwidget.h>
#include <qdatetime.h>
#include <qpixmap.h>

struct IDispatch;


// Referenced namespace
namespace mscorlib {
    class _Type;
}


namespace FiddlerCOM {


class FIDDLERCOM_EXPORT IFiddlerComClass : public QAxObject
{
public:
    IFiddlerComClass(IDispatch *subobject = 0, QAxObject *parent = 0)
    : QAxObject((IUnknown*)subobject, parent)
    {
        internalRelease();
    }

    /*
    Method SetAfterSessionComplete
    */
    inline void SetAfterSessionComplete(int f);

    /*
    Method SetBeforeRequest
    */
    inline void SetBeforeRequest(int f);

    /*
    Method SetBeforeResponse
    */
    inline void SetBeforeResponse(int f);

    /*
    Method SetOnLogString
    */
    inline void SetOnLogString(int f);

    /*
    Method SetOnNotification
    */
    inline void SetOnNotification(int f);

    /*
    Method Shutdown
    */
    inline void Shutdown();

    /*
    Method Startup
    */
    inline void Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL);

    /*
    Method StartupWithLog
    */
    inline void StartupWithLog(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL, bool bLog);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

// Actual coclasses
class FIDDLERCOM_EXPORT FiddlerCOMClass : public QAxObject
{
public:
    FiddlerCOMClass(QObject *parent = 0)
    : QAxObject(parent)
    {
        setControl("{45ce0eac-ba23-3eaf-8d74-95cf8f87e588}");
    }

    FiddlerCOMClass(IFiddlerComClass *iface)
    : QAxObject()
    {
        initializeFrom(iface);
        delete iface;
    }

    /*
    Property ToString
    */
    inline QString ToString() const; //Returns the value of ToString

    /*
    Method Equals
    */
    inline bool Equals(const QVariant& obj);

    /*
    Method GetHashCode
    */
    inline int GetHashCode();

    /*
    Method GetType
    */
    inline mscorlib::_Type* GetType();

    /*
    Method SetAfterSessionComplete
    */
    inline void SetAfterSessionComplete(int f);

    /*
    Method SetBeforeRequest
    */
    inline void SetBeforeRequest(int f);

    /*
    Method SetBeforeResponse
    */
    inline void SetBeforeResponse(int f);

    /*
    Method SetOnLogString
    */
    inline void SetOnLogString(int f);

    /*
    Method SetOnNotification
    */
    inline void SetOnNotification(int f);

    /*
    Method Shutdown
    */
    inline void Shutdown();

    /*
    Method Startup
    */
    inline void Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL);

    /*
    Method StartupWithLog
    */
    inline void StartupWithLog(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL, bool bLog);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

// member function implementation
#ifndef QAX_DUMPCPP_FIDDLERCOM_NOINLINES
inline void IFiddlerComClass::SetAfterSessionComplete(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
}

inline void IFiddlerComClass::SetBeforeRequest(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
}

inline void IFiddlerComClass::SetBeforeResponse(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 10, _a);
}

inline void IFiddlerComClass::SetOnLogString(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
}

inline void IFiddlerComClass::SetOnNotification(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
}

inline void IFiddlerComClass::Shutdown()
{
    void *_a[] = {0};
    qt_metacall(QMetaObject::InvokeMetaMethod, 13, _a);
}

inline void IFiddlerComClass::Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL)
{
    void *_a[] = {0, (void*)&iListenPort, (void*)&bRegisterAsSystemProxy, (void*)&bDecryptSSL};
    qt_metacall(QMetaObject::InvokeMetaMethod, 14, _a);
}

inline void IFiddlerComClass::StartupWithLog(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL, bool bLog)
{
    void *_a[] = {0, (void*)&iListenPort, (void*)&bRegisterAsSystemProxy, (void*)&bDecryptSSL, (void*)&bLog};
    qt_metacall(QMetaObject::InvokeMetaMethod, 15, _a);
}


inline QString FiddlerCOMClass::ToString() const
{
    QVariant qax_result = property("ToString");
    Q_ASSERT(qax_result.isValid());
    return *(QString*)qax_result.constData();
}

inline bool FiddlerCOMClass::Equals(const QVariant& obj)
{
    bool qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&obj};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
    return qax_result;
}

inline int FiddlerCOMClass::GetHashCode()
{
    int qax_result;
    void *_a[] = {(void*)&qax_result};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
    return qax_result;
}

inline mscorlib::_Type* FiddlerCOMClass::GetType()
{
    mscorlib::_Type* qax_result = 0;
#ifdef QAX_DUMPCPP_MSCORLIB_H
    qRegisterMetaType<mscorlib::_Type*>("mscorlib::_Type*", &qax_result);
    qRegisterMetaType<mscorlib::_Type>("mscorlib::_Type", qax_result);
#endif
    void *_a[] = {(void*)&qax_result};
    qt_metacall(QMetaObject::InvokeMetaMethod, 10, _a);
    return qax_result;
}

inline void FiddlerCOMClass::SetAfterSessionComplete(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
}

inline void FiddlerCOMClass::SetBeforeRequest(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
}

inline void FiddlerCOMClass::SetBeforeResponse(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 13, _a);
}

inline void FiddlerCOMClass::SetOnLogString(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 14, _a);
}

inline void FiddlerCOMClass::SetOnNotification(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 15, _a);
}

inline void FiddlerCOMClass::Shutdown()
{
    void *_a[] = {0};
    qt_metacall(QMetaObject::InvokeMetaMethod, 16, _a);
}

inline void FiddlerCOMClass::Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL)
{
    void *_a[] = {0, (void*)&iListenPort, (void*)&bRegisterAsSystemProxy, (void*)&bDecryptSSL};
    qt_metacall(QMetaObject::InvokeMetaMethod, 17, _a);
}

inline void FiddlerCOMClass::StartupWithLog(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL, bool bLog)
{
    void *_a[] = {0, (void*)&iListenPort, (void*)&bRegisterAsSystemProxy, (void*)&bDecryptSSL, (void*)&bLog};
    qt_metacall(QMetaObject::InvokeMetaMethod, 18, _a);
}



#endif

}

QT_BEGIN_NAMESPACE

namespace QtMetaTypePrivate {
template<>
struct QMetaTypeFunctionHelper<FiddlerCOM::IFiddlerComClass, /* Accepted */ true> {
    static void Delete(void *t) { delete static_cast<FiddlerCOM::IFiddlerComClass*>(t); }
    static void *Create(const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new FiddlerCOM::IFiddlerComClass();
    }
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<FiddlerCOM::IFiddlerComClass*>(t)->FiddlerCOM::IFiddlerComClass::~IFiddlerComClass();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) FiddlerCOM::IFiddlerComClass;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const FiddlerCOM::IFiddlerComClass*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<FiddlerCOM::IFiddlerComClass*>(t); }
#endif // QT_NO_DATASTREAM
};

template<>
struct QMetaTypeFunctionHelper<FiddlerCOM::FiddlerCOMClass, /* Accepted */ true> {
    static void Delete(void *t) { delete static_cast<FiddlerCOM::FiddlerCOMClass*>(t); }
    static void *Create(const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new FiddlerCOM::FiddlerCOMClass();
    }
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<FiddlerCOM::FiddlerCOMClass*>(t)->FiddlerCOM::FiddlerCOMClass::~FiddlerCOMClass();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) FiddlerCOM::FiddlerCOMClass;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const FiddlerCOM::FiddlerCOMClass*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<FiddlerCOM::FiddlerCOMClass*>(t); }
#endif // QT_NO_DATASTREAM
};

} // namespace QtMetaTypePrivate
QT_END_NAMESPACE

#endif

