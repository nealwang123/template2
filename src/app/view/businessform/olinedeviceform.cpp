#include "olinedeviceform.h"
#include "ui_olinedeviceform.h"

OlineDeviceForm::OlineDeviceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OlineDeviceForm)
{
    ui->setupUi(this);
    totalNum = 0;
    redNum = 0;
    blueNum = 0;
    yellowNum = 0;
    m_model = new QStandardItemModel();



    m_delegate = new ItemDelegate(this);
    m_filterButtonGroup = new QButtonGroup(this);

    // 设置互斥
    m_filterButtonGroup->setExclusive(true);

    m_filterButtonGroup->addButton(ui->allBtn);
    m_filterButtonGroup->addButton(ui->redBtn);
    m_filterButtonGroup->addButton(ui->blueBtn);
    m_filterButtonGroup->addButton(ui->yellowBtn);

    // 连接信号槽
    connect(m_filterButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));

    ui->listView->setItemDelegate(m_delegate);       //为视图设置委托
    ui->listView->setSpacing(15);                   //为视图设置控件间距
    m_proxyModel = new QSortFilterProxyModel(ui->listView);
    m_proxyModel->setSourceModel(m_model);
    m_proxyModel->setFilterRole(Qt::UserRole);
    m_proxyModel->setDynamicSortFilter(true);
    ui->listView->setModel(m_proxyModel);                  //为委托设置模型
    ui->listView->setViewMode(QListView::IconMode); //设置Item图标显示
    ui->listView->setDragEnabled(false);            //控件不允许拖动


}

OlineDeviceForm::~OlineDeviceForm()
{
    delete ui;
}
void OlineDeviceForm::resetData(){
    m_model->clear();
    totalNum = 0;
    redNum = 0;
    blueNum = 0;
    yellowNum = 0;
    updateButtonNum();
}


void OlineDeviceForm::addData(int index,QString Date,int state)
{

    totalNum++;
    QStandardItem *Item = new QStandardItem;
    ItemData itemData;
    itemData.id = index;
    itemData.date = QString("%1").arg(Date);
    itemData.state = state;
    ItemStatus itemStatus;
    switch (itemData.state) {
    case 0:
        itemStatus = S_RED;
        redNum++;
        break;
    case 1:
        itemStatus = S_BLUE;
        blueNum++;
        break;
    case 2:
        itemStatus = S_YELLOW;
        yellowNum++;
        break;

    }
    Item->setData(itemStatus,Qt::UserRole);  // 单一存取
    Item->setData(QVariant::fromValue(itemData),Qt::UserRole+1);//整体存取
    m_model->appendRow(Item);      //追加Item
    //ui->listView->setSelectionMode();
    ui->listView->setSelectionRectVisible(false);
    updateButtonNum();

}
void OlineDeviceForm::updateTime(){
    ui->label_Time->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}
void OlineDeviceForm::onButtonClicked(QAbstractButton *button)
{// 当前点击的按钮

    qDebug() << button->text();

}

void OlineDeviceForm::updateButtonNum()
{
    ui->allBtn->setText(tr("All %1").arg(totalNum));
    ui->redBtn->setText(tr("离线 %1").arg(redNum));
    ui->blueBtn->setText(tr("在线 %1").arg(blueNum));
    ui->yellowBtn->setText(tr("下线或异常 %1").arg(yellowNum));
}

void OlineDeviceForm::on_redBtn_clicked()
{
    if(m_proxyModel)
    {
        m_proxyModel->setFilterFixedString(QString::number(S_RED));
    }
}

void OlineDeviceForm::on_allBtn_clicked()
{
    if(m_proxyModel)
    {
        m_proxyModel->setFilterFixedString(QString());
    }
}

void OlineDeviceForm::on_blueBtn_clicked()
{
    if(m_proxyModel)
    {
        m_proxyModel->setFilterFixedString(QString::number(S_BLUE));
    }
}

void OlineDeviceForm::on_yellowBtn_clicked()
{
    if(m_proxyModel)
    {
        m_proxyModel->setFilterFixedString(QString::number(S_YELLOW));
    }
}
void OlineDeviceForm::setData(int id,int frameid,int state){
    int rows = m_model->rowCount();
    int column = m_model->columnCount();
    for (int i=0;i<rows;++i)
    {
        for(int j=0;j<column;++j)
        {
            QStandardItem* item = m_model->item(i,j);
            if (item)
            {

            }
        }
    }


}
void OlineDeviceForm::on_setRedBtn_clicked()
{
    QModelIndexList modelIndexList = ui->listView->selectionModel()->selectedIndexes();
    QModelIndexList sourceIndexList;
    foreach (QModelIndex modelIndex, modelIndexList){
        sourceIndexList<<m_proxyModel->mapToSource(modelIndex); //获取源model的modelIndex
    }

//    g_proxyModel->setDynamicSortFilter(false);
    foreach (QModelIndex sourceIndex, sourceIndexList){
        ItemStatus status = (ItemStatus)(sourceIndex.data(Qt::UserRole).toInt());
        qDebug() << "Index : " << sourceIndex.row();

        switch (status) {
            case S_RED:
                redNum--;
                break;
            case S_BLUE:
                blueNum--;
                break;
            case S_YELLOW:
                yellowNum--;
                break;
        }

        status = S_RED;
        redNum++;

        m_model->setData(sourceIndex,status,Qt::UserRole);
    }
//    g_proxyModel->setDynamicSortFilter(true);
    updateButtonNum();
}
