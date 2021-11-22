
#ifndef MOUSEZOOMTYPE_H
#define MOUSEZOOMTYPE_H
#include <QObject>
#include <QStringList>
#include <QMetaType>
#include <QMetaObject>
#include <QMetaEnum>

namespace internals {
    class MouseWheelZoomType:public QObject
    {
        Q_OBJECT
        Q_ENUMS(Types)
    public:

    virtual ~MouseWheelZoomType();

    enum Types
    {
        /// <summary>
        /// zooms map to current mouse position and makes it map center
        /// </summary>
        MousePositionAndCenter,

        /// <summary>
        /// zooms to current mouse position, but doesn't make it map center,
        /// google/bing style ;}
        /// </summary>
        MousePositionWithoutCenter,

        /// <summary>
        /// zooms map to current view center
        /// </summary>
        ViewCenter
    };
    static QString StrByType(Types const& value)
    {
        QMetaObject metaObject = MouseWheelZoomType().staticMetaObject;
        QMetaEnum metaEnum= metaObject.enumerator( metaObject.indexOfEnumerator("Types"));
        QString s=metaEnum.valueToKey(value);
        return s;
    }
    static Types TypeByStr(QString const& value)
    {
        QMetaObject metaObject = MouseWheelZoomType().staticMetaObject;
        QMetaEnum metaEnum= metaObject.enumerator( metaObject.indexOfEnumerator("Types"));
        Types s=(Types)metaEnum.keyToValue(value.toLatin1());
        return s;
    }
    static QStringList TypesList()
    {
        QStringList ret;
        QMetaObject metaObject = MouseWheelZoomType().staticMetaObject;
        QMetaEnum metaEnum= metaObject.enumerator( metaObject.indexOfEnumerator("Types"));
        for(int x=0;x<metaEnum.keyCount();++x)
        {
            ret.append(metaEnum.key(x));
        }
        return ret;
    }

};

}
Q_DECLARE_METATYPE(internals::MouseWheelZoomType::Types)
#endif // MOUSEZOOMTYPE_H

