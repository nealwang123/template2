/********************************************************************************
** Form generated from reading UI file 'helloworldwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELLOWORLDWIDGET_H
#define UI_HELLOWORLDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelloWorldWidget
{
public:
    QHBoxLayout *horizontalLayout;
    QSlider *verticalSlider;
    QTextEdit *textEdit;

    void setupUi(QWidget *HelloWorldWidget)
    {
        if (HelloWorldWidget->objectName().isEmpty())
            HelloWorldWidget->setObjectName(QStringLiteral("HelloWorldWidget"));
        HelloWorldWidget->resize(100, 337);
        HelloWorldWidget->setMinimumSize(QSize(100, 0));
        horizontalLayout = new QHBoxLayout(HelloWorldWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 1, -1, -1);
        verticalSlider = new QSlider(HelloWorldWidget);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setLayoutDirection(Qt::RightToLeft);
        verticalSlider->setOrientation(Qt::Vertical);

        horizontalLayout->addWidget(verticalSlider);

        textEdit = new QTextEdit(HelloWorldWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        horizontalLayout->addWidget(textEdit);


        retranslateUi(HelloWorldWidget);

        QMetaObject::connectSlotsByName(HelloWorldWidget);
    } // setupUi

    void retranslateUi(QWidget *HelloWorldWidget)
    {
        HelloWorldWidget->setWindowTitle(QApplication::translate("HelloWorldWidget", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class HelloWorldWidget: public Ui_HelloWorldWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELLOWORLDWIDGET_H
