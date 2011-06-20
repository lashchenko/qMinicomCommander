#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QtCore>

class CommandHandler
{
public:
    CommandHandler(QString cmd);
    virtual void process() = 0;
protected:
    QString command;
};

class MinicomHanndler : public CommandHandler
{
public:
    MinicomHanndler(QString cmd);
    void process();
};

class BashHanndler : public CommandHandler
{
public:
    BashHanndler(QString cmd);
    void process();
};

class WaitHanndler : public CommandHandler
{
public:
    WaitHanndler(QString cmd);
    void process();
};

#endif // COMMANDHANDLER_H
