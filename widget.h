#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>

#include "colorparser.h"
#include "commandparser.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void updateText();
    void runCommand();
    void runCommand(QString commandKey);

private:
    QTimer timer;
    QTextEdit browser;
    QLineEdit lineEdit;

    quint64 fileSize;

    ColorParser *colorParser;
    CommandParser *commandParser;
};

#endif // WIDGET_H
