#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void switchIcon(bool );
    void stateLedIcon(int ); // 0 为启动成功  1 为失败   2 为关闭  其他 为未知错误

private:
    Ui::Widget *ui;
    bool _Switch;

private slots:
    void smSwitchBtnClickedSlots();
    void smSettingBtnClickedSlots();
    void sowBtnClickedSlots();
signals:
    void sendMsg2Manager(int );
};

#endif // WIDGET_H
