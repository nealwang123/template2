#ifndef SOCKETFORMMAIN_H
#define SOCKETFORMMAIN_H

#include <QWidget>
#include"frmtcpclient.h"
#include"mypainterwidget.h"
namespace Ui {
class SocketFormMain;
}

class SocketFormMain : public QWidget
{
    Q_OBJECT

public:
    explicit SocketFormMain(QWidget *parent = 0);
    ~SocketFormMain();
    void initForm();

private slots:
    void on_pushButton_Config_released();

private:
    Ui::SocketFormMain *ui;
    frmTcpClient *frm1;
    MyPainterWidget *frm2;
};

#endif // SOCKETFORMMAIN_H
