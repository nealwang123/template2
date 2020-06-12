#ifndef BUTTONPOWER_H
#define BUTTONPOWER_H

#include <QWidget>

class ButtonPower: public QWidget
{
    Q_OBJECT
public:
    explicit ButtonPower(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString imgFile;

public slots:
    void setValue(quint8 value);
};

#endif //BUTTONPOWER_H
