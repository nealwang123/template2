/********************************************************************************
** Form generated from reading UI file 'proxycredentialsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROXYCREDENTIALSDIALOG_H
#define UI_PROXYCREDENTIALSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

namespace Utils {

class Ui_ProxyCredentialsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *infotext;
    QFormLayout *formLayout;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Utils__ProxyCredentialsDialog)
    {
        if (Utils__ProxyCredentialsDialog->objectName().isEmpty())
            Utils__ProxyCredentialsDialog->setObjectName(QStringLiteral("Utils__ProxyCredentialsDialog"));
        Utils__ProxyCredentialsDialog->resize(279, 114);
        verticalLayout = new QVBoxLayout(Utils__ProxyCredentialsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        infotext = new QLabel(Utils__ProxyCredentialsDialog);
        infotext->setObjectName(QStringLiteral("infotext"));

        verticalLayout->addWidget(infotext);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        usernameLabel = new QLabel(Utils__ProxyCredentialsDialog);
        usernameLabel->setObjectName(QStringLiteral("usernameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, usernameLabel);

        usernameLineEdit = new QLineEdit(Utils__ProxyCredentialsDialog);
        usernameLineEdit->setObjectName(QStringLiteral("usernameLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, usernameLineEdit);

        passwordLabel = new QLabel(Utils__ProxyCredentialsDialog);
        passwordLabel->setObjectName(QStringLiteral("passwordLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, passwordLabel);

        passwordLineEdit = new QLineEdit(Utils__ProxyCredentialsDialog);
        passwordLineEdit->setObjectName(QStringLiteral("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(1, QFormLayout::FieldRole, passwordLineEdit);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(Utils__ProxyCredentialsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Utils__ProxyCredentialsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Utils__ProxyCredentialsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Utils__ProxyCredentialsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Utils__ProxyCredentialsDialog);
    } // setupUi

    void retranslateUi(QDialog *Utils__ProxyCredentialsDialog)
    {
        Utils__ProxyCredentialsDialog->setWindowTitle(QApplication::translate("Utils::ProxyCredentialsDialog", "Proxy Credentials", Q_NULLPTR));
        infotext->setText(QApplication::translate("Utils::ProxyCredentialsDialog", "The proxy %1 requires a username and password.", Q_NULLPTR));
        usernameLabel->setText(QApplication::translate("Utils::ProxyCredentialsDialog", "Username:", Q_NULLPTR));
        usernameLineEdit->setPlaceholderText(QApplication::translate("Utils::ProxyCredentialsDialog", "Username", Q_NULLPTR));
        passwordLabel->setText(QApplication::translate("Utils::ProxyCredentialsDialog", "Password:", Q_NULLPTR));
        passwordLineEdit->setPlaceholderText(QApplication::translate("Utils::ProxyCredentialsDialog", "Password", Q_NULLPTR));
    } // retranslateUi

};

} // namespace Utils

namespace Utils {
namespace Ui {
    class ProxyCredentialsDialog: public Ui_ProxyCredentialsDialog {};
} // namespace Ui
} // namespace Utils

#endif // UI_PROXYCREDENTIALSDIALOG_H
