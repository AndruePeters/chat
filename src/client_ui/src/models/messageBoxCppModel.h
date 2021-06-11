#ifndef MESSAGEBOXCPPMODEL_H
#define MESSAGEBOXCPPMODEL_H

#include <QAbstractListModel>

#include <vector>

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit ChatModel(QObject* parent = nullptr);

  private:
    std::vector<QObject*> messages;
};


#endif