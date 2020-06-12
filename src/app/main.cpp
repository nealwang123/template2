/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

//#include "../tools/qtcreatorcrashhandler/crashhandlersetup.h"
//#include <qtsingleapplication.h>
#include <QApplication>
#include"maincontroller.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    const QString libexecPath = QCoreApplication::applicationDirPath()
//            + '/' + RELATIVE_LIBEXEC_PATH;
//#ifdef ENABLE_QT_BREAKPAD
//    QtSystemExceptionHandler systemExceptionHandler(libexecPath);
//#else
//    // Display a backtrace once a serious signal is delivered (Linux only).
//    CrashHandlerSetup setupCrashHandler(Core::Constants::IDE_DISPLAY_NAME,
//                                        CrashHandlerSetup::EnableRestart, libexecPath);
//#endif
    a.setApplicationName(QUIHelper::appName());
    a.setApplicationVersion("1.0.0");
    a.setWindowIcon(QIcon(":/main.ico"));
    QFont font;
    font.setFamily(QUIConfig::FontName);
    font.setPixelSize(QUIConfig::FontSize);
    a.setFont(font);
    AppInit::Instance()->start();
    QUIWidget qui;
    MainController c;
//    MainWindow frm;

//    frm.show();
//    frmMain frm;
    //初始化托盘
    TrayIcon::Instance()->setMainWidget(&c.getMainForm());
    TrayIcon::Instance()->setIcon(":/main.ico");
    TrayIcon::Instance()->setToolTip("测试程序");
    TrayIcon::Instance()->setVisible(true);

    //初始化主窗体
    qui.setMainWidget(&c.getMainForm());
    qui.setTitle("Main");
    qui.setMinHide(false);
    qui.setAlignment(Qt::AlignCenter);
    qui.setVisible(QUIWidget::BtnMenu, true);
    qui.show();
    QObject::connect(&qui, SIGNAL(changeStyle(QString)), &c.getMainForm(), SLOT(changeStyle(QString)));
    QObject::connect(&qui, SIGNAL(closing()), TrayIcon::Instance(), SLOT(closeAll()));

    return a.exec();
}
