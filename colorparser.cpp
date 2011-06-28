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
        if( list.size() < 3 ) {
            continue;
        }
        config.append(list);
    }
    file.close();
}

QString ColorParser::processString(QString string)
{
    foreach( QStringList list, config ) {
        if( string.indexOf(list.first()) != -1 ) {
            QString line;
            line += "<div style=\"background-color:'" + list.at(1) + "';"
                    + "color:'" + list.at(2) + "';" + "\">";
            if( list.size() > 3 ) {
                for( int i=3; i<list.size(); ++i ) {
                    line += "<" + list.at(i) + ">";
                }
            }
            line += string;
            if( list.size() > 3 ) {
                for( int i=list.size()-1; i>=3; --i ) {
                    line += "</" + list.at(i) + ">";
                }
            }
            line += "</div>";
            return line;
        }
    }

    QString line;
    line += "<div style=\"background-color:'#222222'; color:'#64665F';\">";
    line += string;
    line += "</div>";
    return line;
}
