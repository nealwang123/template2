#ifndef PSBROKERWIDGET_H
#define PSBROKERWIDGET_H

#include <QWidget>
#include"psbrokermonitor.h"
namespace Ui {
class PSBrokerWidget;
}

class PSBrokerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PSBrokerWidget(QWidget *parent = 0);
    ~PSBrokerWidget();

private slots:
    void on_pushButton_start_clicked();

private:
    Ui::PSBrokerWidget *ui;
    PSBrokerMonitor * brokerthread;
};

#endif // PSBROKERWIDGET_H
