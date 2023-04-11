#include "ctranslator.h"
#include <QDebug>

CTranslator *CTranslator::getInstance()
{
    static CTranslator translator;
    return &translator;
}

void CTranslator::clear()
{
    auto it = languages.begin();
    while(it != languages.end()) {
        for(int i = 0; i < it.value().size(); ++i)
            delete it.value()[i];
        ++it;
    }

    languages.clear();
    emit listChanged();

    this->defaultLanguage = QLocale::AnyLanguage;
    this->currentLanguage = QLocale::AnyLanguage;
    app = nullptr;
#ifdef QT_QML_LIB // Q_QML_EXPORT
    this->engine = nullptr;
#endif
}

void CTranslator::init(QLocale::Language defaultLanguage,
                       QCoreApplication *app
                       #ifdef QT_QML_LIB // Q_QML_EXPORT
                       , QQmlApplicationEngine *engine
                       #endif
                       )
{
    clear();

    this->app = app;
#ifdef QT_QML_LIB // Q_QML_EXPORT
    this->engine = engine;
#endif

    this->defaultLanguage = defaultLanguage;
    this->currentLanguage = defaultLanguage;

    //@N Я ЗАДУМАЛСЯ, А НУЖНО ЛИ УДАЛЯТЬ СРАЗУ ВСЕ ПРЕВОДЫ? ПУСТЬ БУДУТ..
    //@N если уже есть что-то, то мб правильно сначала удалить у app?
    //    auto it = languages.find(defaultLanguage);
    //    if(it != languages.end()) {
    //        // что-то не так. Так не должно быть. init же.. Нужно сбросить
    //        for(auto t : it.value()) delete t;
    //    }
    //    //@N мб set вызвать?
}

CTranslator::CTranslator() : QObject(nullptr)
{
    this->defaultLanguage = QLocale::AnyLanguage;
    this->currentLanguage = QLocale::AnyLanguage;
    app = nullptr;
#ifdef QT_QML_LIB // Q_QML_EXPORT
    this->engine = nullptr;
#endif
}

CTranslator::~CTranslator()
{
    for(auto language : languages) {
        for(auto t : language) delete t;
    }
}

QLocale::Language CTranslator::getDefaultLanguage() const
{
    return defaultLanguage;
}

void CTranslator::appendLanguage(QLocale::Language language, const QString &url)
{
    auto it = languages.find(language);

    TranslatorInfo *translator = nullptr;
    if(it == languages.end()) {
        // новый добавляем
        translator = new TranslatorInfo(url);
        languages.insert(language, { translator });
        emit listChanged();
    }
    else { // уже есть
        // сравним, не повторяется ли он..
        if(!contains(it.value(), url)) {
            translator = new TranslatorInfo(url);
            it.value().append(translator);
        }
    }

    if(app != nullptr && language == currentLanguage && translator != nullptr) {
        // обновим.. Новое же что-то добавилось..
        app->installTranslator(&translator->translator);
#ifdef QT_QML_LIB // Q_QML_EXPORT


#if QT_VERSION > QT_VERSION_CHECK(5, 8, 0)
        if(engine != nullptr) engine->retranslate();
#endif
        //@N придумать что сделать..

#endif
    }
}

void CTranslator::appendLanguage(QLocale::Language language, const QStringList &urls)
{
    auto it = languages.find(language);
    QList <TranslatorInfo*> newTranslators;

    if(it == languages.end()) {
        // новый добавляем
        for(auto u : urls) {
            auto translator = new TranslatorInfo(u);
            newTranslators.append(translator);
        }
        languages.insert(language, newTranslators);
        emit listChanged();
    }
    else {
        for(auto u : urls) {
            if(contains(it.value(), u)) continue;
            auto translator = new TranslatorInfo(u);
            it.value().append(translator);
            newTranslators.append(translator);
        }
    }

    if(app != nullptr && language == currentLanguage && !newTranslators.isEmpty()) {
        // обновим.. Новое же что-то добавилось..
        for(auto t : newTranslators) {
            app->installTranslator(&t->translator);
        }
#ifdef QT_QML_LIB // Q_QML_EXPORT
#if QT_VERSION > QT_VERSION_CHECK(5, 8, 0)
        if(engine != nullptr) engine->retranslate();
#endif
        //@N придумать что сделать..

#endif
    }
}

void CTranslator::eraseLanguage(QLocale::Language language)
{
    auto it = languages.find(language);
    if(it == languages.end()) return;  // нет такого языка
    languages.erase(it);

    if(language == currentLanguage) {
        if(!languages.isEmpty()) setCurrentLanguage(languages.firstKey());
    }
}

QVariantList CTranslator::listLanguage() const
{
    QVariantList m;
    if(!languages.contains(defaultLanguage))
        m.append(defaultLanguage);

    auto it = languages.constBegin();
    while(it != languages.constEnd()) {
        m.append(it.key());
        ++it;
    }
    return m;
}

QLocale::Language CTranslator::getCurrentLanguage() const
{
    return currentLanguage;
}

QString CTranslator::languageToString(QLocale::Language language)
{
    return QLocale::languageToString(language);
}

void CTranslator::setCurrentLanguage(QLocale::Language language)
{
    if(currentLanguage == language) return; //@N

    if(app != nullptr) {
        // app есть, можно менять язык реально
        auto itNewLanguage = languages.constFind(language);
        // Посмотрим есть ли такой язык. Или мб он записан в дефолтный? Если нет, то выходим
        if(itNewLanguage == languages.constEnd() && language != defaultLanguage) return;

        // нужно бы выгрузить старые переводы
        auto itCurrentLanguage = languages.constFind(currentLanguage);
        if(itCurrentLanguage != languages.constEnd()) {
            for(auto t : itCurrentLanguage.value()) {
                // пройдемся по списку url для языка currentLanguage
                app->removeTranslator(&t->translator);
            }
        }

        // если такой язык найден, то возьмем у него переводы
        if(itNewLanguage != languages.constEnd()) {
            for(auto t : itNewLanguage.value()) {
                // пройдемся по списку url для языка language
                app->installTranslator(&t->translator);
            }
        }

#ifdef QT_QML_LIB // Q_QML_EXPORT
#if QT_VERSION > QT_VERSION_CHECK(5, 8, 0)
        if(engine != nullptr) engine->retranslate();
#endif
        //@N придумать что сделать..

#endif
    }

    currentLanguage = language;
    emit currentLanguageChanged(currentLanguage);
}

bool CTranslator::contains(const QList<CTranslator::TranslatorInfo *> &list, const QString &url) const
{
    for(auto t : list) {
        if(t->url.compare(url) == 0) return true;
    }
    return false;
}





// Private

CTranslator::TranslatorInfo::TranslatorInfo(const QString &url)
{
    this->url = url;
    translator.load(url);
}

bool CTranslator::TranslatorInfo::operator==(const TranslatorInfo &p) const
{
    return (this->url.compare(p.url) == 0);
}

bool CTranslator::TranslatorInfo::operator!=(const TranslatorInfo &p) const
{
    return (this->url.compare(p.url) != 0);
}

CTranslator::TranslatorInfo &CTranslator::TranslatorInfo::operator=(const TranslatorInfo &right)
{
    //проверка на самоприсваивание
    if(this == &right) return *this;
    url = right.url;
    return *this;
}
