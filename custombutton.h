#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QPushButton>

class CustomButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomButton(QWidget *parent = 0);

    void setkey(QString text);
    QString getKey() const;

signals:
    void clicked(QString text);

private slots:
    void slotClicked();

private:
    QString key;
};

#endif // CUSTOMBUTTON_H
