// Controller for the settings view

#ifndef CLIENT_SETTINGS_CONTROLLER_H
#define CLIENT_SETTINGS_CONTROLLER_H


#include <QObject>

class Q_DECL_EXPORT SettingsController : public QObject
{
    Q_OBJECT

  public:
    explicit SettingsController(QObject* parent = nullptr);

  public slots:
    void onSaveButton();
};


#endif