#include <QtGui/QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
//    w.show();
    w.showMaximized();

//    qApp->setStyle(new QPlastiqueStyle());
//    qApp->setPalette(Qt::darkGray);

    return a.exec();
}
