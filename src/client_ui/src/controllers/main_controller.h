/// Main controller

#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <memory>

#include <QObject>
#include <QString>

#include <controllers/settings_controller.h>

class QQmlApplicationEngine;

class Q_DECL_EXPORT MainController : public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString ui_welcomeMessage MEMBER welcomeMessage CONSTANT)

  public:
    explicit MainController(QObject* parent = nullptr);
    void connectSignalsAndSlots(QObject* root);
    QString welcomeMessage = "This is the MainController";

  public slots:
    void onAddMessageToModel(const QString& message);

  private:
    SettingsController* settingsController = nullptr;

  signals:
};

#endif    // MAINCONTROLLER_H
