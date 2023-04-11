#ifndef QCC_H
#define QCC_H

#include <QQmlApplicationEngine>

#define QCC_RELEASE_VERSION "0"
#define QCC_SUBVERSION "38"
#define QCC_VERSION QCC_RELEASE_VERSION + QString(".22.07.") + QCC_SUBVERSION

class QCC {
public:
    static void registerTypes(QQmlApplicationEngine *engine);
private:
    QCC() = delete;
    ~QCC() = delete;
};

#endif // QCC_H
