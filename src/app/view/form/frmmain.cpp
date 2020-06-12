#include "frmmain.h"
#include "ui_frmmain.h"
#include "quiwidget.h"

#include "frmdbtcpclient.h"
#include "frmdbtcpserver.h"
#include "frmdbtcpdata.h"
#include "frmdbclean.h"
#include "frmdbpage.h"
#include "frmdbsql.h"
#include "frmdbdelegate.h"
#include "frmdatacreate.h"

frmMain::frmMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    this->initForm();
    this->changeBtnStyle();

    qDebug() << "Available drivers:";
    QStringList drivers = QSqlDatabase::drivers();
    foreach(QString driver, drivers)
        qDebug() << "\t "<< driver;
    qDebug() << "End";


}


frmMain::~frmMain()
{
    delete ui;
}

void frmMain::initForm()
{
    ui->widgetLeft->setProperty("flag", "left");
    ui->widgetMain->setStyleSheet(QString("QWidget[flag=\"left\"] QAbstractButton{min-height:%1px;max-height:%1px;}").arg(20));

    //左侧导航样式
    pixChar << 0xf062 << 0xf063 << 0xf045 << 0xf197 << 0xf0ce << 0xf1e6 << 0xf20b << 0xf108;
    btns << ui->btnDbTcpClient << ui->btnDbTcpServer << ui->btnDbTcpData << ui->btnDbAutoClean << ui->btnDbPage << ui->btnDbSql << ui->btnDbDelegate << ui->btnDataCreate;

    int count = btns.count();
    for (int i = 0; i < count; i++) {
        btns.at(i)->setCheckable(true);
        btns.at(i)->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btns.at(i), SIGNAL(clicked(bool)), this, SLOT(buttonClick()));
    }

    frmDbTcpClient *w1 = new frmDbTcpClient;
    frmDbTcpServer *w2 = new frmDbTcpServer;
    frmDbTcpData *w3 = new frmDbTcpData;
    frmDbClean *w4 = new frmDbClean;
    frmDbPage *w5 = new frmDbPage;
    frmDbSql *w6 = new frmDbSql;
    frmDbDelegate *w7 = new frmDbDelegate;
    frmDataCreate *w8 = new frmDataCreate;

    connect(this, SIGNAL(changeStyle()), w2, SLOT(changeStyle()));

    ui->stackedWidget->addWidget(w1);
    ui->stackedWidget->addWidget(w2);
    ui->stackedWidget->addWidget(w3);
    ui->stackedWidget->addWidget(w4);
    ui->stackedWidget->addWidget(w5);
    ui->stackedWidget->addWidget(w6);
    ui->stackedWidget->addWidget(w7);
    ui->stackedWidget->addWidget(w8);

    ui->btnDbPage->click();
}

void frmMain::buttonClick()
{
    QToolButton *b = (QToolButton *)sender();

    int count = btns.count();
    for (int i = 0; i < count; i++) {
        if (btns.at(i) == b) {
            btns.at(i)->setChecked(true);
            btns.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btns.at(i), false)));
        } else {
            btns.at(i)->setChecked(false);
            btns.at(i)->setIcon(QIcon(IconHelper::Instance()->getPixmap(btns.at(i), true)));
        }
    }

    if (b == ui->btnDbTcpClient) {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (b == ui->btnDbTcpServer) {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (b == ui->btnDbTcpData) {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (b == ui->btnDbAutoClean) {
        ui->stackedWidget->setCurrentIndex(3);
    } else if (b == ui->btnDbPage) {
        ui->stackedWidget->setCurrentIndex(4);
    } else if (b == ui->btnDbSql) {
        ui->stackedWidget->setCurrentIndex(5);
    } else if (b == ui->btnDbDelegate) {
        ui->stackedWidget->setCurrentIndex(6);
    } else if (b == ui->btnDataCreate) {
        ui->stackedWidget->setCurrentIndex(7);
    }
}

void frmMain::changeBtnStyle()
{
    IconHelper::Instance()->removeStyle(btns);

    QString borderColor = QUIConfig::HighColor;
    QString normalBgColor = QUIConfig::NormalColorStart;
    QString darkBgColor = QUIConfig::DarkColorStart;
    QString normalTextColor = QUIConfig::TextColor;
    QString darkTextColor = QUIConfig::TextColor;
    IconHelper::Instance()->setStyle(ui->widgetLeft, btns, pixChar, 15, 25, 20, "left", 5,
                                     borderColor, normalBgColor, darkBgColor, normalTextColor, darkTextColor);
}

void frmMain::changeStyle(const QString &qssFile)
{
    QFile file(qssFile);

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QUIConfig::PanelColor = qss.mid(20, 7);
        QUIConfig::TextColor = qss.mid(49, 7);
        file.close();
    }

    App::StyleName = qssFile;
    App::writeConfig();
    emit changeStyle();

    this->changeBtnStyle();
}
