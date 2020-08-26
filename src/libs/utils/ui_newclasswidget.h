/********************************************************************************
** Form generated from reading UI file 'newclasswidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWCLASSWIDGET_H
#define UI_NEWCLASSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <utils/classnamevalidatinglineedit.h>
#include <utils/filenamevalidatinglineedit.h>
#include <utils/pathchooser.h>

namespace Utils {

class Ui_NewClassWidget
{
public:
    QFormLayout *formLayout;
    QLabel *classNameLabel;
    Utils::ClassNameValidatingLineEdit *classLineEdit;
    QLabel *baseClassLabel;
    QComboBox *baseClassComboBox;
    QLabel *classTypeLabel;
    QComboBox *classTypeComboBox;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QLabel *headerLabel;
    Utils::FileNameValidatingLineEdit *headerFileLineEdit;
    QLabel *sourceLabel;
    Utils::FileNameValidatingLineEdit *sourceFileLineEdit;
    QLabel *generateFormLabel;
    QCheckBox *generateFormCheckBox;
    QLabel *formLabel;
    Utils::FileNameValidatingLineEdit *formFileLineEdit;
    QLabel *pathLabel;
    Utils::PathChooser *pathChooser;

    void setupUi(QWidget *Utils__NewClassWidget)
    {
        if (Utils__NewClassWidget->objectName().isEmpty())
            Utils__NewClassWidget->setObjectName(QStringLiteral("Utils__NewClassWidget"));
        formLayout = new QFormLayout(Utils__NewClassWidget);
        formLayout->setContentsMargins(0, 0, 0, 0);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        classNameLabel = new QLabel(Utils__NewClassWidget);
        classNameLabel->setObjectName(QStringLiteral("classNameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, classNameLabel);

        classLineEdit = new Utils::ClassNameValidatingLineEdit(Utils__NewClassWidget);
        classLineEdit->setObjectName(QStringLiteral("classLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, classLineEdit);

        baseClassLabel = new QLabel(Utils__NewClassWidget);
        baseClassLabel->setObjectName(QStringLiteral("baseClassLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, baseClassLabel);

        baseClassComboBox = new QComboBox(Utils__NewClassWidget);
        baseClassComboBox->setObjectName(QStringLiteral("baseClassComboBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(baseClassComboBox->sizePolicy().hasHeightForWidth());
        baseClassComboBox->setSizePolicy(sizePolicy);

        formLayout->setWidget(1, QFormLayout::FieldRole, baseClassComboBox);

        classTypeLabel = new QLabel(Utils__NewClassWidget);
        classTypeLabel->setObjectName(QStringLiteral("classTypeLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, classTypeLabel);

        classTypeComboBox = new QComboBox(Utils__NewClassWidget);
        classTypeComboBox->setObjectName(QStringLiteral("classTypeComboBox"));

        formLayout->setWidget(2, QFormLayout::FieldRole, classTypeComboBox);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);

        formLayout->setItem(3, QFormLayout::LabelRole, verticalSpacer);

        verticalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Fixed);

        formLayout->setItem(3, QFormLayout::FieldRole, verticalSpacer_2);

        headerLabel = new QLabel(Utils__NewClassWidget);
        headerLabel->setObjectName(QStringLiteral("headerLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, headerLabel);

        headerFileLineEdit = new Utils::FileNameValidatingLineEdit(Utils__NewClassWidget);
        headerFileLineEdit->setObjectName(QStringLiteral("headerFileLineEdit"));

        formLayout->setWidget(4, QFormLayout::FieldRole, headerFileLineEdit);

        sourceLabel = new QLabel(Utils__NewClassWidget);
        sourceLabel->setObjectName(QStringLiteral("sourceLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, sourceLabel);

        sourceFileLineEdit = new Utils::FileNameValidatingLineEdit(Utils__NewClassWidget);
        sourceFileLineEdit->setObjectName(QStringLiteral("sourceFileLineEdit"));

        formLayout->setWidget(5, QFormLayout::FieldRole, sourceFileLineEdit);

        generateFormLabel = new QLabel(Utils__NewClassWidget);
        generateFormLabel->setObjectName(QStringLiteral("generateFormLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, generateFormLabel);

        generateFormCheckBox = new QCheckBox(Utils__NewClassWidget);
        generateFormCheckBox->setObjectName(QStringLiteral("generateFormCheckBox"));

        formLayout->setWidget(6, QFormLayout::FieldRole, generateFormCheckBox);

        formLabel = new QLabel(Utils__NewClassWidget);
        formLabel->setObjectName(QStringLiteral("formLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, formLabel);

        formFileLineEdit = new Utils::FileNameValidatingLineEdit(Utils__NewClassWidget);
        formFileLineEdit->setObjectName(QStringLiteral("formFileLineEdit"));

        formLayout->setWidget(7, QFormLayout::FieldRole, formFileLineEdit);

        pathLabel = new QLabel(Utils__NewClassWidget);
        pathLabel->setObjectName(QStringLiteral("pathLabel"));

        formLayout->setWidget(8, QFormLayout::LabelRole, pathLabel);

        pathChooser = new Utils::PathChooser(Utils__NewClassWidget);
        pathChooser->setObjectName(QStringLiteral("pathChooser"));

        formLayout->setWidget(8, QFormLayout::FieldRole, pathChooser);

#ifndef QT_NO_SHORTCUT
        classNameLabel->setBuddy(classLineEdit);
        baseClassLabel->setBuddy(baseClassComboBox);
        classTypeLabel->setBuddy(classTypeComboBox);
        headerLabel->setBuddy(headerFileLineEdit);
        sourceLabel->setBuddy(sourceFileLineEdit);
        generateFormLabel->setBuddy(generateFormCheckBox);
        formLabel->setBuddy(formFileLineEdit);
        pathLabel->setBuddy(pathLabel);
#endif // QT_NO_SHORTCUT

        retranslateUi(Utils__NewClassWidget);

        QMetaObject::connectSlotsByName(Utils__NewClassWidget);
    } // setupUi

    void retranslateUi(QWidget *Utils__NewClassWidget)
    {
        classNameLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Class name:", Q_NULLPTR));
        baseClassLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Base class:", Q_NULLPTR));
        classTypeLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Type information:", Q_NULLPTR));
        classTypeComboBox->clear();
        classTypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("Utils::NewClassWidget", "None", Q_NULLPTR)
         << QApplication::translate("Utils::NewClassWidget", "Inherits QObject", Q_NULLPTR)
         << QApplication::translate("Utils::NewClassWidget", "Inherits QWidget", Q_NULLPTR)
         << QApplication::translate("Utils::NewClassWidget", "Inherits QDeclarativeItem - Qt Quick 1", Q_NULLPTR)
         << QApplication::translate("Utils::NewClassWidget", "Inherits QQuickItem - Qt Quick 2", Q_NULLPTR)
         << QApplication::translate("Utils::NewClassWidget", "Based on QSharedData", Q_NULLPTR)
        );
        headerLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Header file:", Q_NULLPTR));
        sourceLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Source file:", Q_NULLPTR));
        generateFormLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Generate form:", Q_NULLPTR));
        generateFormCheckBox->setText(QString());
        formLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Form file:", Q_NULLPTR));
        pathLabel->setText(QApplication::translate("Utils::NewClassWidget", "&Path:", Q_NULLPTR));
        Q_UNUSED(Utils__NewClassWidget);
    } // retranslateUi

};

} // namespace Utils

namespace Utils {
namespace Ui {
    class NewClassWidget: public Ui_NewClassWidget {};
} // namespace Ui
} // namespace Utils

#endif // UI_NEWCLASSWIDGET_H
