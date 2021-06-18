// Settings controller

#include <controllers/settings_controller.h>

#include <QDebug>

SettingsController::SettingsController(QObject* parent) : QObject(parent)
{
    if (parent == nullptr) {
        qDebug() << "parent is nullptr";
    }
}


void SettingsController::onSaveButton()
{
    qDebug() << "You clicked save.";
}

#include "moc_settings_controller.cpp"