#include "buttonpower.h"
#include "qpainter.h"

ButtonPower::ButtonPower(QWidget *parent) : QWidget(parent)
{
    imgFile = ":/imageTest/buttonpower_4.png";
}

void ButtonPower::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QImage img(imgFile);
    painter.drawImage(rect(), img);
}

void ButtonPower::setValue(quint8 value)
{
    imgFile = QString(":/imageTest/buttonpower_%1.png").arg(value);
    update();
}
