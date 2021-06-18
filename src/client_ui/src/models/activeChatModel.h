#ifndef ACTIVECHATMODEL_H
#define ACTIVECHATMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <vector>

struct Message {
    QString from;
    QString to;
    QString message;
};


class ActiveChatModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Roles {
        SenderRole = Qt::UserRole,
        ReceiverRole,
        MessageRole
    };

    explicit ActiveChatModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void onAddMessage(Message msg);

  private:
    QVector<Message> m_data;
};

#endif