#include "commandparser.h"


CommandParser::CommandParser(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    QString line;
    bool isNeedReadLine = true;

    // end of file or last line
    while (!in.atEnd() ){//|| !isNeedReadLine) {

        if( isNeedReadLine ) {
            line = in.readLine();
        } else {
            isNeedReadLine = true;
        }

        if( line.startsWith("#command: ") ) {
            line = line.remove("#command: ");

            QString commandKey = line;
            QList<CommandHandler*> list;

            CommandHandler *prev = 0;
            while (!in.atEnd()) {
                QString command = in.readLine();

                /// TODO: IN FUTURE TIME :
//                for( QString key : keys ) {
//                    if( start(key) ) {
//                        command = remove(key+": ");
//                        list.append(factory.createHandler(key, command);
//                    }
//                }

                if( command.startsWith("minicom: ") ) {
                    list.append(new MinicomHanndler(command.remove("minicom: ")));
                    list.last()->setPrev(prev);
                    prev = list.last();
                    list.last()->setNext(0);
                }

                if( command.startsWith("bash: ") ) {
                    list.append(new BashHanndler(command.remove("bash: ")));
                    list.last()->setPrev(prev);
                    prev = list.last();
                    list.last()->setNext(0);
                }

                if( command.startsWith("wait: ") ) {
                    list.append(new WaitHanndler(command.remove("wait: ")));
                    list.last()->setPrev(prev);
                    prev = list.last();
                    list.last()->setNext(0);
                }

                if( command.startsWith("#command: ") ) {
                    isNeedReadLine = false;
                    line = command;
                    break;
                }
            }

            handlers.insert(commandKey, list);
        }
    }
}
