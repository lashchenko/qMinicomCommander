#ifndef COLORPARSER_H
#define COLORPARSER_H

#include <QtCore>
#include <QtGui/QColor>

class ColorParser
{
public:
    ColorParser(QString fileName);
//    QStringList parseFile(QString &fileName);
    QString processString(QString string);

    void setBgColor(QColor color);
    void setTextColor(QColor color);

private:
//    QStringList text;
    QList<QStringList> config;
    QString bgColor;
    QString textColor;
};

#endif // COLORPARSER_H
