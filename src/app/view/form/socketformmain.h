#ifndef SOCKETFORMMAIN_H
#define SOCKETFORMMAIN_H

#include <QWidget>

namespace Ui {
class SocketFormMain;
}

class SocketFormMain : public QWidget
{
    Q_OBJECT

public:
    explicit SocketFormMain(QWidget *parent = 0);
    ~SocketFormMain();
    void initForm();
private:
    Ui::SocketFormMain *ui;
};

#endif // SOCKETFORMMAIN_H
