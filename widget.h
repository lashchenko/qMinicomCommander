#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void updateText();
    void runCommand();
    void runF1();

private:
    QTimer timer;
    QTextBrowser browser;
    QLineEdit lineEdit;
    QPushButton button;

    quint64 fileSize;
};

#endif // WIDGET_H
