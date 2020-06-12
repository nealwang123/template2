#include "dbdelegate.h"

DbDelegate::DbDelegate(QObject *parent) : QItemDelegate(parent)
{
    delegateType = "QLineEdit";
    edit = false;
    pwd = false;
    delegateValue.append("");
    pwdColumn = -1;

    //设置显示不同图标的按钮无边框,背景透明
    checkBtn.setStyleSheet("QPushButton{border:none;background:none;}");
    checkColumn = -1;
    checkTarget = -1;
    checkType = "contains";
    checkValue = "报警";
    checkOkImage = ":/imageTest/msg_error.png";
    checkNoImage = ":/imageTest/msg_info.png";
    checkImageWidth = 15;
    checkImageHeight = 15;
}

QWidget *DbDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QWidget *editor = new QWidget(parent);

    if (delegateType == "QLineEdit") {
        QLineEdit *txt = new QLineEdit(parent);
        txt->setText(delegateValue.at(0));
        editor = txt;
        //txt->setStyleSheet("QLineEdit{border-radius:0px;}");
        connect(txt, SIGNAL(textChanged(QString)), this, SIGNAL(valueChange(QString)));

        if (pwd) {
            txt->setEchoMode(QLineEdit::Password);
        }
    } else if (delegateType == "QComboBox") {
        QComboBox *cbox = new QComboBox(parent);
        cbox->setEditable(edit);
        cbox->addItems(delegateValue);
        editor = cbox;
        //cbox->setStyleSheet("QComboBox{border-radius:0px;}");
        connect(cbox, SIGNAL(currentIndexChanged(QString)), this, SIGNAL(valueChange(QString)));
    } else if (delegateType == "QDateTimeEdit") {
        QDateTimeEdit *dateTime = new QDateTimeEdit(parent);        
        dateTime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
        dateTime->setCalendarPopup(true);
        dateTime->setDateTime(QDateTime::fromString(delegateValue.at(0), "yyyy-MM-dd HH:mm:ss"));
        dateTime->calendarWidget()->setLocale(QLocale::Chinese);
        editor = dateTime;
    } else if (delegateType == "QDateEdit") {
        QDateEdit *date = new QDateEdit(parent);        
        date->setDisplayFormat("yyyy-MM-dd");
        date->setCalendarPopup(true);
        date->setDate(QDate::fromString(delegateValue.at(0), "yyyy-MM-dd"));
        date->calendarWidget()->setLocale(QLocale::Chinese);
        editor = date;
    } else if (delegateType == "QTimeEdit") {
        QTimeEdit *time = new QTimeEdit(parent);        
        time->setDisplayFormat("HH:mm:ss");
        time->setTime(QTime::fromString(delegateValue.at(0), "HH:mm:ss"));
        editor = time;
    }

    editor->installEventFilter(const_cast<DbDelegate *>(this));
    return editor;
}

void DbDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index).toString();

    if (delegateType == "QLineEdit") {
        QLineEdit *txt = static_cast<QLineEdit *>(editor);
        txt->setText(str);
    } else if (delegateType == "QComboBox") {
        QComboBox *cbox = static_cast<QComboBox *>(editor);
        cbox->setCurrentIndex(cbox->findText(str));
    } else if (delegateType == "QDateTimeEdit") {
        QDateTimeEdit *dateTime = static_cast<QDateTimeEdit *>(editor);
        dateTime->setDateTime(QDateTime::fromString(str, "yyyy-MM-dd HH:mm:ss"));
    } else if (delegateType == "QDateEdit") {
        QDateEdit *date = static_cast<QDateEdit *>(editor);
        date->setDate(QDate::fromString(str, "yyyy-MM-dd"));
    } else if (delegateType == "QTimeEdit") {
        QTimeEdit *time = static_cast<QTimeEdit *>(editor);
        time->setTime(QTime::fromString(str, "HH:mm:ss"));
    }
}

void DbDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const
{
    if (delegateType == "QLineEdit") {
        QLineEdit *txt = static_cast<QLineEdit *>(editor);
        QString str = txt->text();
        model->setData(index, str);
    } else if (delegateType == "QComboBox") {
        QComboBox *cbox = static_cast<QComboBox *>(editor);
        QString str = cbox->currentText();
        model->setData(index, str);
    } else if (delegateType == "QDateTimeEdit") {
        QDateTimeEdit *dateTime = static_cast<QDateTimeEdit *>(editor);
        QString str = dateTime->dateTime().toString("yyyy-MM-dd HH:mm:ss");
        model->setData(index, str);
    } else if (delegateType == "QDateEdit") {
        QDateEdit *date = static_cast<QDateEdit *>(editor);
        QString str = date->date().toString("yyyy-MM-dd");
        model->setData(index, str);
    } else if (delegateType == "QTimeEdit") {
        QTimeEdit *time = static_cast<QTimeEdit *>(editor);
        QString str = time->time().toString("HH:mm:ss");
        model->setData(index, str);
    }
}

void DbDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void DbDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (pwdColumn >= 0 && index.column() == pwdColumn) {
        //获得索引对应Model中的数据,将对应数据格式替换
        QString str = index.model()->data(index, Qt::DisplayRole).toString();
        int len = str.length();
        QString text;

        for (int i = 0; i < len; i++) {
            text += "●";
        }

        drawDisplay(painter, option, option.rect, text);
        return;
    }

    if (checkColumn >= 0 && index.column() == checkColumn && checkTarget >= 0) {
        //获取参照列的字符串数据
        QString str = index.model()->index(index.row(), checkTarget).data().toString();

        //设置委托显示的按钮图标大小和区域
        QStyleOptionButton button;
        button.iconSize = QSize(checkImageWidth, checkImageHeight);
        button.rect = option.rect;
        button.state |= QStyle::State_Enabled;

        //根据过滤规则判断是否复合
        bool existCheck = false;
        if (checkType == "==") {
            if (str == checkValue) {
                existCheck = true;
            }
        } else if (checkType == ">") {
            if (str > checkValue) {
                existCheck = true;
            }
        } else if (checkType == ">=") {
            if (str >= checkValue) {
                existCheck = true;
            }
        } else if (checkType == "<") {
            if (str < checkValue) {
                existCheck = true;
            }
        } else if (checkType == "<=") {
            if (str <= checkValue) {
                existCheck = true;
            }
        } else if (checkType == "!=") {
            if (str != checkValue) {
                existCheck = true;
            }
        } else if (checkType == "contains") {
            QStringList list = checkValue.split("|");
            foreach (QString value, list) {
                if (str.contains(value)) {
                    existCheck = true;
                    break;
                }
            }
        }

        //复合关键字则错误图标,否则正确图标
        if (existCheck) {
            button.icon = QIcon(checkOkImage);
        } else {
            button.icon = QIcon(checkNoImage);
        }

        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, &checkBtn);
        return;
    }

    QItemDelegate::paint(painter, option, index);
}

void DbDelegate::setDelegateType(const QString &delegateType)
{
    this->delegateType = delegateType;
}

void DbDelegate::setDelegateEdit(bool edit)
{
    this->edit = edit;
}

void DbDelegate::setDelegatePwd(bool pwd)
{
    this->pwd = pwd;
}

void DbDelegate::setDelegateValue(const QStringList &delegateValue)
{
    this->delegateValue = delegateValue;
}

void DbDelegate::setPwdColumn(int pwdColumn)
{
    this->pwdColumn = pwdColumn;
}

void DbDelegate::setCheckColumn(int checkColumn)
{
    this->checkColumn = checkColumn;
}

void DbDelegate::setCheckTarget(int checkTarget)
{
    this->checkTarget = checkTarget;
}

void DbDelegate::setCheckType(const QString &checkType)
{
    this->checkType = checkType;
}

void DbDelegate::setCheckValue(const QString &checkValue)
{
    this->checkValue = checkValue;
}

void DbDelegate::setCheckOkImage(const QString &checkOkImage)
{
    this->checkOkImage = checkOkImage;
}

void DbDelegate::setCheckNoImage(const QString &checkNoImage)
{
    this->checkNoImage = checkNoImage;
}

void DbDelegate::setCheckImageWidth(int checkImageWidth)
{
    this->checkImageWidth = checkImageWidth;
}

void DbDelegate::setCheckImageHeight(int checkImageHeight)
{
    this->checkImageHeight = checkImageHeight;
}

