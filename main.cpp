#include <QtGui/QApplication>

#include <iostream>

#include "widget.h"

/**
  * run as:
  * # qMinicomCommander | minicom > miniout.txt
  *
  * colors.cfg - format:
  * keyword:bgColor:textColor
  * example:
  *     root:#f00:#000
  *     user:lightslategray:red
  * commands.cfg - format:
  * #command: commandName
  * bash: ls -la
  * // comment
  * wait: 10
  * message: message show in system tray
  * minicom: printenv
  *
  **/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.showMaximized();

    qApp->setStyle(new QPlastiqueStyle());
    qApp->setPalette(QColor("#2d2d2d"));
//    qApp->setPalette(Qt::darkGray);
//    QApplication::setWindowIcon()
//    qApp->setWindowIcon(QIcon(":/img/rabbit.png"));
    qApp->setWindowIcon(QIcon(":/img/pig.png"));

    return a.exec();
}
