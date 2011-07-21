#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>

#include "colorparser.h"
#include "regexpparser.h"
#include "commandparser.h"
#include "custombutton.h"
#include "settingsdialog.h"
#include "commandeditline.h"

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

    void updateColors();
    void updateCommands();
    void updateRegexp();
    void updatePalette();

    void showDebug(QString debug, int icon = QSystemTrayIcon::Information);

    void activated(QSystemTrayIcon::ActivationReason reason);

    void updatePeriodChange(int period);
    void updateLinesChange(int lines);
    void updateOnOff(bool checked);

    void updateSettings();

    void updateShowLines();

    void clear();

protected:
    void createTrayIcon();
    void createActions();

    void closeEvent(QCloseEvent *event);

    void processFile();

    void keyPressEvent(QKeyEvent *event);

private:
    SettingsDialog settings;

    QTimer timer;

//    QLineEdit lineEdit;
    CommandEditLine lineEdit;
    QTextBrowser browser;

    ColorParser *colorParser;
    CommandParser *commandParser;
    RegexpParser *regexpParser;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction *updateColorsAction;
    QAction *updateCommandsAction;
    QAction *updateRegexpAction;
    QAction *updatePaletteAction;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QVector<CustomButton*> buttons;
    QHBoxLayout *buttonLayout;

    qint64 fileSize;
    qint64 lineNumber;

    QCheckBox updateOn;

    QSpinBox updatePeriod;
    QSpinBox updateLines;
    QCheckBox showLines;

    QList<CommandHandler*> commands;

    QFile file;
    QTextStream in;

    QStringList buffer;
};

#endif // WIDGET_H
