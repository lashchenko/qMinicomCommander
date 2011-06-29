#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>

//namespace Settings {
//    const QString colors = "SETTINGS/COLORS";
//    const QString commands = "SETTINGS/COMMANDS";
//    const QString out = "SETTINGS/OUT";
//}

enum SETTINGS {
    COLORS,
    COMMANDS,
    REGEXP,
    PALETTE,
    OUT
};

class SettingsItem;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

    QString getValue(int key) const;

signals:
    void save();

public slots:

//    void selectColors();
//    void selectCommands();
//    void selectOut();
//    void selectRegexp();

//    void selectPath();

    void saveSettings();
    void readSettings();

private:
//    QLabel pathToColors;
//    QLabel pathToCommands;
//    QLabel pathToRegExp;
//    QLabel pathToOut;

//    QToolButton buttonColors;
//    QToolButton buttonCommands;
//    QToolButton buttonRegExp;
//    QToolButton buttonOut;

//    QMap<int, QLabel*> pathes;
//    QMap<int, QToolButton*> selectors;
//    QMap<int, QString> configs;

    QMap<int, SettingsItem*> items;
//    QList<int> indexes;
};

class SettingsItem : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsItem(QWidget *parent = 0);

    QString getPath() const;
    void setPath(QString text);

    QString getDefault() const;
    void setDefault(QString fileName);

private slots:
    void selectPath();

private:
    QLabel path;
    QToolButton selector;

    QString defaultFileName;
};

#endif // SETTINGSDIALOG_H
