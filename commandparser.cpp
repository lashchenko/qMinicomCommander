#include "commandparser.h"

#include <QDebug>

CommandParser::CommandParser(QWidget *w)
{
    widget = w;
}

void CommandParser::parse(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    QString line;
    bool isNeedReadLine = true;

    while (!in.atEnd() ){

        if( isNeedReadLine ) {
            line = in.readLine();
        } else {
            isNeedReadLine = true;
        }

        //     0       1          2
        // #command: dtRun : <b>dt run</b>
        // ...
        // bash: build
        // command: dtRun
        if( line.startsWith("#command:") ) {
            QStringList words = line.split(":");
//            line = line.remove("#command: ");


//            QString commandKey = line;
            QString commandKey = words.at(1).trimmed();

//            qDebug() << commandKey << " ++++++++++++++ added command";

            QString richText = commandKey;
            if( (words.size() > 2) && (words.at(2).isEmpty() == false) ) {
                richText = words.at(2);
            }
            QList<CommandHandler*> commands;
            QString comments;

            CommandHandler *prev = 0;
            while (!in.atEnd()) {
                QString command = in.readLine();

                bool isNeed = false;

                if( command.startsWith("minicom: ") ) {
                    commands.append(new MinicomHanndler(command.remove("minicom: ")));
                    isNeed = true;
                }

                else if( command.startsWith("bash: ") ) {
                    commands.append(new BashHanndler(command.remove("bash: ")));
                    isNeed = true;
                }

                else if( command.startsWith("message: ") ) {
                    commands.append(new MessageHanndler(command.remove("message: ")));
                    isNeed = true;
                }

                else if( command.startsWith("wait: ") ) {
                    commands.append(new WaitHanndler(command.remove("wait: ")));
                    commands.last()->setPrev(prev);

                    if( prev ) {
                        prev->disconnect(commands.last());
                        // This thread has been running parallel with a previous thread.
                        // After this thread has been finished -> starting next thread.
                        qDebug() << "connect  __ " + prev->getCommand() + " __  started to  __ " + commands.last()->getCommand();
                        QObject::connect(prev, SIGNAL(started()), commands.last(), SLOT(start()), Qt::QueuedConnection);
                        prev->setNext(commands.last());
                    }

                    prev = commands.last();
                    commands.last()->setNext(0);
                    QObject::connect(commands.last(), SIGNAL(showMessage(QString,int)),widget, SLOT(showDebug(QString,int)),Qt::QueuedConnection);
                }

                else if( command.startsWith("command: ") ) {
                    QString k = command.replace("command: ", "").trimmed();

//                    qDebug() << k << " _______________________ using command";
//                    commands.append(handlers.value(k));
                    QList<CommandHandler*> listOfNewHandlers = handlers.value(k);
                    foreach( CommandHandler *h, listOfNewHandlers ) {

                        WaitHanndler *waitHandler = qobject_cast<WaitHanndler*>(h);
                        if( waitHandler ) {
                            commands.append(new WaitHanndler(waitHandler->getCommand()));
                        }

                        MessageHanndler *messageHandler = qobject_cast<MessageHanndler*>(h);
                        if( messageHandler ) {
                            commands.append(new MessageHanndler(messageHandler->getCommand()));
                        }

                        BashHanndler *bashHandler = qobject_cast<BashHanndler*>(h);
                        if( bashHandler ) {
                            commands.append(new BashHanndler(bashHandler->getCommand()));
                        }

                        MinicomHanndler *minicomHandler = qobject_cast<MinicomHanndler*>(h);
                        if( minicomHandler ) {
                            commands.append(new MinicomHanndler(minicomHandler->getCommand()));
                        }

                        if( waitHandler ) {
                            commands.last()->setPrev(prev);

                            if( prev ) {
                                prev->disconnect(commands.last());
                                // This thread has been running parallel with a previous thread.
                                // After this thread has been finished -> starting next thread.
                                qDebug() << "connect  __ " + prev->getCommand() + " __  started to  __ " + commands.last()->getCommand();
                                QObject::connect(prev, SIGNAL(started()), commands.last(), SLOT(start()), Qt::QueuedConnection);
                                prev->setNext(commands.last());
                            }

                            prev = commands.last();
                            commands.last()->setNext(0);
                            QObject::connect(commands.last(), SIGNAL(showMessage(QString,int)),widget, SLOT(showDebug(QString,int)),Qt::QueuedConnection);
                        } else {
                            commands.last()->setPrev(prev);

                            if( prev ) {
                                qDebug() << "connect  __ " + prev->getCommand() + " __  finished to  __ " + commands.last()->getCommand();
                                QObject::connect(prev, SIGNAL(finished()), commands.last(), SLOT(start()), Qt::QueuedConnection);
                                prev->setNext(commands.last());

                            }

                            prev = commands.last();
                            commands.last()->setNext(0);
                            QObject::connect(commands.last(), SIGNAL(showMessage(QString,int)),widget, SLOT(showDebug(QString,int)),Qt::QueuedConnection);
                        }
                    }

                    if( prev ) {
                        prev->setNext(handlers.value(k).first());

                        WaitHanndler *waitHandler = qobject_cast<WaitHanndler*>(handlers.value(k).first());
                        if( waitHandler ) {
                            qDebug() << "connect  __ " + prev->getCommand() + " __  started to  __ " + handlers.value(k).first()->getCommand();
                            QObject::connect(prev, SIGNAL(started()), handlers.value(k).first(), SLOT(start()), Qt::QueuedConnection);
                        } else {
                            qDebug() << "connect  __ " + prev->getCommand() + " __ finished to __ " + handlers.value(k).first()->getCommand();
                            QObject::connect(prev, SIGNAL(finished()), handlers.value(k).first(), SLOT(start()), Qt::QueuedConnection);
                        }
                    }
                    handlers.value(k).first()->setPrev(prev);
                    handlers.value(k).last()->setNext(0);
                    prev = handlers.value(k).last();
                }

                if( isNeed ) {
                    commands.last()->setPrev(prev);

                    if( prev ) {
                        qDebug() << "connect  __ " + prev->getCommand() + " __  finished to  __ " + commands.last()->getCommand();
                        QObject::connect(prev, SIGNAL(finished()), commands.last(), SLOT(start()), Qt::QueuedConnection);
                        prev->setNext(commands.last());

                    }

                    prev = commands.last();
                    commands.last()->setNext(0);
                    QObject::connect(commands.last(), SIGNAL(showMessage(QString,int)),widget, SLOT(showDebug(QString,int)),Qt::QueuedConnection);

                }

                if( command.startsWith("// ") ) {
                    comments += command.remove("// ");
                }

                if( command.startsWith("#command: ") ) {
                    isNeedReadLine = false;
                    line = command;
                    break;
                }
            }

            qDebug() << "********************************************";

            handlers.insert(commandKey, commands);
            tips.insert(commandKey, comments);
            rich.insert(commandKey, richText);
        }
    }

    file.close();
}
