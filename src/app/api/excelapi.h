#ifndef EXCELAPI_H
#define EXCELAPI_H

#include <QObject>
#include <QStringList>
#include <QTextStream>

class ExcelAPI : public QObject
{
	Q_OBJECT
public:
    static ExcelAPI *Instance();
    explicit ExcelAPI(QObject *parent = 0);

private:
    static QScopedPointer<ExcelAPI> self;

private:
	void checkBorder(bool border, QTextStream &stream);

public slots:
    //导出数据到excel
    void saveExcel(const QString &fileName, const QString &sheetName, const QString &title, const QString &subTitle,
                   const QList<QString> &columnNames, const QList<int> &columnWidths, const QStringList &content,
                   bool border = true, bool check = false, int checkColumn = 0,
                   const QString &checkType = "==", const QString &checkValue = "0");

    //导出数据到excel,分段形式,带子标题
    void saveExcel(const QString &fileName, const QString &sheetName, const QString &title,
                   const QList<QString> &columnNames, const QList<int> &columnWidths,
                   const QStringList &subTitle1, const QStringList &subTitle2, const QStringList &content,
                   bool border = true, bool check = false, int checkColumn = 0,
                   const QString &checkType = "==", const QString &checkValue = "0");

    //导出数据到excel,随机背景颜色
    void saveExcel(const QString &fileName, const QString &sheetName, const QString &title,
                   const QList<QString> &columnNames, const QList<int> &columnWidths, const QStringList &content);

};

#endif // EXCELAPI_H
