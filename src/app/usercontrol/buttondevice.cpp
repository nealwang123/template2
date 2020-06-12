#include "buttondevice.h"
#include "qpainter.h"

ButtonDevice::ButtonDevice(QWidget *parent) : QWidget(parent)
{
    setGray();
}

void ButtonDevice::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QImage img(imgFile);
    painter.drawImage(rect(), img);
}

void ButtonDevice::setImage(QString imageName)
{
    imgFile = QString(":/imageTest/buttondevice_%1.png").arg(imageName);
    update();
}

void ButtonDevice::setGray()
{
    setImage("gray");
}

void ButtonDevice::setRed()
{
    setImage("red");
}

void ButtonDevice::setGreen()
{
    setImage("green");
}
