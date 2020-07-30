#ifndef OLINEDEVICEFORM_H
#define OLINEDEVICEFORM_H

#include <QWidget>
#include <QButtonGroup>
#include <QObjectUserData>
#include <QSortFilterProxyModel>
#include <QDebug>
#include "itemdelegate.h"
#include "itemdef.h"
#include <QDateTime>
#include<QTimer>
namespace Ui {
class OlineDeviceForm;
}

class OlineDeviceForm : public QWidget
{
    Q_OBJECT

public:
    explicit OlineDeviceForm(QWidget *parent = nullptr);
    ~OlineDeviceForm();
    void addData(int index,QString Date,int state=1);
    void resetData();

    void updateButtonNum();
    void updateTime();
    void setData(int id,int frameid,int state);

private:

private slots:
    void onButtonClicked(QAbstractButton *button);
    void on_redBtn_clicked();

    void on_allBtn_clicked();

    void on_blueBtn_clicked();

    void on_yellowBtn_clicked();

    void on_setRedBtn_clicked();

private:
    Ui::OlineDeviceForm *ui;
    QButtonGroup *m_filterButtonGroup;
    ItemDelegate *m_delegate;                 //委托
    QSortFilterProxyModel* m_proxyModel;
    QStandardItemModel *m_model;
    int totalNum;
    int redNum;
    int blueNum;
    int yellowNum;

};

#endif // OLINEDEVICEFORM_H
