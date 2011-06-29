#include "paletteparser.h"

QPalette PaletteParser::getPalette(QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QPalette();
    }

    QTextStream in(&file);

    QPalette palette;

    while (!in.atEnd()) {
        QStringList list = in.readLine().split(":");
        if( list.size() != 2 ) {
            continue;
        }
        QString telement = list.first().trimmed();
        QString color = list.last().trimmed();
        if( telement == "Base") {
            palette.setColor(QPalette::Base, QColor(color));
        } else if( telement == "Text") {
            palette.setColor(QPalette::Text, QColor(color));
        } else if( telement == "Button") {
            palette.setColor(QPalette::Button, QColor(color));
        } else if( telement == "ButtonText") {
            palette.setColor(QPalette::ButtonText, QColor(color));
        } else if( telement == "Highlight") {
            palette.setColor(QPalette::Highlight, QColor(color));
        } else if( telement == "HighlightedText") {
            palette.setColor(QPalette::HighlightedText, QColor(color));
        } else if( telement == "ToolTipBase") {
            palette.setColor(QPalette::ToolTipBase, QColor(color));
        } else if( telement == "ToolTipText") {
            palette.setColor(QPalette::ToolTipText, QColor(color));
        } else if( telement == "Window") {
            palette.setColor(QPalette::Window, QColor(color));
        } else if( telement == "WindowText") {
            palette.setColor(QPalette::WindowText, QColor(color));
        } else if( telement == "Shadow") {
            palette.setColor(QPalette::Shadow, QColor(color));
        }
    }
    file.close();

    return palette;
}
