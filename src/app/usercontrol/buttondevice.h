#ifndef BUTTONDEVICE_H
#define BUTTONDEVICE_H

#include <QWidget>

class ButtonDevice: public QWidget
{
    Q_OBJECT
public:
    explicit ButtonDevice(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString imgFile;

public slots:
    void setImage(QString imageName);
    void setGray();
    void setRed();
    void setGreen();
};

#endif //BUTTONDEVICE_H
