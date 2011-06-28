#include "commandhandler.h"
#include <QDebug>
#include <iostream>

#include <stdio.h>
#include <time.h>
#include <ctime>

CommandHandler::CommandHandler(QString cmd)
{
    command = cmd;
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

void CommandHandler::run()
{
    if( !prev ) { // first handler
        disconnect(0,0,this, SLOT(start()));
    }
}

void CommandHandler::debug(QString info)
{
    std::cerr << command.toStdString() << std::endl;
    std::cerr << QTime::currentTime().toString().toStdString() << " : " << "debug information: " << info.toStdString() << std::endl;
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
}

void MinicomHanndler::run()
{
    CommandHandler::run();

    debug("minicom thread start");

    std::cout << command.toStdString() << std::endl;

    debug(command);
    debug("minicom thread finish");
}

BashHanndler::BashHanndler(QString cmd)
    : CommandHandler(cmd)
{
}

void BashHanndler::run()
{
    CommandHandler::run();

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

    std::cerr << "BEFORE = " << before << " : AFTER = " << after << std::endl;
    if( after - before < getPeriod() ) {
//        std::cerr << "BEFORE = " << before << " : AFTER = " << after << std::endl;
        emit showMessage(trUtf8("команда %1 выполнялась меньше, чем Вы задавали в файле конфигурации. Возможно произошла фигня. Проверьте файл конфигурации.")
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



MessageHanndler::MessageHanndler(QString cmd)
    : CommandHandler(cmd)
{
}

void MessageHanndler::run()
{
    CommandHandler::run();

    emit(showMessage(command));
}
