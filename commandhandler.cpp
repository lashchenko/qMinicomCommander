#include "commandhandler.h"

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

QString CommandHandler::getCommand() const
{
    return command;
}

MinicomHanndler::MinicomHanndler(QString cmd)
    : CommandHandler(cmd)
{
}

void MinicomHanndler::process()
{
    std::cout << "MINICOM : " << command.toStdString() << std::endl;
}

BashHanndler::BashHanndler(QString cmd)
    : CommandHandler(cmd)
{
}

void BashHanndler::process()
{
    std::cout << "BASH : "<< command.toStdString() << std::endl;

    QTemporaryFile file;
    if (!file.open()) {
        // file.fileName() returns the unique file name
        return;
    }
    file.close(); // ??

    QFile data(file.fileName());
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out << command;
    }
    data.close();
    data.flush();

    QProcess process;
    process.setStandardInputFile(file.fileName());
    process.start("bash");

    if( next ) {
        bool ok;
        int value = next->getCommand().toInt(&ok, 10);
        if( ok ) {
            process.waitForFinished(value);
        } else {
            process.waitForFinished();
        }
    }
}

WaitHanndler::WaitHanndler(QString cmd)
    : CommandHandler(cmd)
{
}

void WaitHanndler::process()
{
//    std::cout << "WAIT : " << command.toStdString() << std::endl;
//    bool ok;
//    int value = command.toInt(&ok, 10);
//    if( ok ) {
//        sleep(value);
//    }
}
