#include "commandhandler.h"
#include <QDebug>
#include <iostream>

#include <stdio.h>
#include <time.h>
#include <ctime>

CommandHandler::CommandHandler(QString cmd)
{
    command = cmd;
    isEnabled = false;
}

void CommandHandler::setPrev(CommandHandler *p)
{
    prev = p;
}

void CommandHandler::setNext(CommandHandler *n)
{
    next = n;
}

void CommandHandler::setSystemTray(QSystemTrayIcon *systemTray)
{
    tray = systemTray;
}

QString CommandHandler::getCommand() const
{
    return command;
}

QString CommandHandler::getId() const
{
    return id;
}

void CommandHandler::connecting()
{
    if( prev ) {
        qDebug() << "connect  __ " + prev->getCommand() + " __  finished to  __ " + this->getCommand();
        QObject::connect(prev, SIGNAL(finished()), this, SLOT(start()), Qt::QueuedConnection);
        prev->setNext(this);

    }
}

void CommandHandler::run()
{
//    if( !prev ) { // first handler
//        disconnect(0,0,this, SLOT(start()));
//    }
}

void CommandHandler::debug(QString info)
{
    std::cerr << command.toStdString() << std::endl;
    std::cerr << QTime::currentTime().toString().toStdString() << " : " << "debug information: " << info.toStdString() << std::endl;
}

void CommandHandler::setEnabled(bool enable)
{
    isEnabled = enable;
    if( next ) {
        next->setEnabled(enable);
    }
}

int CommandHandler::getPeriod()//CommandHandler *pNext)
{
    if( next ) {
        WaitHanndler *waitHandler = qobject_cast<WaitHanndler*>(next);
        if( waitHandler ) {
            return waitHandler->period();
        }
        MessageHanndler *messageHanndler = qobject_cast<MessageHanndler*>(next);
        if( messageHanndler ) {
            return messageHanndler->getPeriod();
        }
    }

    return -1;
}


MinicomHanndler::MinicomHanndler(QString cmd)
    : CommandHandler(cmd)
{
    id = "minicom:";
}

void MinicomHanndler::run()
{
    CommandHandler::run();

    if( !isEnabled ) {
        debug("--- skip MinicomHanndler::run()");
        emit finished();
        return;
    }

    debug("minicom thread start");

    std::cout << command.toStdString() << std::endl;

    debug(command);
    debug("minicom thread finish");
}

BashHanndler::BashHanndler(QString cmd)
    : CommandHandler(cmd)
{
    id = "bash:";
}

void BashHanndler::run()
{
    CommandHandler::run();

    if( !isEnabled ) {
        debug("--- skip BashHanndler::run()");
        emit finished();
        return;
    }

    debug("bash thread start");

    QTemporaryFile file;
    file.setAutoRemove(false);
    if (!file.open()) {
        return;
    }

    QFile data(file.fileName());
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out << command;
    }
    data.close();
    data.flush();

    QProcess *process = new QProcess();
    process->setStandardInputFile(file.fileName());
    process->setStandardOutputFile("/home/alashchenko/ooooooo.txt");

    time_t before = time(NULL);

    process->start("bash");
    process->waitForFinished( getPeriod() );

    time_t after = time(NULL);

    process->terminate();
    delete process;

    if( after - before < getPeriod() ) {
        std::cerr << "BEFORE = " << before << " : AFTER = " << after << std::endl;

        setEnabled(false);

        emit showMessage(trUtf8("command %1 fail. W.T.F. ?")
                         .arg(command), QSystemTrayIcon::Warning);
    }

//    QTime t3 = t2 -t1;

    debug("process finish");
    for( ;; ) {
        yieldCurrentThread();
    }
}


WaitHanndler::WaitHanndler(QString cmd)
    : CommandHandler(cmd)
{
    id = "wait:";
}

int WaitHanndler::period() const
{
    bool ok;
    int value = command.toInt(&ok, 10);
    if( ok ) {
        return value * 1000;
    } else {
        return 0;
    }
}

void WaitHanndler::run()
{
    CommandHandler::run();

    if( !isEnabled ) {
        debug("--- skip WaitHanndler::run()");
        emit finished();
        return;
    }

    debug("wait thread start");

    bool ok;
    int value = command.toInt(&ok, 10);
    if( ok ) {
        sleep(value);
    }
    if( prev ) {
        prev->terminate();
    }

    debug("wait thread finish");
}

void WaitHanndler::connecting()
{
    if( prev ) {
//        prev->disconnect(commands.last());
        // This thread has been running parallel with a previous thread.
        // After this thread has been finished -> starting next thread.
        qDebug() << "connect  __ " + prev->getCommand() + " __  started to  __ " + this->getCommand();
        QObject::connect(prev, SIGNAL(started()), this, SLOT(start()), Qt::QueuedConnection);
    }

//    QObject::connect(this, SIGNAL(showMessage(QString,int)),widget, SLOT(showDebug(QString,int)),Qt::QueuedConnection);
}


MessageHanndler::MessageHanndler(QString cmd)
    : CommandHandler(cmd)
{
    id = "message:";
}

void MessageHanndler::run()
{
    CommandHandler::run();

    if( !isEnabled ) {
        debug("--- skip MessageHanndler::run()");
        emit finished();
        return;
    }

    emit(showMessage(command));
}
