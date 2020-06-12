#ifndef FRMDBTCPCLIENT_H
#define FRMDBTCPCLIENT_H

#include <QWidget>

namespace Ui
{
    class frmDbTcpClient;
}

class frmDbTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit frmDbTcpClient(QWidget *parent = 0);
    ~frmDbTcpClient();

private:
    Ui::frmDbTcpClient *ui;

    int maxCount;
    int currentCount;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void test();

private slots:
    void debug(const QString &msg);

private slots:
    void on_btnStart_clicked();
    void on_txtDeviceCountClient_textChanged(const QString &arg1);
};

#endif // FRMDBTCPCLIENT_H
