#ifndef CANDEBUGFORM_H
#define CANDEBUGFORM_H

#include <QWidget>
#include "canapi.h"
namespace Ui {
class CanDebugForm;
}

class CanDebugForm : public QWidget
{
    Q_OBJECT

public:
    explicit CanDebugForm(QWidget *parent = nullptr);
    ~CanDebugForm();
public slots:
    void slot_sendCanData(VCI_CAN_OBJ& obj);
    void slot_recvCanData(VCI_CAN_OBJ& obj);
private slots:
    void on_pushButton_Clear_released();

private:
    Ui::CanDebugForm *ui;
};

#endif // CANDEBUGFORM_H
