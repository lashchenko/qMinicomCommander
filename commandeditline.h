#ifndef COMMANDEDITLINE_H
#define COMMANDEDITLINE_H

#include <QtGui>

class CommandEditLine : public QLineEdit
{
    Q_OBJECT
public:
    explicit CommandEditLine(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);

signals:
    void enterPressed();

public slots:
    void setText(const QString &command);

protected:

    bool setCommand(const QString &command);

//protected slots:
//    void keyPressed(QKeyEvent *event);

private:
    QStringList commands;
    int current;
};

#endif // COMMANDEDITLINE_H
