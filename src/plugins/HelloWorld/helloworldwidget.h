#ifndef HELLOWORLDWIDGET_H
#define HELLOWORLDWIDGET_H

#include <QWidget>

namespace Ui {
class HelloWorldWidget;
}

class HelloWorldWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HelloWorldWidget(QWidget *parent = nullptr);
    ~HelloWorldWidget();
public slots:
    void slot_recMsgfromManager(int value);
private:
    Ui::HelloWorldWidget *ui;
};

#endif // HELLOWORLDWIDGET_H
