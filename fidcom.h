/****************************************************************************
**
** Namespace FidCOM generated by dumpcpp from type library
** e:\QTProject_GIT\a86b43fca89c33a2f703c0c6c7221cb8\FidCOM\FidCOM\bin\Release\FidCOM.tlb
**
****************************************************************************/

#ifndef QAX_DUMPCPP_FIDCOM_H
#define QAX_DUMPCPP_FIDCOM_H

// Define this symbol to __declspec(dllexport) or __declspec(dllimport)
#ifndef FIDCOM_EXPORT
#define FIDCOM_EXPORT
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


namespace FidCOM {


class FIDCOM_EXPORT IFidComClass : public QAxObject
{
public:
    IFidComClass(IDispatch *subobject = 0, QAxObject *parent = 0)
    : QAxObject((IUnknown*)subobject, parent)
    {
        internalRelease();
    }

    /*
    Method InstallCertificate
    */
    inline bool InstallCertificate(QString& certStr, QString& keyStr);

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
    Method SetupCertificate
    */
    inline void SetupCertificate(const QString& certStr, const QString& keyStr);

    /*
    Method Shutdown
    */
    inline void Shutdown();

    /*
    Method Startup
    */
    inline void Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL);

    /*
    Method UninstallCertificate
    */
    inline bool UninstallCertificate(QString& certStr, QString& keyStr);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

// Actual coclasses
class FIDCOM_EXPORT FidCOMClass : public QAxObject
{
public:
    FidCOMClass(QObject *parent = 0)
    : QAxObject(parent)
    {
        setControl("{31498642-c503-30e5-b8d5-289d79dffdfc}");
    }

    FidCOMClass(IFidComClass *iface)
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
    Method InstallCertificate
    */
    inline bool InstallCertificate(QString& certStr, QString& keyStr);

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
    Method SetupCertificate
    */
    inline void SetupCertificate(const QString& certStr, const QString& keyStr);

    /*
    Method Shutdown
    */
    inline void Shutdown();

    /*
    Method Startup
    */
    inline void Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL);

    /*
    Method UninstallCertificate
    */
    inline bool UninstallCertificate(QString& certStr, QString& keyStr);

// meta object functions
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const { return &staticMetaObject; }
    virtual void *qt_metacast(const char *);
};

// member function implementation
#ifndef QAX_DUMPCPP_FIDCOM_NOINLINES
inline bool IFidComClass::InstallCertificate(QString& certStr, QString& keyStr)
{
    bool qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&certStr, (void*)&keyStr};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
    return qax_result;
}

inline void IFidComClass::SetAfterSessionComplete(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
}

inline void IFidComClass::SetBeforeRequest(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 10, _a);
}

inline void IFidComClass::SetBeforeResponse(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
}

inline void IFidComClass::SetOnLogString(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
}

inline void IFidComClass::SetOnNotification(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 13, _a);
}

inline void IFidComClass::SetupCertificate(const QString& certStr, const QString& keyStr)
{
    void *_a[] = {0, (void*)&certStr, (void*)&keyStr};
    qt_metacall(QMetaObject::InvokeMetaMethod, 14, _a);
}

inline void IFidComClass::Shutdown()
{
    void *_a[] = {0};
    qt_metacall(QMetaObject::InvokeMetaMethod, 15, _a);
}

inline void IFidComClass::Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL)
{
    void *_a[] = {0, (void*)&iListenPort, (void*)&bRegisterAsSystemProxy, (void*)&bDecryptSSL};
    qt_metacall(QMetaObject::InvokeMetaMethod, 16, _a);
}

inline bool IFidComClass::UninstallCertificate(QString& certStr, QString& keyStr)
{
    bool qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&certStr, (void*)&keyStr};
    qt_metacall(QMetaObject::InvokeMetaMethod, 17, _a);
    return qax_result;
}


inline QString FidCOMClass::ToString() const
{
    QVariant qax_result = property("ToString");
    Q_ASSERT(qax_result.isValid());
    return *(QString*)qax_result.constData();
}

inline bool FidCOMClass::Equals(const QVariant& obj)
{
    bool qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&obj};
    qt_metacall(QMetaObject::InvokeMetaMethod, 8, _a);
    return qax_result;
}

inline int FidCOMClass::GetHashCode()
{
    int qax_result;
    void *_a[] = {(void*)&qax_result};
    qt_metacall(QMetaObject::InvokeMetaMethod, 9, _a);
    return qax_result;
}

inline mscorlib::_Type* FidCOMClass::GetType()
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

inline bool FidCOMClass::InstallCertificate(QString& certStr, QString& keyStr)
{
    bool qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&certStr, (void*)&keyStr};
    qt_metacall(QMetaObject::InvokeMetaMethod, 11, _a);
    return qax_result;
}

inline void FidCOMClass::SetAfterSessionComplete(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 12, _a);
}

inline void FidCOMClass::SetBeforeRequest(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 13, _a);
}

inline void FidCOMClass::SetBeforeResponse(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 14, _a);
}

inline void FidCOMClass::SetOnLogString(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 15, _a);
}

inline void FidCOMClass::SetOnNotification(int f)
{
    void *_a[] = {0, (void*)&f};
    qt_metacall(QMetaObject::InvokeMetaMethod, 16, _a);
}

inline void FidCOMClass::SetupCertificate(const QString& certStr, const QString& keyStr)
{
    void *_a[] = {0, (void*)&certStr, (void*)&keyStr};
    qt_metacall(QMetaObject::InvokeMetaMethod, 17, _a);
}

inline void FidCOMClass::Shutdown()
{
    void *_a[] = {0};
    qt_metacall(QMetaObject::InvokeMetaMethod, 18, _a);
}

inline void FidCOMClass::Startup(int iListenPort, bool bRegisterAsSystemProxy, bool bDecryptSSL)
{
    void *_a[] = {0, (void*)&iListenPort, (void*)&bRegisterAsSystemProxy, (void*)&bDecryptSSL};
    qt_metacall(QMetaObject::InvokeMetaMethod, 19, _a);
}

inline bool FidCOMClass::UninstallCertificate(QString& certStr, QString& keyStr)
{
    bool qax_result;
    void *_a[] = {(void*)&qax_result, (void*)&certStr, (void*)&keyStr};
    qt_metacall(QMetaObject::InvokeMetaMethod, 20, _a);
    return qax_result;
}



#endif

}

QT_BEGIN_NAMESPACE

namespace QtMetaTypePrivate {
template<>
struct QMetaTypeFunctionHelper<FidCOM::IFidComClass, /* Accepted */ true> {
    static void Delete(void *t) { delete static_cast<FidCOM::IFidComClass*>(t); }
    static void *Create(const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new FidCOM::IFidComClass();
    }
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<FidCOM::IFidComClass*>(t)->FidCOM::IFidComClass::~IFidComClass();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) FidCOM::IFidComClass;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const FidCOM::IFidComClass*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<FidCOM::IFidComClass*>(t); }
#endif // QT_NO_DATASTREAM
};

template<>
struct QMetaTypeFunctionHelper<FidCOM::FidCOMClass, /* Accepted */ true> {
    static void Delete(void *t) { delete static_cast<FidCOM::FidCOMClass*>(t); }
    static void *Create(const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new FidCOM::FidCOMClass();
    }
    static void Destruct(void *t)
    {
        Q_UNUSED(t)
        static_cast<FidCOM::FidCOMClass*>(t)->FidCOM::FidCOMClass::~FidCOMClass();
    }
    static void *Construct(void *where, const void *t)
    {
        Q_ASSERT(!t);
        Q_UNUSED(t)
        return new (where) FidCOM::FidCOMClass;
    }
#ifndef QT_NO_DATASTREAM
    static void Save(QDataStream &stream, const void *t) { stream << *static_cast<const FidCOM::FidCOMClass*>(t); }
    static void Load(QDataStream &stream, void *t) { stream >> *static_cast<FidCOM::FidCOMClass*>(t); }
#endif // QT_NO_DATASTREAM
};

} // namespace QtMetaTypePrivate
QT_END_NAMESPACE

#endif

