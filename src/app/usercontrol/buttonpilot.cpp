#include "buttonpilot.h"
#include "qpainter.h"

ButtonPilot::ButtonPilot(QWidget *parent) : QWidget(parent)
{
    setBlue();
}

void ButtonPilot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QImage img(imgFile);
    painter.drawImage(rect(), img);
}

void ButtonPilot::setImage(QString imageName)
{
    imgFile = QString(":/imageTest/buttonpilot_%1.png").arg(imageName);
    update();
}

void ButtonPilot::setBlue()
{
    setImage("blue");
}

void ButtonPilot::setGray()
{
    setImage("gray");
}

void ButtonPilot::setGreen()
{
    setImage("green");
}

void ButtonPilot::setRed()
{
    setImage("red");
}

void ButtonPilot::setYellow()
{
    setImage("yellow");
}
