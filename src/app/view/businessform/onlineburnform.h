#ifndef ONLINEBURNFORM_H
#define ONLINEBURNFORM_H

#include <QWidget>
#include"quiwidget.h"
#include"uds.h"
namespace Ui {
class OnlineBurnForm;
}

class OnlineBurnForm : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineBurnForm(QWidget *parent = nullptr);
    ~OnlineBurnForm();
    ECANStatus writehexBlockData(QList<HexRecordBlock> HexBlockList, int type);
    void stringToHtmlFilter(QString &str);
    void stringToHtml(QString& str,QColor crl);
    void displayStr(QString str,int index=0);
    void updateDone();
private slots:
    void slot_burnTimer();
    void slot_autoScroll();
    void on_cBoxcansend_activated(int index);
    void on_cBoxCanSendDiscrib_activated(int index);

    void on_toolButton_released();

    void on_pushButton_released();

    void on_pushButton_update_released();

private:
    Ui::OnlineBurnForm *ui;
    //can发送指令
    QStringList m_list_canSendCommand,m_list_canSendDiscrib;
    int m_commandIndex;
    QString m_selfSendStr;

    //整块block数据hex文件
    static QList<HexRecordBlock> hexRecordBlocks_DriverList ;
    FileParse fileParse;
    bool m_ParseFile;
    //
    QTimer * burnTimer;
};

#endif // ONLINEBURNFORM_H
