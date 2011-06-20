#include "custombutton.h"

CustomButton::CustomButton(QString text, QWidget *parent) :
    QPushButton(parent)
{
    QPushButton::setText(text);

    connect(this, SIGNAL(clicked()), SLOT(slotClicked()));
}

void CustomButton::slotClicked()
{
    emit(clicked(QPushButton::text()));
}
