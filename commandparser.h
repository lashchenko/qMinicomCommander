#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QtCore>

#include "commandhandler.h"

class CommandParser
{
public:
    CommandParser(QString fileName);

//private:
    QMap< QString, QList<CommandHandler*> > handlers;
};

#endif // COMMANDPARSER_H
