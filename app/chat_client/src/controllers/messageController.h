#ifndef MESSAGECONTROLLER_H
#define MESSAGECONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QString>

#include <models/activeChatModel.h>

class Q_DECL_EXPORT MessageController : public QObject
{
    Q_OBJECT

  public:
    explicit MessageController(QObject* parent = nullptr);

  public slots:
    void onMessageSend(const QString& message);
    void onMessageReceive(const QString& message);

  signals:
    void sendMessage(Message msg);
};


#endif