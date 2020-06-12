#ifndef DBDELEGATE_H
#define DBDELEGATE_H

/**
 * 自定义委托类
 * 1:可设置多种委托类型,例如文本框/下拉框/日期框等
 * 2:可设置文本框是否密文显示及对应密码显示的列
 * 3:可设置默认数据,包括下拉框数据集合
 * 4:提供值变化信号,比方说下拉框值改动触发
 * 5:支持某一列数据校验显示不同图标
 * 6:支持设置校验列/校验规则/校验值/校验成功图标/校验失败图标
 */

#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

class DbDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit DbDelegate(QObject *parent = 0);

protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index)const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &)const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QString delegateType;       //委托类型
    bool edit;                  //可否编辑
    bool pwd;                   //可否密码显示
    QStringList delegateValue;  //委托参数
    int pwdColumn;              //密码显示列,默认-1标识无

    QPushButton checkBtn;       //校验数据显示不同图标
    int checkColumn;            //校验数据列
    int checkTarget;            //校验数据参照列
    QString checkType;          //校验规则,例如>=
    QString checkValue;         //检验值
    QString checkOkImage;       //校验成功图标
    QString checkNoImage;       //校验失败图标
    int checkImageWidth;        //图片宽度
    int checkImageHeight;       //图片高度

signals:
    //值改变信号
    void valueChange(const QString &text);

public slots:
    //设置委托类型
    void setDelegateType(const QString &delegateType);

    //设置下拉框是否允许编辑
    void setDelegateEdit(bool edit);

    //设置文本框是否密码显示
    void setDelegatePwd(bool pwd);

    //设置委托参数
    void setDelegateValue(const QStringList &delegateValue);

    //设置要隐藏密码的列
    void setPwdColumn(int pwdColumn);

    //设置校验数据列
    void setCheckColumn(int checkColumn);

    //设置校验数据参照列
    void setCheckTarget(int checkTarget);

    //设置校验规则 == > >= < <= != contains
    void setCheckType(const QString &checkType);

    //设置校验值
    void setCheckValue(const QString &checkValue);

    //设置校验成功显示图标
    void setCheckOkImage(const QString &checkOkImage);

    //设置校验失败显示图标
    void setCheckNoImage(const QString &checkNoImage);

    //设置校验图片宽度
    void setCheckImageWidth(int checkImageWidth);

    //设置校验图片高度
    void setCheckImageHeight(int checkImageHeight);

};

#endif // DBDELEGATE_H
