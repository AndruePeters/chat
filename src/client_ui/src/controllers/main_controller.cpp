// Main controller
// Entry point to every other controller

#include <controllers/main_controller.h>

#include <QDebug>
#include <QQmlApplicationEngine>


MainController::MainController(QObject* parent)
  : QObject(parent),
    settingsController(new SettingsController(this))
{
    QObject* settingsView = parent->findChild<QObject*>("settingsView");

    if (settingsView == nullptr) {
        qDebug() << "settingsView is sadly nullptr";
        return;
    }

    QObject::connect(settingsView, SIGNAL(saveSettings()), settingsController, SLOT(onSaveButton()));
}


#include "moc_main_controller.cpp"