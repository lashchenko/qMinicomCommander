#include "commandparser.h"

#include <QDebug>

CommandParser::CommandParser(QWidget *w)
{
    widget = w;
}

void CommandParser::processLine(QString line)
{
    int commandsSize = commands.size();

    if( line.startsWith("message: ") ) {
        commands.append( new MessageHanndler(line.remove("message: ")) );
    }

    if( line.startsWith("wait: ") ) {
        commands.append( new WaitHanndler(line.remove("wait: ")) );
    }

    if( line.startsWith("minicom: ") ) {
        commands.append( new MinicomHanndler(line.remove("minicom: ")) );
    }

    if( line.startsWith("bash: ") ) {
        commands.append( new BashHanndler(line.remove("bash: ")) );
    }

    if( commandsSize < commands.size() ) {
        commands.last()->setPrev(prev);
        commands.last()->connecting();

        prev = commands.last();

        QObject::connect(commands.last(), SIGNAL(showMessage(QString,int)),widget, SLOT(showDebug(QString,int)),Qt::QueuedConnection);

        return;
    }


    if( line.startsWith("command: ") ) {
        QString key = line.replace("command: ", "").trimmed();

        qDebug() << key << " <<<-------------- using command";

        QList<CommandHandler*> listOfNewHandlers = handlers.value(key);
        foreach( CommandHandler *h, listOfNewHandlers ) {
            processLine(h->getId() + " " + h->getCommand());
        }
    }
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

        if( line.startsWith("#command:") ) {
            QStringList words = line.split(":");

            QString commandKey = words.at(1).trimmed();

            qDebug() << commandKey << " ++++++++++++++ added command";

            QString richText;
            if( (words.size() > 2) && (words.at(2).isEmpty() == false) ) {
                richText = words.at(2);
            }

            commands.clear();
            comments.clear();

            prev = 0;

            while (!in.atEnd()) {
                QString command = in.readLine();

                processLine(command);

                if( command.startsWith("// ") ) {
                    comments += command.remove("// ");
                }

                if( command.startsWith("#command: ") ) {
                    isNeedReadLine = false;
                    line = command;
                    break;
                }
            }

            qDebug();
            qDebug() << "********************************************";
            qDebug();

            if( !commands.isEmpty() ) {
                // if not empty lines at end of config file
                handlers.insert(commandKey, commands);

                if( !comments.isEmpty() ) {
                    tips.insert(commandKey, comments);
                }
                if( !richText.isEmpty() ) {
                    rich.insert(commandKey, richText);
                }
            }
        }
    }

    file.close();
}
