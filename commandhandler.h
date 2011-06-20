#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QtCore>

class CommandHandler
{
public:
    CommandHandler(QString cmd);

    void setPrev(CommandHandler *p);
    void setNext(CommandHandler *n);

    QString getCommand() const;

    virtual void process() = 0;

protected:
    QString command;
    CommandHandler *prev;
    CommandHandler *next;
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
