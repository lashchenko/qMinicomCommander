#include "commandparser.h"


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

        if( line.startsWith("#command: ") ) {
            line = line.remove("#command: ");

            QString commandKey = line;
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

                if( command.startsWith("bash: ") ) {
                    commands.append(new BashHanndler(command.remove("bash: ")));
                    isNeed = true;
                }

                if( command.startsWith("message: ") ) {
                    commands.append(new MessageHanndler(command.remove("message: ")));
                    isNeed = true;
                }

                if( command.startsWith("wait: ") ) {
                    commands.append(new WaitHanndler(command.remove("wait: ")));
                    commands.last()->setPrev(prev);

                    if( prev ) {
                        prev->disconnect(commands.last());
                        // This thread has been running parallel with a previous thread.
                        // After this thread has been finished -> starting next thread.
                        QObject::connect(prev, SIGNAL(started()), commands.last(), SLOT(start()), Qt::QueuedConnection);
                    }

                    prev = commands.last();
                    commands.last()->setNext(0);
                    QObject::connect(commands.last(), SIGNAL(showMessage(QString)), widget, SLOT(showDebug(QString)));
                }

                if( isNeed ) {
                    commands.last()->setPrev(prev);

                    if( prev ) {
                        QObject::connect(prev, SIGNAL(finished()), commands.last(), SLOT(start()), Qt::QueuedConnection);
                    }

                    prev = commands.last();
                    commands.last()->setNext(0);
                    QObject::connect(commands.last(), SIGNAL(showMessage(QString)), widget, SLOT(showDebug(QString)));
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

            handlers.insert(commandKey, commands);
            tips.insert(commandKey, comments);
        }
    }

    file.close();
}
