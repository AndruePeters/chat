#include "activeChatModel.h"

ActiveChatModel::ActiveChatModel(QObject* parent): QAbstractListModel(parent)
{
    Message msg;
    msg.to = "to";
    msg.from = "from";
    msg.message = "message";
    m_data.push_back(msg);

    msg.from = "drue";
    m_data.push_back(msg);
    onAddMessage(msg);
}


int ActiveChatModel::rowCount( const QModelIndex& parent) const{
    return m_data.size();
}

QVariant ActiveChatModel::data(const QModelIndex& index, int role) const
{
    qDebug() << "calling data()";
    if (!index.isValid() ) {
        qDebug() << "index is invalid";
        return QVariant();
    }

    const Message& message = m_data[index.row()];
    qDebug() << "row: " << index.row();
    switch (role) {
        case SenderRole: return message.from;
        case ReceiverRole: return message.to;
        case MessageRole: return message.message;
        default: return "wrong";
    }
}

QHash<int, QByteArray> ActiveChatModel::roleNames() const
{
    qDebug() << "calling roleNames()";
    static QHash<int, QByteArray> mapping {
        {SenderRole, "sender"},
        {ReceiverRole, "receiver"},
        {MessageRole, "message"}
    };

    return mapping;
}

void ActiveChatModel::onAddMessage(Message msg)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append(msg);
    endInsertRows();


}


#include "moc_activeChatModel.cpp"