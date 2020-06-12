#ifndef FRMDBTCPSERVER_H
#define FRMDBTCPSERVER_H

#include <QWidget>

class frmDevicePanel;
class DbTcpServerThread;

namespace Ui
{
class frmDbTcpServer;
}

class frmDbTcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit frmDbTcpServer(QWidget *parent = 0);
    ~frmDbTcpServer();

private:
    Ui::frmDbTcpServer *ui;

    int maxCount;
    int currentCount;

    QStringList deviceNames;
    QStringList groupNames;
    QList<frmDevicePanel *> devices;
    QList<DbTcpServerThread *> servers;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void loadServer();
    void loadData();

public slots:
    void changeStyle();

private slots:
    void debug(const QString &msg);
    void dataReceive(const QStringList &keyData, const QStringList &resultData);

private slots:
    void on_btnStart_clicked();
    void on_btnLoad_clicked();
    void on_txtDeviceCountServer_textChanged(const QString &arg1);
};

#endif // FRMDBTCPSERVER_H
