#include "commandeditline.h"

CommandEditLine::CommandEditLine(QWidget *parent) :
    QLineEdit(parent)
{
    current = -1;
}


void CommandEditLine::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "------------------------------------------------------";
    qDebug() << commands;
    qDebug() << event->key();
    qDebug() << "------------------------------------------------------";

    if( event->key() == Qt::Key_Up ) {

        qDebug() << "Key_Up";
        if( commands.isEmpty() ) {
            return;
        }

        qDebug() << "current [1]:" << current;
        --current;
        qDebug() << "current [2]:" << current;

        if( current < 0 ) {
            QLineEdit::setText("");
            current = -1;
            qDebug() << "current [ ]:" << current;

        } else if( current < commands.size() ){
            QLineEdit::setText(commands.at(current));

        } else {
            QLineEdit::setText("");
            current = commands.size();
            qDebug() << "current [ ]:" << current;
        }

    } else if( event->key() == Qt::Key_Down ) {

        qDebug() << "Key_Down";
        if( commands.isEmpty() ) {
            return;
        }

        qDebug() << "current [1]:" << current;
        ++current;
        qDebug() << "current [2]:" << current;

        if( current < 0 ) {
            QLineEdit::setText("");
            current = -1;
            qDebug() << "current [ ]:" << current;

        } else if( current < commands.size() ){
            QLineEdit::setText(commands.at(current));

        } else {
            QLineEdit::setText("");
            current = commands.size();
            qDebug() << "current [ ]:" << current;
        }

    } else if( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return ) {

        qDebug() << "Key_Enter";
        bool isNew = true;
        foreach (QString command, commands) {
            if( command == text() ) {
                isNew = false;
                break;
            }
        }

        if( setCommand(text()) ) {
            current = commands.size();
            qDebug() << "current [ ]:" << current;
        }


        emit enterPressed();

//    } else if( event->key() == Qt::Key_Control ) { //0x01000021)
//            event->k
//            keyPressEvent();
//            QLineEdit::setText(text()+ " ");
    } else {
        qDebug() << "ELSE  ::: " + event->key();
        QLineEdit::keyPressEvent(event);
    }
}

void CommandEditLine::setText(const QString &command)
{
    QLineEdit::setText(command);
    setCommand(command);
}


bool CommandEditLine::setCommand(const QString &command)
{
    foreach (QString t, commands) {
        if( t == command ) {
            qDebug() << t << " == " << command;
            return false;
        }
    }
    commands.append(command);
    return true;
}
