#include <QtGui/QApplication>
#include "widget.h"

/**
  * run as:
  * # sudo qMinicomCommander | minicom > o.txt
  *
  * colors.cfg - format:
  * keyword:bgColor:textColor
  * example:
  *     root:#f00:#000
  *     user:lightslategray:red
  * commands.cfg - format:
  * #command: commandName
  * bash: ls -la
  * wait: 10
  * minicom: printenv
  *
  **/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.showMaximized();

    qApp->setStyle(new QPlastiqueStyle());
    qApp->setPalette(Qt::darkGray);

    return a.exec();
}
