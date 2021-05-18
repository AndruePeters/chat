#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <controllers/main-controller.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<cm::controllers::MainController>("CM", 1, 0, "MainController");
    cm::controllers::MainController mainController;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("mainController", &mainController);
    const QUrl url(QStringLiteral("qrc:/views/MainView"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
             if (!obj && url == objUrl)
                 QCoreApplication::exit(-1);
      }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
