#ifndef DEMARCATIONDELEGATE_H
#define DEMARCATIONDELEGATE_H

#include <QWidget>
#include <QtSql>

namespace Ui {
class DemarcationDelegate;
}

class DemarcationDelegate : public QWidget
{
    Q_OBJECT

public:
    explicit DemarcationDelegate(QWidget *parent = 0);
    ~DemarcationDelegate();
    bool createConnection();
    void startDemarcation(QStringList list);
private:
    Ui::DemarcationDelegate *ui;

    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型

    QSqlDatabase _db;
    QStringList m_deviceInfoList;
    QTimer* demarcationTimer;
    int demarcationIndex;
private slots:
    void initForm(QString fileName);
    void slot_demarcationTimer();


private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnDelete_clicked();
    void on_btnReturn_clicked();
    void on_btnClear_clicked();
};

#endif // DEMARCATIONDELEGATE_H
