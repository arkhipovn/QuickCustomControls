#include "windowloader.h"
#include <QDebug>

#include <QQuickItem>

WindowLoader::WindowLoader(QObject *parent) : QObject(parent), rootObject(nullptr)
{
    qDebug() << "WindowLoader";
}

WindowLoader::~WindowLoader()
{
    qDebug() << "~WindowLoader";
    //    qDeleteAll(objects);
}

QObject *WindowLoader::getRootObject() const
{
    return rootObject;
}

void WindowLoader::setRootObject(QObject *value)
{
    if(rootObject != value) {
        rootObject = value;
        rootObjectChanged();
    }
}
#include <QQmlEngine>
#include <QQuickView>

QObject *WindowLoader::createObject(int id)
{
    destroyObject(id);
    auto it = components.constFind(id);
    if(it == components.constEnd()) return nullptr;
    qDebug() << "create";

    auto v = it.value()->create();
    qDebug() << v << v->isWindowType() << v->isWidgetType() << qobject_cast<QQuickItem*>(v)

                << qobject_cast<QQmlEngine*>(v)
                   << qobject_cast<QQuickView*>(v);



    QObject::connect(v, &QObject::destroyed, [&]() {
        qDebug() << "dest";
    });

    //    QObject::connect(v, &QQuickItem::visibleChanged, [&]() {
    //        qDebug() << "visi" << v->isVisible();
    //    });

    return v;

    //    return rootObject/*reinterpret_cast<QQmlContext*>(rootObject)*/);

    //    /*
    //


    //    auto component = &it.value();
    //    QQmlIncubator incubator;
    //    component->create(incubator);

    //    while (!incubator.isReady()) {
    //        QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    //    }

    //    return incubator.object();*/
}


void WindowLoader::destroyObject(int id)
{
    //    auto it = objects.constFind(id);
    //    if(it != objects.constEnd()) {
    //        it.value().clear();
    //        objects.erase(it);
    //    }
}

//void WindowLoader::append(int id, const QString &url)
//{

//}

//void WindowLoader::append(int id, const QQmlComponent &component)
//{
//    if(components.constFind(id) != components.constEnd()) return;
//    components.insert(id, component);
//}
