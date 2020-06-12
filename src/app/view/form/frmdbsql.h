#ifndef FRMDBSQL_H
#define FRMDBSQL_H

#include <QWidget>

namespace Ui {
class frmDbSql;
}

class frmDbSql : public QWidget
{
    Q_OBJECT

public:
    explicit frmDbSql(QWidget *parent = 0);
    ~frmDbSql();

private:
    Ui::frmDbSql *ui;

private slots:
    void initConfig();
    void saveConfig();

private slots:
    void on_btnSqliteConnect_clicked();
    void on_btnSqliteSql_clicked();
    void on_btnMysqlConnect_clicked();
    void on_btnMysqlSql_clicked();
};

#endif // FRMDBSQL_H
