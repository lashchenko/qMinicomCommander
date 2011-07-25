#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QtGui>

class FindDialog : public QWidget
{
    Q_OBJECT
public:
    FindDialog(QWidget *parent = 0);

signals:
    void signalFindPrev(QString text);
    void signalFindNext(QString text);

private slots:
    void slotFindPrev();
    void slotFindNext();

private:
    QLineEdit edit;
    QPushButton prev;
    QPushButton next;
};

#endif // FINDDIALOG_H
