#ifndef WINDOWLOADER_H
#define WINDOWLOADER_H

#include <QObject>
#include <QQmlComponent>
#include <QQmlIncubator>
#include <QDebug>

class WindowLoader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject *root READ getRootObject WRITE setRootObject NOTIFY rootObjectChanged)

public:
    explicit WindowLoader(QObject *parent = nullptr);
    ~WindowLoader();

    QObject *getRootObject() const;
    void setRootObject(QObject *value);

public slots:
    QObject *createObject(int id);
    void destroyObject(int id);

    //    void append(int id, const QString &url);
    //    void append(int id, const QQmlComponent &component);

    void setCons(QObject *p) {
        qDebug() << p;
    }


    void append(int id, QQmlComponent *component) {
        qDebug() << id << component;
        components.insert(id, component);
    }

signals:
    void rootObjectChanged();

private:
    QObject *rootObject;
    QHash <int, QQmlComponent*> components;

    QHash <int, QQmlIncubator> objects;
};

#endif // WINDOWLOADER_H
