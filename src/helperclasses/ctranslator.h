#ifndef CTRANSLATOR_H
#define CTRANSLATOR_H

#include<QLocale>
#include <QTranslator>
#include <QCoreApplication>

#ifdef QT_QML_LIB // Q_QML_EXPORT
#include <QQmlApplicationEngine>
#endif

class CTranslator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList languages READ listLanguage NOTIFY listChanged)
    Q_PROPERTY(QLocale::Language currentLanguage READ getCurrentLanguage WRITE setCurrentLanguage NOTIFY currentLanguageChanged)

public:
    static CTranslator *getInstance();
    void clear();

    /*
        @defaultLanguage - какой язык у написанного приложения в самом начале (без файлов перевода)
        @app - QApplication. Нужно для автоматической установки языка
        @engine - для автоматической установки языка в qml
    */
    void init(QLocale::Language defaultLanguage = QLocale::English,
              QCoreApplication *app = nullptr
          #ifdef QT_QML_LIB // Q_QML_EXPORT
              , QQmlApplicationEngine *engine = nullptr
        #endif
            );

    void setCurrentLanguage(QLocale::Language language);

    void appendLanguage(QLocale::Language language, const QString &url);
    void appendLanguage(QLocale::Language language, const QStringList &urls);
    void eraseLanguage(QLocale::Language language);

    QVariantList listLanguage() const;
    QLocale::Language getCurrentLanguage() const;
    QLocale::Language getDefaultLanguage() const;

public slots:
    /*static*/ QString languageToString(QLocale::Language language);

private:
    explicit CTranslator();
    ~CTranslator();

    QCoreApplication *app;
#ifdef QT_QML_LIB // Q_QML_EXPORT
    QQmlApplicationEngine *engine;
#endif

    QLocale::Language defaultLanguage;
    QLocale::Language currentLanguage;

    struct TranslatorInfo
    {
        TranslatorInfo(const QString &url);

        QString url;
        QTranslator translator;

        bool operator==(const TranslatorInfo& p) const;
        bool operator!=(const TranslatorInfo& p) const;

        TranslatorInfo& operator=(const TranslatorInfo& right);
    };

    QMap <QLocale::Language, QList <TranslatorInfo*>> languages;    // urls
    bool contains(const QList <TranslatorInfo*> &list, const QString &url) const;

signals:
    void listChanged();
    void currentLanguageChanged(QLocale::Language language);
};

#endif // CTRANSLATOR_H
