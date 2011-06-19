#include "widget.h"
#include <iostream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;

    QHBoxLayout *hl = new QHBoxLayout;
    for( int i=1; i<13; ++i ) {
        QPushButton *b = new QPushButton(tr("F%1").arg(i));
        connect(b, SIGNAL(clicked()), this, SLOT(runF1()));
        hl->addWidget(b);
    }
    vl->addLayout(hl);

    button.setText("run command");
    connect(&button, SIGNAL(clicked()), this, SLOT(runCommand()));

    hl = new QHBoxLayout;
    hl->addWidget(&lineEdit);
    hl->addWidget(&button);


    vl->addLayout(hl);
    vl->addWidget(&browser);

    setLayout(vl);

    fileSize = 0;

    connect(&timer, SIGNAL(timeout()), this, SLOT(updateText()));
    timer.start(1000);
}

Widget::~Widget()
{

}

void Widget::updateText()
{
//    QTextStream stream(stdin);
//    QString line;
//    do {
//        line = stream.readLine();
//    } while (!line.isNull());
    QFile file("o.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    QString style = "<style type=\"text/css\">" \
            ".block2 { " \
            "width: 200px; " \
            "background: #fc0; " \
            "padding: 5px; " \
            "border: solid 1px black; " \
            "float: left; " \
            "position: relative; " \
            "top: 40px; " \
            "left: -70px;" \
            "}" \
            "</style>";

    QString text;
    while (!in.atEnd()) {
        QString line = in.readLine();

        QString htmlBegin = "";
        QString htmlEnd = "";
        if( line.contains(".cpp", Qt::CaseInsensitive)
                || line.contains(".h", Qt::CaseInsensitive)
                || line.contains(".java", Qt::CaseInsensitive) ) {
//            htmlBegin = "<div style=\"background:'#bca';\"> ";
//            htmlEnd = "</div>";
            htmlBegin = "<i>";
            htmlEnd = "</i>";
        } else if( line.contains("root", Qt::CaseInsensitive) ) {
//            htmlBegin = "<div style=\"background:'#efc';\"> ";
//            htmlEnd = "</div>";
            htmlBegin = "<b>";
            htmlEnd = "</b>";
        } else if( line.contains("CFE>", Qt::CaseInsensitive) ) {
            htmlBegin = "<b><i>";
            htmlEnd = "</i></b>";
        }

        htmlEnd += "<br />";
        text.append(htmlBegin + line + htmlEnd);
    }


//    if( browser.toHtml().contains(text) == false) {
    if( fileSize != file.size() ) {

//        std::cout << "TEXT________________________" << std::endl;
//        std::cout << text.toStdString() << std::endl;
//        std::cout << "BROWSER_____________________" << std::endl;
//        std::cout << browser.toHtml().toStdString() << std::endl;
//        std::cout << "****************************" << std::endl;

        browser.setText(text);
//        browser.setText("<pre>" + text + "</pre>");
        QScrollBar *s = browser.verticalScrollBar();
        s->setValue(s->maximum());
    }

    fileSize = file.size();

    file.close();

//    if( fileSize > 1024 ..... )
}

void Widget::runF1()
{
    QFile file("config");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        std::cout << line.toStdString() << std::endl;
        break;
    }
    file.close();
}

void Widget::runCommand()
{
    std::cout << lineEdit.text().toStdString() << std::endl;
}
