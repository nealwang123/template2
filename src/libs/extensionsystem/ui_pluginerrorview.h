/********************************************************************************
** Form generated from reading UI file 'pluginerrorview.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLUGINERRORVIEW_H
#define UI_PLUGINERRORVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

namespace ExtensionSystem {
namespace Internal {

class Ui_PluginErrorView
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *state;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QTextEdit *errorString;

    void setupUi(QWidget *ExtensionSystem__Internal__PluginErrorView)
    {
        if (ExtensionSystem__Internal__PluginErrorView->objectName().isEmpty())
            ExtensionSystem__Internal__PluginErrorView->setObjectName(QStringLiteral("ExtensionSystem__Internal__PluginErrorView"));
        ExtensionSystem__Internal__PluginErrorView->resize(579, 342);
        gridLayout = new QGridLayout(ExtensionSystem__Internal__PluginErrorView);
        gridLayout->setContentsMargins(2, 2, 2, 2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(ExtensionSystem__Internal__PluginErrorView);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        state = new QLabel(ExtensionSystem__Internal__PluginErrorView);
        state->setObjectName(QStringLiteral("state"));

        gridLayout->addWidget(state, 0, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(ExtensionSystem__Internal__PluginErrorView);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        verticalLayout->addWidget(label_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        errorString = new QTextEdit(ExtensionSystem__Internal__PluginErrorView);
        errorString->setObjectName(QStringLiteral("errorString"));
        errorString->setTabChangesFocus(true);
        errorString->setReadOnly(true);

        gridLayout->addWidget(errorString, 1, 1, 1, 1);


        retranslateUi(ExtensionSystem__Internal__PluginErrorView);

        QMetaObject::connectSlotsByName(ExtensionSystem__Internal__PluginErrorView);
    } // setupUi

    void retranslateUi(QWidget *ExtensionSystem__Internal__PluginErrorView)
    {
        label->setText(QApplication::translate("ExtensionSystem::Internal::PluginErrorView", "State:", Q_NULLPTR));
        label_2->setText(QApplication::translate("ExtensionSystem::Internal::PluginErrorView", "Error message:", Q_NULLPTR));
        Q_UNUSED(ExtensionSystem__Internal__PluginErrorView);
    } // retranslateUi

};

} // namespace Internal
} // namespace ExtensionSystem

namespace ExtensionSystem {
namespace Internal {
namespace Ui {
    class PluginErrorView: public Ui_PluginErrorView {};
} // namespace Ui
} // namespace Internal
} // namespace ExtensionSystem

#endif // UI_PLUGINERRORVIEW_H
