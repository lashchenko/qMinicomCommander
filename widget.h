#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>

#include "colorparser.h"
#include "regexpparser.h"
#include "commandparser.h"
#include "custombutton.h"
#include "settingsdialog.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void updateText(bool force=false);
    void runCommand();
    void runCommand(QString commandKey);

    /// TODO:
    /// save config
    /// TRAY

    void updateColors();
    void updateCommands();
    void updateRegexp();

    void showDebug(QString debug, int icon = QSystemTrayIcon::Information);

    void activated(QSystemTrayIcon::ActivationReason reason);

    void updatePeriodChange(int period);
    void updateLinesChange(int lines);
    void updateOnOff(bool checked);

    void updateSettings();

protected:
    void createTrayIcon();
    void createActions();

    void closeEvent(QCloseEvent *event);

private:
    SettingsDialog settings;

    QTimer timer;

    QLineEdit lineEdit;
    QTextBrowser browser;

    ColorParser *colorParser;
    CommandParser *commandParser;
    RegexpParser *regexpParser;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *updateColorsAction;
    QAction *updateCommandsAction;
    QAction *updateRegexpAction;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QVector<CustomButton*> buttons;
    QHBoxLayout *buttonLayout;

    qint64 filePosition;
    qint64 fileSize;
    qint64 lineNumber;

    QCheckBox updateOn;

    QSpinBox updatePeriod;
    QSpinBox updateLines;

    QList<CommandHandler*> commands;
};

#endif // WIDGET_H
