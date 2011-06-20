#ifndef COLORPARSER_H
#define COLORPARSER_H

#include <QtCore>

class ColorParser
{
public:
    ColorParser(QString fileName);
//    QStringList parseFile(QString &fileName);
    QString processString(QString string);

private:
    QStringList text;
    QList<QStringList> config;
};

#endif // COLORPARSER_H
