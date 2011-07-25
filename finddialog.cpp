#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(&edit);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(&prev);
    hl->addWidget(&next);

    vl->addLayout(hl);
    setLayout(vl);

    prev.setText("prev");
    connect(&prev, SIGNAL(clicked()), this, SLOT(slotFindPrev()));

    next.setText("next");
    connect(&next, SIGNAL(clicked()), this, SLOT(slotFindNext()));
}

void FindDialog::slotFindPrev()
{
    qDebug() << "emit FIND_PREV";
    emit signalFindPrev(edit.text());
}

void FindDialog::slotFindNext()
{
    qDebug() << "emit FIND_NEXT";
    emit signalFindNext(edit.text());
}
