#include "colorparser.h"

ColorParser::ColorParser(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QStringList list = in.readLine().split(":");
        if( list.size() != 3 ) {
            continue;
        }
        config.append(list);
    }
}

QString ColorParser::processString(QString string)
{
    foreach( QStringList list, config ) {
        if( string.indexOf(list.first()) != -1 ) {
            QString line;
            line += "<div style=\"background-color:'" + list.at(1) + "';"
                    + "color:'" + list.at(2) + "';" + "\">";
            line += string;
            line += "</div>";
            return line;
        }
    }

    QString line;
    line += "<div style=\"background-color:'#fff'; color:'#000';\">";
    line += string;
    line += "</div>";
    return line;
}
