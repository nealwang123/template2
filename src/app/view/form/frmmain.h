#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QDialog>

class QToolButton;

namespace Ui
{
class frmMain;
}

class frmMain : public QDialog
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private:
    Ui::frmMain *ui;
    QList<int> pixChar;
    QList<QToolButton *> btns;

private slots:    
    void initForm();
    void buttonClick();
    void changeBtnStyle();

public slots:
    void changeStyle(const QString &qssFile);

signals:
    void changeStyle();
};

#endif // FRMMAIN_H
