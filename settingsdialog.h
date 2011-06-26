#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>

namespace Settings {
    const QString colors = "SETTINGS/COLORS";
    const QString commands = "SETTINGS/COMMANDS";
    const QString out = "SETTINGS/OUT";
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

    QString getValue(QString key) const;

signals:
    void save();

public slots:

    void selectColors();
    void selectCommands();
    void selectOut();

    void saveSettings();
    void readSettings();

private:
    QLabel pathToColors;
    QLabel pathToCommands;
    QLabel pathToOut;

    QToolButton buttonColors;
    QToolButton buttonCommands;
    QToolButton buttonOut;

    QMap<QString, QString> configs;
};

#endif // SETTINGSDIALOG_H
