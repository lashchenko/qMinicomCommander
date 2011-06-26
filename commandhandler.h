#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <QtCore>
#include <QSystemTrayIcon>

/*
 *
 */
class CommandHandler : public QThread
{
    Q_OBJECT
public:
    CommandHandler(QString cmd);

    void setPrev(CommandHandler *p);
    void setNext(CommandHandler *n);

    void setSystemTray(QSystemTrayIcon *systemTray);

    QString getCommand() const;

    void debug(QString text);


signals:
    void showMessage(QString info);

protected:
    void run();

protected:
    QString command;
    CommandHandler *prev;
    CommandHandler *next;
    QSystemTrayIcon *tray;
};


/*
 *
 */
class MinicomHanndler : public CommandHandler
{
    Q_OBJECT
public:
    MinicomHanndler(QString cmd);

protected:
    void run();
};


/*
 *
 */
class BashHanndler : public CommandHandler
{
    Q_OBJECT
public:
    BashHanndler(QString cmd);

protected:
    void run();
};


/*
 *
 */
class WaitHanndler : public CommandHandler
{
    Q_OBJECT
public:
    WaitHanndler(QString cmd);

protected:
    void run();
};

/*
 *
 */
class MessageHanndler : public CommandHandler
{
    Q_OBJECT
public:
    MessageHanndler(QString cmd);

protected:
    void run();
};

#endif // COMMANDHANDLER_H