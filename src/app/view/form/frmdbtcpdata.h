#ifndef FRMDBTCPDATA_H
#define FRMDBTCPDATA_H

#include <QWidget>

namespace Ui
{
    class frmDbTcpData;
}

class frmDbTcpData : public QWidget
{
    Q_OBJECT

public:
    explicit frmDbTcpData(QWidget *parent = 0);
    ~frmDbTcpData();

private:
    Ui::frmDbTcpData *ui;

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
};

#endif // FRMDBTCPDATA_H
