#ifndef INITIALPHASEFORM_H
#define INITIALPHASEFORM_H

#include <QWidget>
#include <quiwidget.h>
namespace Ui {
class InitialPhaseForm;
}

class InitialPhaseForm : public QWidget
{
    Q_OBJECT

public:
    explicit InitialPhaseForm(QWidget *parent = nullptr);
    ~InitialPhaseForm();
    void displayWorkmode(QString str);
    void initialHandle();
private slots:
    void on_cBoxcansend_activated(int index);
    void on_cBoxCanSendDiscrib_activated(int index);

private:
    Ui::InitialPhaseForm *ui;
    //can发送指令
    QStringList m_list_canSendCommand,m_list_canSendDiscrib;
    int m_commandIndex;
    QString m_selfSendStr;
};

#endif // INITIALPHASEFORM_H
