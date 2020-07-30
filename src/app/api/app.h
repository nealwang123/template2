#ifndef APP_H
#define APP_H

#include <QList>

class App
{
public:
    static QString FontName;        //全局字体名称
    static int FontSize;            //全局字体大小
    static QString ConfigFile;      //配置文件文件路径及名称

    static QString StyleName;       //样式文件名称
    static QString LocalDBType;     //本地数据库类型
    static QString LocalDBIP;       //本地数据库主机
    static int LocalDBPort;         //本地数据库端口
    static QString LocalDBName;     //本地数据库名称
    static QString LocalUserName;   //本地数据库用户名
    static QString LocalUserPwd;    //本地数据库密码

    static double PlotMinValue;     //曲线图Y轴最小值
    static double PlotMaxValue;     //曲线图Y轴最大值
    static double PlotDefaultValue; //曲线默认初始值
    static double TempUpperBuffer;  //高温告警缓冲值
    static double TempLimitBuffer;  //低温告警缓冲值
    static double DampUpperBuffer;  //高湿告警缓冲值
    static double DampLimitBuffer;  //低湿告警缓冲值

    static int DeviceCountClient;   //客户端设备数量
    static QString DbNameClient;    //客户端数据库名称
    static QString HostNameClient;  //客户端数据库地址
    static int PortClient;          //客户端数据库端口
    static QString UserNameClient;  //客户端数据库用户名
    static QString UserPwdClient;   //客户端数据库用户密码

    static int DeviceCountServer;   //服务端设备数量
    static QString DbNameServer;    //服务端数据库名称
    static QString HostNameServer;  //服务端数据库地址
    static int PortServer;          //服务端数据库端口
    static QString UserNameServer;  //服务端数据库用户名
    static QString UserPwdServer;   //服务端数据库用户密码
    static int DeviceColumn;        //设备列数

    static QString DbNameLocal;     //本地数据库名称
    static QString HostNameLocal;   //本地数据库地址
    static int PortLocal;           //本地数据库端口
    static QString UserNameLocal;   //本地数据库用户名
    static QString UserPwdLocal;    //本地数据库用户密码

    static QString DbNameNet;       //远程数据库名称
    static QString HostNameNet;     //远程数据库地址
    static int PortNet;             //远程数据库端口
    static QString UserNameNet;     //远程数据库用户名
    static QString UserPwdNet;      //远程数据库用户密码

    static QString DbNameClean;     //清理数据库名称
    static QString HostNameClean;   //清理数据库地址
    static int PortClean;           //清理数据库端口
    static QString UserNameClean;   //清理数据库用户名
    static QString UserPwdClean;    //清理数据库用户密码
    static int CleanCount;          //保留最大记录数
    static int CleanInterval;       //清理数据库定时间隔

    static QString DbNameData;      //数据发生器数据库名称
    static QString HostNameData;    //数据发生器数据库地址
    static int PortData;            //数据发生器数据库端口
    static QString UserNameData;    //数据发生器数据库用户名
    static QString UserPwdData;     //数据发生器数据库用户密码
    static int DataCount;           //每次数据数量
    static int DataInterval;        //数据发生间隔

    static QString SqliteDBName;    //sqlite数据库名称
    static QString SqliteSql;       //sqlite数据库脚本
    static QString MysqlDBIP;       //mysql服务器IP
    static int MysqlDBPort;         //mysql服务器端口
    static QString MysqlDBName;     //mysql数据库名称
    static QString MysqlUserName;   //mysql数据库用户名
    static QString MysqlUserPwd;    //mysql数据库密码
    static QString MysqlSql;        //mysql数据库脚本

    //add config
    static int AlignCount;
    static int AlignFilter;
    static QString AlignAngle;
    static QString AlignDistance;

    //TCP客户端配置参数
    static bool HexSendTcpClient;       //16进制发送
    static bool HexReceiveTcpClient;    //16进制接收
    static bool AsciiTcpClient;         //ASCII模式
    static bool DebugTcpClient;         //启用数据调试
    static bool AutoSendTcpClient;      //自动发送数据
    static int IntervalTcpClient;       //发送数据间隔
    static QString TcpServerIP;         //服务器IP
    static int TcpServerPort;           //服务器端口

    //EOL config
    static QString HWVersion;
    static int OnlineCaStatDelay;//在线设备统计延时量
    static QString OnlineBurnfile;//固件升级文件路径

    //轨迹配置
    static int XWidth;         //水平
    static int YHeight;        //纵向
    static double ZeroPointX;         //零点X
    static double ZeroPointY;        //零点Y

    static void readConfig();       //读取配置文件,在main函数最开始加载程序载入
    static void writeConfig();      //写入配置文件,在更改配置文件程序关闭时调用
    static void newConfig();        //以初始值新建配置文件
    static bool checkConfig();      //校验配置文件

    static QString SendFileName;        //发送配置文件名
    static QString DeviceFileName;      //模拟设备数据文件名

    static QStringList Intervals;
    static QStringList Datas;
    static QStringList Keys;
    static QStringList Values;
    static void readSendData();
    static void readDeviceData();

};

#endif // APP_H
