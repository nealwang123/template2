#include "socketformmain.h"
#include "ui_socketformmain.h"
#include"frmtcpclient.h"
#include"mypainterwidget.h"
SocketFormMain::SocketFormMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SocketFormMain)
{
    ui->setupUi(this);

    initForm();
}

SocketFormMain::~SocketFormMain()
{
    delete ui;
}
void SocketFormMain::initForm(){
    frmTcpClient *frm1=new frmTcpClient();
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    MyPainterWidget *frm2=new MyPainterWidget();
    hlayout1->addWidget(frm2);
    ui->tabWidget->clear();//清空选项卡
    //ui->tabWidget->removeTab(0);//移除选项卡
    ui->tabWidget->setTabPosition(QTabWidget::North);//设置选项卡的方位，默认在上方(东南西北)
    ui->tabWidget->addTab(frm1, QIcon("Resources\\a10.ico"), tr("配置"));//在后面添加带图标的选项卡
    //ui->tabWidget->setIconSize(QSize(50, 25));//设置图标的大小(选项卡的大小也会改变)
    ui->tabWidget->addTab(frm2, QIcon(QPixmap("Resources\\a11.png").scaled(150, 120)), "显示");//添加选项卡
    ui->tabWidget->setTabToolTip(1, tr("图像"));//鼠标悬停弹出提示
    ui->tabWidget->setTabShape(QTabWidget::Triangular);//设置选项卡的形状 Rounded
    ui->tabWidget->setMovable(true);
    ui->tabWidget->usesScrollButtons();//选项卡滚动
    connect(frm1,SIGNAL(sig_FrameContent(Frame_Content&)),frm2,SLOT(slot_FrameContent(Frame_Content&)));
//    QHBoxLayout *hlayout1 = new QHBoxLayout;
//    QRadioButton *continusModeRadioButton = new QRadioButton(tr("连续"));
//    QRadioButton *triggerModeRadioButton = new QRadioButton(tr("触发"));
//    hlayout1->addWidget(continusModeRadioButton);
//    hlayout1->addWidget(triggerModeRadioButton);

//    QHBoxLayout *hlayout2 = new QHBoxLayout;
//    QPushButton *startGrabbingPushButton = new QPushButton(tr("开始"));
//    QPushButton *stopGrabbingPushButton = new QPushButton(tr("停止"));
//    hlayout2->addWidget(startGrabbingPushButton);
//    hlayout2->addWidget(stopGrabbingPushButton);

//    QHBoxLayout *hlayout3 = new QHBoxLayout;
//    QCheckBox *softwareTriggerCheckBox = new QCheckBox(tr("选择"));
//    QPushButton *softwareOncePushButton = new QPushButton(tr("终止"));
//    hlayout3->addWidget(softwareTriggerCheckBox);
//    hlayout3->addWidget(softwareOncePushButton);

//    QHBoxLayout *hlayout4 = new QHBoxLayout;
//    QCheckBox *detectionCheckBox = new QCheckBox(tr("检测"));
//    hlayout4->addWidget(detectionCheckBox);

//    QGridLayout *vlayout = new QGridLayout;
//    vlayout->addLayout(hlayout1, 0, 0);
//    vlayout->addLayout(hlayout2, 1, 0);
//    vlayout->addLayout(hlayout3, 2, 0);
//    vlayout->addLayout(hlayout4, 3, 0);

//    QWidget *tabCollection = new QWidget;
//    tabCollection->setLayout(vlayout);
//    ui->tabWidget->insertTab(1, tabCollection, QIcon("Resources\\a12.ico"), tr("采集"));//插入选项卡

}
