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

void ColorParser::setBgColor(QColor color)
{
    bgColor = color.name();
}

void ColorParser::setTextColor(QColor color)
{
    textColor = color.name();
}

QString ColorParser::processString(QString string)
{
    bool isStarDotStar = false;
    QStringList starList;
    foreach( QStringList list, config ) {
        if( list.first() == "*.*" ) {
            if( list.size() > 2 ) {
                isStarDotStar = true;
                starList = list;
            }
        }
        if( string.indexOf(list.first()) != -1 ) {
            QString line;
            line += "<div style=\"background-color:'" + (list.at(1).trimmed().isEmpty() ? "#222" : list.at(1).trimmed()) + "';"
                    + "color:'" + (list.at(2).trimmed().isEmpty() ? "#64665F" : list.at(2).trimmed()) + "';" + "\">";
            if( list.size() > 3 ) {
                for( int i=3; i<list.size(); ++i ) {
                    line += "<" + list.at(i).trimmed() + ">";
                }
            }
            line += string;
            if( list.size() > 3 ) {
                for( int i=list.size()-1; i>=3; --i ) {
                    line += "</" + list.at(i).trimmed() + ">";
                }
            }
            line += "</div>";
            return line;
        }        
    }



    QString line;
//    line += "<div style=\"background-color:'#222222'; color:'#64665F';\">";
    if( !isStarDotStar ) {
        line += "<div style=\"background-color:'" + bgColor + "'; color:'" + textColor + "';\">";
    } else {
        line += "<div style=\"background-color:'" + (starList.at(1).trimmed().isEmpty() ? bgColor : starList.at(1).trimmed()) + "';"
            + "color:'" + (starList.at(2).trimmed().isEmpty() ? textColor : starList.at(2).trimmed()) + "';" + "\">";
    }

    line += string;
    line += "</div>";
    return line;
}
