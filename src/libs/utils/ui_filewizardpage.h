/********************************************************************************
** Form generated from reading UI file 'filewizardpage.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEWIZARDPAGE_H
#define UI_FILEWIZARDPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWizardPage>
#include <utils/filenamevalidatinglineedit.h>
#include <utils/pathchooser.h>

namespace Utils {

class Ui_WizardPage
{
public:
    QFormLayout *formLayout;
    QLabel *nameLabel;
    Utils::FileNameValidatingLineEdit *nameLineEdit;
    QLabel *pathLabel;
    Utils::PathChooser *pathChooser;

    void setupUi(QWizardPage *Utils__WizardPage)
    {
        if (Utils__WizardPage->objectName().isEmpty())
            Utils__WizardPage->setObjectName(QStringLiteral("Utils__WizardPage"));
        Utils__WizardPage->resize(196, 68);
        formLayout = new QFormLayout(Utils__WizardPage);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        nameLabel = new QLabel(Utils__WizardPage);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, nameLabel);

        nameLineEdit = new Utils::FileNameValidatingLineEdit(Utils__WizardPage);
        nameLineEdit->setObjectName(QStringLiteral("nameLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        pathLabel = new QLabel(Utils__WizardPage);
        pathLabel->setObjectName(QStringLiteral("pathLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, pathLabel);

        pathChooser = new Utils::PathChooser(Utils__WizardPage);
        pathChooser->setObjectName(QStringLiteral("pathChooser"));

        formLayout->setWidget(1, QFormLayout::FieldRole, pathChooser);


        retranslateUi(Utils__WizardPage);

        QMetaObject::connectSlotsByName(Utils__WizardPage);
    } // setupUi

    void retranslateUi(QWizardPage *Utils__WizardPage)
    {
        Utils__WizardPage->setTitle(QApplication::translate("Utils::WizardPage", "Choose the Location", Q_NULLPTR));
        nameLabel->setText(QApplication::translate("Utils::WizardPage", "Name:", Q_NULLPTR));
        pathLabel->setText(QApplication::translate("Utils::WizardPage", "Path:", Q_NULLPTR));
    } // retranslateUi

};

} // namespace Utils

namespace Utils {
namespace Ui {
    class WizardPage: public Ui_WizardPage {};
} // namespace Ui
} // namespace Utils

#endif // UI_FILEWIZARDPAGE_H
