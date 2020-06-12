/********************************************************************************
** Form generated from reading UI file 'removefiledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOVEFILEDIALOG_H
#define UI_REMOVEFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

namespace Utils {

class Ui_RemoveFileDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *fileToDeleteLabel;
    QLabel *fileNameLabel;
    QSpacerItem *verticalSpacer;
    QCheckBox *deleteFileCheckBox;
    QCheckBox *removeVCCheckBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Utils__RemoveFileDialog)
    {
        if (Utils__RemoveFileDialog->objectName().isEmpty())
            Utils__RemoveFileDialog->setObjectName(QStringLiteral("Utils__RemoveFileDialog"));
        Utils__RemoveFileDialog->resize(514, 159);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Utils__RemoveFileDialog->sizePolicy().hasHeightForWidth());
        Utils__RemoveFileDialog->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(Utils__RemoveFileDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        fileToDeleteLabel = new QLabel(Utils__RemoveFileDialog);
        fileToDeleteLabel->setObjectName(QStringLiteral("fileToDeleteLabel"));
        sizePolicy.setHeightForWidth(fileToDeleteLabel->sizePolicy().hasHeightForWidth());
        fileToDeleteLabel->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(fileToDeleteLabel);

        fileNameLabel = new QLabel(Utils__RemoveFileDialog);
        fileNameLabel->setObjectName(QStringLiteral("fileNameLabel"));
        sizePolicy.setHeightForWidth(fileNameLabel->sizePolicy().hasHeightForWidth());
        fileNameLabel->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QStringLiteral("Courier New"));
        fileNameLabel->setFont(font);
        fileNameLabel->setText(QStringLiteral("placeholder"));
        fileNameLabel->setWordWrap(true);

        verticalLayout->addWidget(fileNameLabel);

        verticalSpacer = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        deleteFileCheckBox = new QCheckBox(Utils__RemoveFileDialog);
        deleteFileCheckBox->setObjectName(QStringLiteral("deleteFileCheckBox"));

        verticalLayout->addWidget(deleteFileCheckBox);

        removeVCCheckBox = new QCheckBox(Utils__RemoveFileDialog);
        removeVCCheckBox->setObjectName(QStringLiteral("removeVCCheckBox"));

        verticalLayout->addWidget(removeVCCheckBox);

        buttonBox = new QDialogButtonBox(Utils__RemoveFileDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy1);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Utils__RemoveFileDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Utils__RemoveFileDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Utils__RemoveFileDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Utils__RemoveFileDialog);
    } // setupUi

    void retranslateUi(QDialog *Utils__RemoveFileDialog)
    {
        Utils__RemoveFileDialog->setWindowTitle(QApplication::translate("Utils::RemoveFileDialog", "Remove File", Q_NULLPTR));
        fileToDeleteLabel->setText(QApplication::translate("Utils::RemoveFileDialog", "File to remove:", Q_NULLPTR));
        deleteFileCheckBox->setText(QApplication::translate("Utils::RemoveFileDialog", "&Delete file permanently", Q_NULLPTR));
        removeVCCheckBox->setText(QApplication::translate("Utils::RemoveFileDialog", "&Remove from version control", Q_NULLPTR));
    } // retranslateUi

};

} // namespace Utils

namespace Utils {
namespace Ui {
    class RemoveFileDialog: public Ui_RemoveFileDialog {};
} // namespace Ui
} // namespace Utils

#endif // UI_REMOVEFILEDIALOG_H
