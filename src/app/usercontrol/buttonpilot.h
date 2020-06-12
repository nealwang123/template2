#ifndef BUTTONPILOT_H
#define BUTTONPILOT_H

#include <QWidget>

class ButtonPilot: public QWidget
{
    Q_OBJECT
public:
    explicit ButtonPilot(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString imgFile;

public slots:
    void setImage(QString imageName);
    void setBlue();
    void setGray();
    void setGreen();
    void setRed();
    void setYellow();
};

#endif //BUTTONPILOT_H
