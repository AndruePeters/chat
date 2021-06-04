#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QString>

namespace cm::controllers {
class Q_DECL_EXPORT MainController : public QObject {

Q_OBJECT
    Q_PROPERTY( QString ui_welcomeMessage MEMBER welcomeMessage CONSTANT )

public:
    explicit MainController(QObject *parent = nullptr);
    QString welcomeMessage = "This is the MainController";

signals:

};

}

#endif // MAINCONTROLLER_H
