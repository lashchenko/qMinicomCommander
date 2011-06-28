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

//    enum ColorRole { WindowText, Button, Light, Midlight, Dark, Mid,
//                     Text, BrightText, ButtonText, Base, Window, Shadow,
//                     Highlight, HighlightedText,
//                     Link, LinkVisited, // ### Qt 5: remove
//                     AlternateBase,
//                     NoRole, // ### Qt 5: value should be 0 or -1
//                     ToolTipBase, ToolTipText,
//                     NColorRoles = ToolTipText + 1,
//                     Foreground = WindowText, Background = Window // ### Qt 5: remove
//                   };

    QPalette p = QPalette();
    p.setColor(QPalette::Base, QColor("#222222"));
    p.setColor(QPalette::Text, QColor("#949494"));

    p.setColor(QPalette::Button, QColor("#333333"));
    p.setColor(QPalette::ButtonText, QColor("#64665F"));

    p.setColor(QPalette::Highlight, QColor("#333333"));
    p.setColor(QPalette::HighlightedText, QColor("#439A00"));

    p.setColor(QPalette::ToolTipBase, QColor("#222222"));
    p.setColor(QPalette::ToolTipText, QColor("#64665F"));//949494

    p.setColor(QPalette::Window, QColor("#222222"));
    p.setColor(QPalette::WindowText, QColor("#64665F"));

    p.setColor(QPalette::Shadow, QColor("#000000"));


    qApp->setPalette(p);

    qApp->setStyle(new QPlastiqueStyle());
//    qApp->setPalette(QColor("#2d2d2d"));
//    qApp->setPalette(Qt::darkGray);
//    QApplication::setWindowIcon()
//    qApp->setWindowIcon(QIcon(":/img/rabbit.png"));
    qApp->setWindowIcon(QIcon(":/img/pig.png"));

    return a.exec();
}
