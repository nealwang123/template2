/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QPushButton *stateLed;
    QLCDNumber *smLcdNumber;
    QPushButton *smSwitchBtn;
    QPushButton *smSettingBtn;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *oswBtn;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(432, 202);
        QIcon icon;
        icon.addFile(QStringLiteral(":/Controller.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Widget->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(Widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(Widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        stateLed = new QPushButton(groupBox);
        stateLed->setObjectName(QStringLiteral("stateLed"));
        stateLed->setMinimumSize(QSize(48, 48));
        stateLed->setMaximumSize(QSize(48, 48));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/ledred.ico"), QSize(), QIcon::Normal, QIcon::Off);
        stateLed->setIcon(icon1);
        stateLed->setIconSize(QSize(36, 36));
        stateLed->setFlat(true);

        horizontalLayout->addWidget(stateLed);

        smLcdNumber = new QLCDNumber(groupBox);
        smLcdNumber->setObjectName(QStringLiteral("smLcdNumber"));
        smLcdNumber->setMaximumSize(QSize(16777215, 36));
        smLcdNumber->setStyleSheet(QLatin1String("color: rgb(0, 255, 0);\n"
"background-color: rgb(0, 0, 0);"));

        horizontalLayout->addWidget(smLcdNumber);

        smSwitchBtn = new QPushButton(groupBox);
        smSwitchBtn->setObjectName(QStringLiteral("smSwitchBtn"));
        smSwitchBtn->setMinimumSize(QSize(48, 48));
        smSwitchBtn->setMaximumSize(QSize(48, 48));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/Off.ico"), QSize(), QIcon::Normal, QIcon::Off);
        smSwitchBtn->setIcon(icon2);
        smSwitchBtn->setIconSize(QSize(36, 36));
        smSwitchBtn->setFlat(true);

        horizontalLayout->addWidget(smSwitchBtn);

        smSettingBtn = new QPushButton(groupBox);
        smSettingBtn->setObjectName(QStringLiteral("smSettingBtn"));
        smSettingBtn->setMinimumSize(QSize(48, 48));
        smSettingBtn->setMaximumSize(QSize(48, 48));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/settings.ico"), QSize(), QIcon::Normal, QIcon::Off);
        smSettingBtn->setIcon(icon3);
        smSettingBtn->setIconSize(QSize(36, 36));
        smSettingBtn->setFlat(true);

        horizontalLayout->addWidget(smSettingBtn);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(Widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setStyleSheet(QString::fromUtf8("font: 12pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(337, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        oswBtn = new QPushButton(groupBox_2);
        oswBtn->setObjectName(QStringLiteral("oswBtn"));
        oswBtn->setMinimumSize(QSize(48, 48));
        oswBtn->setMaximumSize(QSize(48, 48));
        oswBtn->setIcon(icon3);
        oswBtn->setIconSize(QSize(36, 36));
        oswBtn->setFlat(true);

        horizontalLayout_2->addWidget(oswBtn);


        verticalLayout->addWidget(groupBox_2);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("Widget", "Speedometer", Q_NULLPTR));
        stateLed->setText(QString());
        smSwitchBtn->setText(QString());
        smSettingBtn->setText(QString());
        groupBox_2->setTitle(QApplication::translate("Widget", "Open Sim Wheel", Q_NULLPTR));
        oswBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
