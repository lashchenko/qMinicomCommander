#ifndef REGEXPPARSER_H
#define REGEXPPARSER_H

#include <QtCore>

class RegexpParser
{
public:
    RegexpParser(QString fileName);
    QString processString(QString string);

private:
//    QStringList text;
    QStringList regexp;
};

#endif // REGEXPPARSER_H
