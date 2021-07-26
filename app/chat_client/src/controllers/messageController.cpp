#include <messageController.h>
#include <QDebug>

#include <models/activeChatModel.h>

MessageController::MessageController(QObject* parent): QObject(parent)
{

}

void MessageController::onMessageSend(const QString& message)
{
    qDebug() << "sent message: " << message;
    Message msg;
    msg.from = "from";
    msg.to = "to";
    msg.message = message; 

    emit sendMessage(msg);
}

void MessageController::onMessageReceive(const QString& message)
{
    qDebug() << "recieved message: " << message;
}
