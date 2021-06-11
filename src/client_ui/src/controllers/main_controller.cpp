// Main controller
// Entry point to every other controller

#include <controllers/main_controller.h>

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QString>


MainController::MainController(QObject* parent)
  : QObject(parent),
    settingsController(new SettingsController(this))
{
    QObject* settingsView = parent->findChild<QObject*>("settingsView");
    QObject* sendButton = parent->findChild<QObject*>("sendButton");

    if (sendButton == nullptr) {
        qDebug() << "messageModel is sadly nullptr";
        return;
    }

    QObject::connect(settingsView, SIGNAL(saveSettings()),              settingsController, SLOT(onSaveButton()));

    QObject::connect(sendButton,   SIGNAL(addMessageToModel(QString)),  this,               SLOT(onAddMessageToModel(QString)));
}

void MainController::onAddMessageToModel(const QString& message)
{
    qDebug() << "you sent: " << message << '\n';
}


#include "moc_main_controller.cpp"