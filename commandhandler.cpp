#include "commandhandler.h"
#include <QDebug>
#include <iostream>

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

}

void CommandHandler::debug(QString info)
{
    std::cerr << "debug information: " << info.toStdString() << std::endl;
}

MinicomHanndler::MinicomHanndler(QString cmd)
    : CommandHandler(cmd)
{
}

void MinicomHanndler::run()
{
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

    QProcess process;
    process.setStandardInputFile(file.fileName());
    process.setStandardOutputFile("/home/alashchenko/ooooooo.txt");

    process.start("bash");
    process.waitForFinished(-1);

    for( ;; ) {
        yieldCurrentThread();
    }
}


WaitHanndler::WaitHanndler(QString cmd)
    : CommandHandler(cmd)
{
}


void WaitHanndler::run()
{
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
    emit(showMessage(command));
}
