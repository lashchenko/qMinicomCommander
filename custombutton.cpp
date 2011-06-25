#include "custombutton.h"

CustomButton::CustomButton(QWidget *parent) :
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), SLOT(slotClicked()));
}

void CustomButton::setkey(QString text)
{
    key = text;
}

QString CustomButton::getKey() const
{
    return key;
}

void CustomButton::slotClicked()
{
    emit(clicked(key));
}
