// Main controller
// Entry point to every other controller

#include <controllers/main_controller.h>

#include <QDebug>
#include <QQmlApplicationEngine>
#include <QString>


MainController::MainController(QObject* parent)
  : QObject(parent),
    settingsController(new SettingsController(this)),
    messageController(new MessageController(this)),
    activeChatModel( new ActiveChatModel(this))
{
    QObject* settingsView = parent->findChild<QObject*>("settingsView");
    QObject* sendButton = parent->findChild<QObject*>("sendButton");
    QObject* chatModel = parent->findChild<QObject*>("activeChatModel");

    if (sendButton == nullptr) {
        qDebug() << "messageModel is sadly nullptr";
        return;
    }

    QObject::connect(settingsView, SIGNAL(saveSettings()),              settingsController, SLOT(onSaveButton()));
    QObject::connect(sendButton,   SIGNAL(addMessageToModel(QString)),  this,               SLOT(onAddMessageToModel(QString)));
    QObject::connect(sendButton, SIGNAL(addMessageToModel(QString)), messageController, SLOT(onMessageSend(QString)));

    QObject::connect(messageController, SIGNAL(sendMessage(Message)), activeChatModel, SLOT(onAddMessage(Message)));
}

void MainController::onAddMessageToModel(const QString& message)
{
    qDebug() << "you sent: " << message << '\n';
}


#include "moc_main_controller.cpp"