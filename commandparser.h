#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QtCore>
#include <QtGui>
#include <QSystemTrayIcon>

#include "commandhandler.h"
//#include "widget.h"

class CommandParser
{
public:
    CommandParser(QWidget *w);
    void parse(QString fileName);

//private:

    QMap< QString, QList<CommandHandler*> > handlers;
    QMap< QString, QString > tips;
    QMap< QString, QString > rich;

    QWidget *widget;
};

#endif // COMMANDPARSER_H
