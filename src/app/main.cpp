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
#include<QProcess>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStringList args;
//    args.append("a");
//    QProcess p;
//    p.start("./PSBroker.exe"); //command是要执行的命令,args是参数)


    App::Intervals << "1" << "10" << "20" << "50" << "100" << "200" << "300" << "500" << "1000" << "1500" << "2000" << "3000" << "5000" << "10000";
    App::readSendData();
    App::readDeviceData();
    //管理目录列表，不存在则创建
    {
        QUIHelper::newDir("输出信息");
        QUIHelper::newDir("xmlfile");
        QUIHelper::newDir("InstructionFileSet");

    }
    QString _version("1.3.4");
    QString nameversion("测试程序"+_version);
    a.setApplicationName(QUIHelper::appName());
    a.setApplicationVersion(_version);
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
    //TrayIcon::Instance()->setMainWidget(&c.getUDSForm());
    //TrayIcon::Instance()->setMainWidget(&c.getSocketFormMain());
    TrayIcon::Instance()->setMainWidget(&c.getUDSForm());

    TrayIcon::Instance()->setIcon(":/main.ico");
    TrayIcon::Instance()->setToolTip(nameversion);
    TrayIcon::Instance()->setVisible(true);

    //初始化主窗体
    qui.setMainWidget(&c.getUDSForm());
    //qui.setMainWidget(&c.getSocketFormMain());
    qui.setTitle(nameversion);
    qui.setMinHide(false);
    qui.setAlignment(Qt::AlignCenter);
    qui.setVisible(QUIWidget::BtnMenu, true);
    qui.show();
//    QObject::connect(&qui, SIGNAL(changeStyle(QString)), &c.getMainForm(), SLOT(changeStyle(QString)));
    QObject::connect(&qui, SIGNAL(closing()), TrayIcon::Instance(), SLOT(closeAll()));
    //添加退出信号
    QObject::connect(&qui, SIGNAL(closing()), &c, SLOT(closeAll()));
    return a.exec();
}
