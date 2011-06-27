#include "regexpparser.h"

#include <iostream>

RegexpParser::RegexpParser(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        regexp.append(in.readLine());
//        std::cerr << regexp.last().toStdString() << std::endl;
    }
    file.close();
}

QString RegexpParser::processString(QString string)
{
    QString line = string;
    foreach(const QString pattern, regexp ) {
        line = line.replace(QRegExp(QString(pattern)), "");
//        std::cerr << pattern.toStdString() << "_";
    }
    return line;
}
