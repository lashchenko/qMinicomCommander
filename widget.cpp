#include "widget.h"

#include "colorparser.h"
#include "commandparser.h"
#include "custombutton.h"

#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;

    commandParser = new CommandParser("/Users/alashchenko/commands.cfg");
    QList<QString> keys = commandParser->handlers.keys();

    QHBoxLayout *hl = new QHBoxLayout;
    foreach( QString key, keys ) {
        CustomButton *button = new CustomButton(key);
        button->setPalette(QColor("lightslategray"));
        connect(button, SIGNAL(clicked(QString)), this, SLOT(runCommand(QString)));
        hl->addWidget(button);
    }
    vl->addLayout(hl);

    QPushButton *button = new QPushButton("run command");
    button->setPalette(QColor("darkorange"));
    connect(button, SIGNAL(clicked()), this, SLOT(runCommand()));

    hl = new QHBoxLayout;
    hl->addWidget(&lineEdit);
    hl->addWidget(button);


    vl->addLayout(hl);
    vl->addWidget(&browser);

    setLayout(vl);

    fileSize = 0;

    colorParser = new ColorParser("/Users/alashchenko/color.cfg");

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateText()));
    timer.start(1000);

}

Widget::~Widget()
{

}

void Widget::runCommand(QString commandKey)
{
    QList<CommandHandler*> commands = commandParser->handlers.value(commandKey);
    foreach( CommandHandler *handler, commands ) {
        handler->process();
    }
}

void Widget::updateText()
{
    QFile file("/Users/alashchenko/Development/Sources/Qt/qMinicomCommander-build-desktop/o.txt");

    if( fileSize == file.size() ) {
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    QString text;


    while (!in.atEnd()) {
        QString line = in.readLine();

        line = line.replace(QRegExp("\\[\\d+\\;\\d+\\w"), "")
               .replace(QRegExp("\\[\\K"), "")
               .replace(QRegExp("\\(\\B\\[\\d\\m"), "")
               .replace(QRegExp("\\[\\d\\m\\(\\B"), "")
               .replace(QRegExp("\\[\\d\\m"), "")
               .replace(QRegExp("\\(\\B"), "");

        text.append(colorParser->processString(line));
    }

    if( fileSize != file.size() ) {

        browser.setText(text);
        QScrollBar *s = browser.verticalScrollBar();
        s->setValue(s->maximum());

    }

    fileSize = file.size();
    file.close();

//    if( fileSize > 1024 ..... )
}

void Widget::runCommand()
{
    std::cout << lineEdit.text().toStdString() << std::endl;
}
