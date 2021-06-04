/// This file is the entry point for the chat client

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <controllers/main_controller.h>
#include <controllers/settings_controller.h>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    // register the custom C++ types
    qmlRegisterType<MainController>("MainController", 1, 0, "MainController");
    qmlRegisterType<SettingsController>("SettingsController", 1, 0, "SettingsController");

    // create the engine
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/views/MainView.qml"));
    engine.load(url);

    // grab the root object so that we can use it as the parent for the main controller
    auto rootObject = engine.rootObjects().first();

    // create the main controller
    MainController mainController(rootObject);
    engine.rootContext()->setContextProperty("mainController", &mainController);

    // I don't know what this does yet.
    QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject* obj, const QUrl& objUrl) {
          if (!obj && url == objUrl)
              QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

    return app.exec();
}
