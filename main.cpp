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
//    qApp->setPalette(QColor("#838B8B"));
    qApp->setPalette(Qt::darkGray);

    return a.exec();
}
