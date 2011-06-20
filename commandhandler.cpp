#include "commandhandler.h"

#include <iostream>

CommandHandler::CommandHandler(QString cmd)
{
    command = cmd;
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
}

WaitHanndler::WaitHanndler(QString cmd)
    : CommandHandler(cmd)
{
}

void WaitHanndler::process()
{
    std::cout << "WAIT : " << command.toStdString() << std::endl;
}
