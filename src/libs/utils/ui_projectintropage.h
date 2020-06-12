/********************************************************************************
** Form generated from reading UI file 'projectintropage.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTINTROPAGE_H
#define UI_PROJECTINTROPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWizardPage>
#include <utils/fancylineedit.h>
#include <utils/pathchooser.h>

namespace Utils {

class Ui_ProjectIntroPage
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *descriptionLabel;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QFormLayout *formLayout;
    QLabel *nameLabel;
    Utils::FancyLineEdit *nameLineEdit;
    QLabel *pathLabel;
    Utils::PathChooser *pathChooser;
    QSpacerItem *directorySeparatorLabel;
    QCheckBox *projectsDirectoryCheckBox;
    QLabel *projectLabel;
    QComboBox *projectComboBox;
    QLabel *stateLabel;

    void setupUi(QWizardPage *Utils__ProjectIntroPage)
    {
        if (Utils__ProjectIntroPage->objectName().isEmpty())
            Utils__ProjectIntroPage->setObjectName(QStringLiteral("Utils__ProjectIntroPage"));
        Utils__ProjectIntroPage->resize(355, 289);
        verticalLayout = new QVBoxLayout(Utils__ProjectIntroPage);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        descriptionLabel = new QLabel(Utils__ProjectIntroPage);
        descriptionLabel->setObjectName(QStringLiteral("descriptionLabel"));
        descriptionLabel->setWordWrap(true);

        verticalLayout->addWidget(descriptionLabel);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer);

        frame = new QFrame(Utils__ProjectIntroPage);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        formLayout = new QFormLayout(frame);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        nameLabel = new QLabel(frame);
        nameLabel->setObjectName(QStringLiteral("nameLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, nameLabel);

        nameLineEdit = new Utils::FancyLineEdit(frame);
        nameLineEdit->setObjectName(QStringLiteral("nameLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, nameLineEdit);

        pathLabel = new QLabel(frame);
        pathLabel->setObjectName(QStringLiteral("pathLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, pathLabel);

        pathChooser = new Utils::PathChooser(frame);
        pathChooser->setObjectName(QStringLiteral("pathChooser"));

        formLayout->setWidget(6, QFormLayout::FieldRole, pathChooser);

        directorySeparatorLabel = new QSpacerItem(0, 12, QSizePolicy::Minimum, QSizePolicy::Fixed);

        formLayout->setItem(4, QFormLayout::LabelRole, directorySeparatorLabel);

        projectsDirectoryCheckBox = new QCheckBox(frame);
        projectsDirectoryCheckBox->setObjectName(QStringLiteral("projectsDirectoryCheckBox"));

        formLayout->setWidget(7, QFormLayout::SpanningRole, projectsDirectoryCheckBox);

        projectLabel = new QLabel(frame);
        projectLabel->setObjectName(QStringLiteral("projectLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, projectLabel);

        projectComboBox = new QComboBox(frame);
        projectComboBox->setObjectName(QStringLiteral("projectComboBox"));

        formLayout->setWidget(3, QFormLayout::FieldRole, projectComboBox);


        verticalLayout->addWidget(frame);

        stateLabel = new QLabel(Utils__ProjectIntroPage);
        stateLabel->setObjectName(QStringLiteral("stateLabel"));
        stateLabel->setWordWrap(true);

        verticalLayout->addWidget(stateLabel);


        retranslateUi(Utils__ProjectIntroPage);

        QMetaObject::connectSlotsByName(Utils__ProjectIntroPage);
    } // setupUi

    void retranslateUi(QWizardPage *Utils__ProjectIntroPage)
    {
        Utils__ProjectIntroPage->setTitle(QApplication::translate("Utils::ProjectIntroPage", "Introduction and Project Location", Q_NULLPTR));
        nameLabel->setText(QApplication::translate("Utils::ProjectIntroPage", "Name:", Q_NULLPTR));
        pathLabel->setText(QApplication::translate("Utils::ProjectIntroPage", "Create in:", Q_NULLPTR));
        projectsDirectoryCheckBox->setText(QApplication::translate("Utils::ProjectIntroPage", "Use as default project location", Q_NULLPTR));
        projectLabel->setText(QApplication::translate("Utils::ProjectIntroPage", "Project:", Q_NULLPTR));
    } // retranslateUi

};

} // namespace Utils

namespace Utils {
namespace Ui {
    class ProjectIntroPage: public Ui_ProjectIntroPage {};
} // namespace Ui
} // namespace Utils

#endif // UI_PROJECTINTROPAGE_H
