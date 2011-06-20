#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>

class CustomButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomButton(QString text, QWidget *parent = 0);

signals:
    void clicked(QString text);

private slots:
    void slotClicked();
};

#endif // CUSTOMBUTTON_H
