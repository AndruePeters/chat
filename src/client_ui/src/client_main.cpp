/// This file is the entry point for the chat client

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <controllers/main_controller.h>
#include <controllers/settings_controller.h>
#include <controllers/messageController.h>

#include <models/activeChatModel.h>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    // register the custom C++ types
    qmlRegisterType<SettingsController>("SettingsController", 1, 0, "SettingsController");
    qmlRegisterType<MessageController>("MessageController", 1, 0, "MessageController");
    qmlRegisterType<Message>("Message", 1, 0, "Message");
    qmlRegisterType<ActiveChatModel>("ActiveChatModel", 1, 0, "ActiveChatModel");


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
