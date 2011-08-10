#include "commandhandler.h"

#include "settingsdialog.h"

#include <QDebug>
#include <iostream>

#include <stdio.h>
#include <time.h>
#include <ctime>

CommandHandler::CommandHandler(QString cmd)
{
    command = cmd;
    isEnabled = false;

    prev = 0;
    next = 0;
}

void CommandHandler::setPrev(CommandHandler *p)
{
    prev = p;
    if( prev ) {
        prev->setNext(this);
    }
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
//        qDebug() <<   "connect  __ "
//                    + prev->getId() + " " + prev->getCommand()
//                    + " __  finished to  __ "
//                    + this->getId() + " " + this->getCommand();
        QObject::connect(prev, SIGNAL(finished()), this, SLOT(start()), Qt::QueuedConnection);
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
    std::cerr << "*******************************************************************************" << std::endl;
    std::cerr << QTime::currentTime().toString().toStdString() << " : " << "debug information: " << info.toStdString() << std::endl;
    std::cerr << "\tcommand : " << command.toStdString() << std::endl;
    std::cerr << "*******************************************************************************" << std::endl;
}

void CommandHandler::setEnabled(bool enable)
{
    isEnabled = enable;
    if( next ) {
        next->setEnabled(enable);
//        debug(id + " next true!!!!!!!!!!!!!!!!" );
    }

//    debug(id + " " + command + " isEnamled: " + isEnabled);
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

const QString MinicomHanndler::MinicomID = "123";

MinicomHanndler::MinicomHanndler(QString cmd)
    : CommandHandler(cmd)
{
    id = "minicom:";
}

void MinicomHanndler::run()
{
    CommandHandler::run();

    if( !isEnabled ) {
        debug("--- skip MinicomHanndler::run() " + command);
        emit finished();
        return;
    }

    debug("minicom thread start");

    std::cout << command.toStdString() << std::endl;

    debug(command);
    debug("minicom thread finish");
}





QString BashHanndler::outputFileName = "";

BashHanndler::BashHanndler(QString cmd)
    : CommandHandler(cmd)
{
    id = "bash:";
}

void BashHanndler::run()
{
    CommandHandler::run();

    if( !isEnabled ) {
        debug("--- skip BashHanndler::run() " + command);
        emit finished();
        return;
    }

    debug("bash thread start");

    if( next && next->getId() == "wait:") {
        while( !next->isRunning()) {
            yieldCurrentThread();
        }
    }

    QTemporaryFile file;
    file.setAutoRemove(false);
    qDebug() << "temp ___i1 name : " << file.fileName();
    if (!file.open()) {
        return;
    }
    qDebug() << "temp ___i2 name : " << file.fileName();

//    QTemporaryFile file;
    QTemporaryFile outputFile;
    outputFile.setAutoRemove(false);
    if (outputFile.open()) {
        BashHanndler::outputFileName = outputFile.fileName();
        qDebug() << "temp name : " << BashHanndler::outputFileName;
//        return;
    }
    BashHanndler::outputFileName = outputFile.fileName();
    qDebug() << "temp name : " << BashHanndler::outputFileName;

    QFile data(file.fileName());
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        out << command;
    }
    data.close();
    data.flush();

//    setEnabled(false);

    QProcess *process = new QProcess();
    process->setStandardInputFile(file.fileName());
    process->setStandardOutputFile(outputFile.fileName());//"/home/alashchenko/ooooooo.txt");

//    time_t before = time(NULL);

    process->start("bash");
    process->waitForFinished(-1);// getPeriod() );



    // process will be terminated only from WaitHandler
//    if( isEnabled ) {
//        emit showMessage(trUtf8("command %1 fail. W.T.F. ?")
//                         .arg(command), QSystemTrayIcon::Warning);
//    }

    if( process->exitCode() != QProcess::NormalExit ) {
        emit showMessage(trUtf8("command %1 fail. W.T.F. ?").arg(command), QSystemTrayIcon::Warning);
    }

    process->terminate();
    delete process;


//    time_t after = time(NULL);

//    process->terminate();
//    delete process;

//    if( after - before < getPeriod() ) {
//        std::cerr << "BEFORE = " << before << " : AFTER = " << after << std::endl;

//        setEnabled(false);

//        emit showMessage(trUtf8("command %1 fail. W.T.F. ?")
//                         .arg(command), QSystemTrayIcon::Warning);
//    }

//    QTime t3 = t2 -t1;

    debug("process finish");
//    for( ;; ) {
//        yieldCurrentThread();
//    }
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
        debug("--- skip WaitHanndler::run() " + command);
        emit finished();
        return;
    }

    debug("wait thread start");

    if( command.trimmed().startsWith("-time=") ) {
        QString commandPostProcessed = command.trimmed().replace("-time=","");
        bool ok;
        int value = commandPostProcessed.toInt(&ok, 10);
        if( ok ) {
            sleep(value);
        }

        if( prev ) {
            prev->setEnabled(true);
            prev->terminate();
            debug("wait thread finish");
            return;
        }

    } else if(command.trimmed().startsWith("-string=") ) {
        QString commandPostProcessed = command.trimmed().replace("-string=","");

        QFile *file;

        if( prev && prev->getId() == "bash:" ) {
            file = new QFile(BashHanndler::outputFileName);
            qDebug() << "temp name = " << BashHanndler::outputFileName;
        } else {
            file = new QFile(SettingsDialog::getValue(OUT));
        }

        if ( file && !file->open(QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text)) {
            setEnabled(false);
            if( prev ) {
                prev->terminate();
                debug("wait thread finish");
                return;
            }
        }

        QTextStream in(file);

        for( ;; ) {
            while( !in.atEnd() ) {
                QString line = in.readLine();
                qDebug() << line;
                if( line.indexOf(commandPostProcessed) != -1) {
                    file->close();
                    if( prev ) {
                        prev->setEnabled(true);
                        prev->terminate();
                        debug("wait thread finish");
                        return;
                    }
                }
            }
            sleep(2);
        }
    } else if(command.trimmed().startsWith("-file=") ) {
        QString commandPostProcessed = command.trimmed().replace("-file=","");
        QFileInfo previous(commandPostProcessed);
        qDebug() << " previous info ::::: " << previous.created();

        for( ;; ) {

            QFileInfo current(commandPostProcessed);
            if( current.exists() ) {
                qDebug() << current.created();
                if( previous.created() != current.created() ) {
                    qDebug() << current.created() << " NOT EQUAL " << previous.created();
                    if( prev ) {
                        prev->setEnabled(true);
                        prev->terminate();
                        debug("wait thread finish");
                        return;
                    }
                } else {
                    qDebug() << current.created() << " EQUAL " << previous.created();
                }
            } else {
                qDebug() << "current not exist!";
            }
            sleep(2);
        }
    }

//    debug("wait thread finish");
}

void WaitHanndler::connecting()
{
    if( prev ) {
        // This thread has been running parallel with a previous thread.
        // After this thread has been finished -> starting next thread.
//        qDebug() <<   "connect  __ "
//                    + prev->getId() + " " + prev->getCommand()
//                    + " __  started to  __ "
//                    + this->getId() + " " + this->getCommand();
        QObject::connect(prev, SIGNAL(started()), this, SLOT(start()), Qt::QueuedConnection);
    }
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
        debug("--- skip MessageHanndler::run() " + command);
        emit finished();
        return;
    }

    emit(showMessage(command));
}
