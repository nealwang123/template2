#include "excelapi.h"
#include "qmutex.h"
#include "qfile.h"
#include "qdatetime.h"
#include "qstringlist.h"

#define DATETIME QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")

QScopedPointer<ExcelAPI> ExcelAPI::self;
ExcelAPI *ExcelAPI::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new ExcelAPI);
        }
    }

    return self.data();
}

ExcelAPI::ExcelAPI(QObject *parent) : QObject(parent)
{
}

void ExcelAPI::checkBorder(bool border, QTextStream &stream)
{
	if (border) {
		stream << ("  <Borders>\n");
		stream << ("   <Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n");
		stream << ("   <Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n");
		stream << ("   <Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n");
		stream << ("   <Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n");
		stream << ("  </Borders>\n");
	}
}

void ExcelAPI::saveExcel(const QString &fileName, const QString &sheetName, const QString &title, const QString &subTitle,
                         const QList<QString> &columnNames, const QList<int> &columnWidths, const QStringList &content,
                         bool border, bool check, int checkColumn, const QString &checkType, const QString &checkValue)
{
	//写入文件
	QFile file(fileName);

	if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
		return;
	}

    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("utf-8");

	//计算行数列数
	int columnCount = columnNames.count();
	int rowCount = content.count();

	//固定头部信息
	stream << ("<?xml version=\"1.0\"?>\n");
	stream << ("<?mso-application progid=\"Excel.Sheet\"?>\n");
	stream << ("<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
	stream << (" xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n");
	stream << (" xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n");
	stream << (" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
	stream << (" xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");

	//文档信息
	stream << (" <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
	stream << ("  <LastAuthor></LastAuthor>\n");
	stream << (QString("  <Created>%1</Created>\n").arg(DATETIME));
	stream << ("  <Version>11.6360</Version>\n");
	stream << (" </DocumentProperties>\n");

	stream << (" <OfficeDocumentSettings xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
	stream << ("  <AllowPNG/>\n");
	stream << (" </OfficeDocumentSettings>\n");

	stream << (" <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
	stream << ("  <WindowHeight>9795</WindowHeight>\n");
	stream << ("  <WindowWidth>21435</WindowWidth>\n");
	stream << ("  <WindowTopX>120</WindowTopX>\n");
	stream << ("  <WindowTopY>75</WindowTopY>\n");
	stream << ("  <WindowTopY>75</WindowTopY>\n");
	stream << ("  <ProtectWindows>False</ProtectWindows>\n");
	stream << (" </ExcelWorkbook>\n");

	//样式表
	stream << (" <Styles>\n");
	stream << ("  <Style ss:ID=\"Default\" ss:Name=\"Normal\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
	stream << ("   <Borders/>\n");
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#000000\"/>\n");
	stream << ("   <Interior/>\n");
	stream << ("   <NumberFormat/>\n");
	stream << ("   <Protection/>\n");
	stream << ("  </Style>\n");

	//标题样式居中大号加粗字体
	stream << ("  <Style ss:ID=\"s_title\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
	checkBorder(border, stream);
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"11\" ss:Color=\"#000000\" ss:Bold=\"1\"/>\n");
	stream << ("  </Style>\n");

	//标题样式--左对齐普通字体
	stream << ("  <Style ss:ID=\"s_subtitle\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Center\"/>\n");
	checkBorder(border, stream);
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#000000\" ss:Bold=\"0\"/>\n");
	stream << ("  </Style>\n");

	//正文样式
	stream << ("  <Style ss:ID=\"s_normal\">\n");
	checkBorder(border, stream);
	stream << (" </Style>\n");

	//正文样式--文字红色
	stream << ("  <Style ss:ID=\"s_red\">\n");
	checkBorder(border, stream);
	stream << ("  <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#FF0000\" ss:Bold=\"0\"/>\n");
	stream << (" </Style>\n");

	stream << (" </Styles>\n");

	//工作表名称
	stream << (QString(" <Worksheet ss:Name=\"%1\">\n").arg(sheetName));

	//表格开始
	stream << (QString("  <Table ss:ExpandedColumnCount=\"%1\" x:FullColumns=\"1\"\n").arg(columnCount));
	stream << ("   x:FullRows=\"1\" ss:DefaultColumnWidth=\"54\" ss:DefaultRowHeight=\"18\">\n");

	//设置字段宽度
	for (int i = 0; i < columnCount; i++) {
		stream << (QString("   <Column ss:AutoFitWidth=\"0\" ss:Width=\"%1\"/>\n").arg(columnWidths.at(i)));
	}

	//表格标题
	if (title.length() > 0) {
		stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"22\">\n");
		stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_title\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
		           .arg(columnCount - 1).arg(title));
		stream << ("   </Row>");
	}

	//表格副标题
	if (subTitle.length() > 0) {
		stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"18\">\n");
		stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_subtitle\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
		           .arg(columnCount - 1).arg(subTitle));
		stream << ("   </Row>");
	}

	//逐个添加字段名称
	if (columnCount > 0) {
		stream << ("   <Row ss:AutoFitHeight=\"0\">\n");

		for (int i = 0; i < columnCount; i++) {
			stream << (QString("    <Cell ss:StyleID=\"s_normal\"><Data ss:Type=\"String\">%1</Data></Cell>\n")
			           .arg(columnNames.at(i)));
		}

		stream << ("   </Row>\n");
	}

	//逐个添加数据
	for (int i = 0; i < rowCount; i++) {
		stream << ("   <Row ss:AutoFitHeight=\"0\">\n");

		QString temp = content.at(i);
		QStringList value = temp.split(";");

		//过滤内容,如果启用了过滤数据,则将符合条件的数据突出颜色显示
		bool existCheck = false;

		if (check) {
			if (checkType == "==") {
				if (value.at(checkColumn) == checkValue) {
					existCheck = true;
				}
			} else if (checkType == ">") {
				if (value.at(checkColumn) > checkValue) {
					existCheck = true;
				}
			} else if (checkType == ">=") {
				if (value.at(checkColumn) >= checkValue) {
					existCheck = true;
				}
			} else if (checkType == "<") {
				if (value.at(checkColumn) < checkValue) {
					existCheck = true;
				}
			} else if (checkType == "<=") {
				if (value.at(checkColumn) <= checkValue) {
					existCheck = true;
				}
			} else if (checkType == "!=") {
				if (value.at(checkColumn) != checkValue) {
					existCheck = true;
				}
			} else if (checkType == "contains") {
				if (value.at(checkColumn).contains(checkValue)) {
					existCheck = true;
				}
			}
		}

		if (existCheck) {
			foreach (QString str, value) {
				stream << (QString("    <Cell ss:StyleID=\"s_red\"><Data ss:Type=\"String\">%1</Data></Cell>\n").arg(str));
			}
		} else {
			foreach (QString str, value) {
				stream << (QString("    <Cell ss:StyleID=\"s_normal\"><Data ss:Type=\"String\">%1</Data></Cell>\n").arg(str));
			}
		}

		stream << ("   </Row>\n");
	}

	stream << ("  </Table>\n");

	//固定结尾格式
	stream << ("  <WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
	stream << ("   <PageSetup>\n");
	stream << ("    <Header x:Margin=\"0.3\"/>\n");
	stream << ("    <Footer x:Margin=\"0.3\"/>\n");
	stream << ("    <PageMargins x:Bottom=\"0.75\" x:Left=\"0.7\" x:Right=\"0.7\" x:Top=\"0.75\"/>\n");
	stream << ("   </PageSetup>\n");
	stream << ("   <Unsynced/>\n");
	stream << ("   <Selected/>\n");
	stream << ("   <ProtectObjects>False</ProtectObjects>\n");
	stream << ("   <ProtectScenarios>False</ProtectScenarios>\n");
	stream << ("  </WorksheetOptions>\n");
	stream << (" </Worksheet>\n");
	stream << ("</Workbook>\n");
}

void ExcelAPI::saveExcel(const QString &fileName, const QString &sheetName, const QString &title,
                         const QList<QString> &columnNames, const QList<int> &columnWidths,
                         const QStringList &subTitle1, const QStringList &subTitle2, const QStringList &content,
                         bool border, bool check, int checkColumn, const QString &checkType, const QString &checkValue)
{
	//写入文件
	QFile file(fileName);

	if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
		return;
	}

    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("utf-8");

	//计算列数
	int columnCount = columnNames.count();

	//固定头部信息
	stream << ("<?xml version=\"1.0\"?>\n");
	stream << ("<?mso-application progid=\"Excel.Sheet\"?>\n");
	stream << ("<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
	stream << (" xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n");
	stream << (" xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n");
	stream << (" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
	stream << (" xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");

	//文档信息
	stream << (" <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
	stream << (QString("  <LastAuthor>%1</LastAuthor>\n").arg(""));
	stream << (QString("  <Created>%1</Created>\n").arg(DATETIME));
	stream << ("  <Version>11.6360</Version>\n");
	stream << (" </DocumentProperties>\n");

	stream << (" <OfficeDocumentSettings xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
	stream << ("  <AllowPNG/>\n");
	stream << (" </OfficeDocumentSettings>\n");

	stream << (" <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
	stream << ("  <WindowHeight>9795</WindowHeight>\n");
	stream << ("  <WindowWidth>21435</WindowWidth>\n");
	stream << ("  <WindowTopX>120</WindowTopX>\n");
	stream << ("  <WindowTopY>75</WindowTopY>\n");
	stream << ("  <WindowTopY>75</WindowTopY>\n");
	stream << ("  <ProtectWindows>False</ProtectWindows>\n");
	stream << (" </ExcelWorkbook>\n");

	//样式表
	stream << (" <Styles>\n");
	stream << ("  <Style ss:ID=\"Default\" ss:Name=\"Normal\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
	stream << ("   <Borders/>\n");
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#000000\"/>\n");
	stream << ("   <Interior/>\n");
	stream << ("   <NumberFormat/>\n");
	stream << ("   <Protection/>\n");
	stream << ("  </Style>\n");

	//标题样式居中大号加粗字体
	stream << ("  <Style ss:ID=\"s_title\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
	checkBorder(border, stream);
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"11\" ss:Color=\"#000000\" ss:Bold=\"1\"/>\n");
	stream << ("  </Style>\n");

	//标题样式--左对齐普通字体
	stream << ("  <Style ss:ID=\"s_subtitle\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Center\"/>\n");
	checkBorder(border, stream);
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#000000\" ss:Bold=\"0\"/>\n");
	stream << ("  </Style>\n");

	//正文样式
	stream << ("  <Style ss:ID=\"s_normal\">\n");
	checkBorder(border, stream);
	stream << (" </Style>\n");

	//正文样式--文字红色
	stream << ("  <Style ss:ID=\"s_red\">\n");
	checkBorder(border, stream);
	stream << ("  <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#FF0000\" ss:Bold=\"0\"/>\n");
	stream << (" </Style>\n");

	stream << (" </Styles>\n");

	//工作表名称
	stream << (QString(" <Worksheet ss:Name=\"%1\">\n").arg(sheetName));

	//表格开始
	stream << (QString("  <Table ss:ExpandedColumnCount=\"%1\" x:FullColumns=\"1\"\n").arg(columnCount));
	stream << ("   x:FullRows=\"1\" ss:DefaultColumnWidth=\"54\" ss:DefaultRowHeight=\"18\">\n");

	//设置字段宽度
	for (int i = 0; i < columnCount; i++) {
		stream << (QString("   <Column ss:AutoFitWidth=\"0\" ss:Width=\"%1\"/>\n").arg(columnWidths.at(i)));
	}

	//表格标题
	if (title.length() > 0) {
		stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"22\">\n");
		stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_title\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
		           .arg(columnCount - 1).arg(title));
		stream << ("   </Row>");
	}

	//循环添加副标题/字段名/内容
	int count = content.count();

	for (int i = 0; i < count; i++) {
		//加个空行隔开
		stream << ("   <Row ss:AutoFitHeight=\"0\">\n");
		stream << ("   </Row>");

		//副标题1
		if (subTitle1.count() > 0 && subTitle1.count() > i) {
			if (subTitle1.at(i).length() > 0) {
				stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"18\">\n");
				stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_subtitle\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
				           .arg(columnCount - 1).arg(subTitle1.at(i)));
				stream << ("   </Row>");
			}
		}

		//副标题2
		if (subTitle2.count() > 0 && subTitle2.count() > i) {
			if (subTitle2.at(i).length() > 0) {
				stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"18\">\n");
				stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_subtitle\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
				           .arg(columnCount - 1).arg(subTitle2.at(i)));
				stream << ("   </Row>");
			}
		}

		//逐个添加字段名称
		if (columnCount > 0) {
			stream << ("   <Row ss:AutoFitHeight=\"0\">\n");

			for (int j = 0; j < columnCount; j++) {
				stream << (QString("    <Cell ss:StyleID=\"s_normal\"><Data ss:Type=\"String\">%1</Data></Cell>\n").arg(columnNames.at(j)));
			}

			stream << ("   </Row>\n");
		}

		QStringList list = content.at(i).split(";");

		//逐个添加数据
		int rowCount = list.count();

		for (int j = 0; j < rowCount; j++) {
			stream << ("   <Row ss:AutoFitHeight=\"0\">\n");
			QString temp = list.at(j);
			QStringList value = temp.split("|");
			int valueCount = value.count();

			//过滤内容,如果启用了过滤数据,则将符合条件的数据突出颜色显示
			bool existCheck = false;

			if (check) {
				if (checkType == "==") {
					if (value.at(checkColumn) == checkValue) {
						existCheck = true;
					}
				} else if (checkType == ">") {
					if (value.at(checkColumn) > checkValue) {
						existCheck = true;
					}
				} else if (checkType == ">=") {
					if (value.at(checkColumn) >= checkValue) {
						existCheck = true;
					}
				} else if (checkType == "<") {
					if (value.at(checkColumn) < checkValue) {
						existCheck = true;
					}
				} else if (checkType == "<=") {
					if (value.at(checkColumn) <= checkValue) {
						existCheck = true;
					}
				} else if (checkType == "!=") {
					if (value.at(checkColumn) != checkValue) {
						existCheck = true;
					}
				} else if (checkType == "contains") {
					if (value.at(checkColumn).contains(checkValue)) {
						existCheck = true;
					}
				}
			}

			if (existCheck) {
				for (int k = 0; k < valueCount; k++) {
					stream << (QString("    <Cell ss:StyleID=\"s_red\"><Data ss:Type=\"String\">%1</Data></Cell>\n").arg(value.at(k)));
				}
			} else {
				for (int k = 0; k < valueCount; k++) {
					stream << (QString("    <Cell ss:StyleID=\"s_normal\"><Data ss:Type=\"String\">%1</Data></Cell>\n").arg(value.at(k)));
				}
			}

			stream << ("   </Row>\n");
		}
	}

	stream << ("  </Table>\n");

	//固定结尾格式
	stream << ("  <WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
	stream << ("   <PageSetup>\n");
	stream << ("    <Header x:Margin=\"0.3\"/>\n");
	stream << ("    <Footer x:Margin=\"0.3\"/>\n");
	stream << ("    <PageMargins x:Bottom=\"0.75\" x:Left=\"0.7\" x:Right=\"0.7\" x:Top=\"0.75\"/>\n");
	stream << ("   </PageSetup>\n");
	stream << ("   <Unsynced/>\n");
	stream << ("   <Selected/>\n");
	stream << ("   <ProtectObjects>False</ProtectObjects>\n");
	stream << ("   <ProtectScenarios>False</ProtectScenarios>\n");
	stream << ("  </WorksheetOptions>\n");
	stream << (" </Worksheet>\n");
	stream << ("</Workbook>\n");
}

void ExcelAPI::saveExcel(const QString &fileName, const QString &sheetName, const QString &title,
                         const QList<QString> &columnNames, const QList<int> &columnWidths, const QStringList &content)
{
	//写入文件
	QFile file(fileName);

	if (!file.open(QFile::WriteOnly | QIODevice::Text)) {
		return;
	}

    QTextStream stream;
    stream.setDevice(&file);
    stream.setCodec("utf-8");

	//计算行数列数
	int columnCount = columnNames.count();
	int rowCount = content.count();

	//固定头部信息
	stream << ("<?xml version=\"1.0\"?>\n");
	stream << ("<?mso-application progid=\"Excel.Sheet\"?>\n");
	stream << ("<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
	stream << (" xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n");
	stream << (" xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n");
	stream << (" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\"\n");
	stream << (" xmlns:html=\"http://www.w3.org/TR/REC-html40\">\n");

	//文档信息
	stream << (" <DocumentProperties xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
	stream << ("  <LastAuthor></LastAuthor>\n");
	stream << (QString("  <Created>%1</Created>\n").arg(DATETIME));
	stream << ("  <Version>11.6360</Version>\n");
	stream << (" </DocumentProperties>\n");

	stream << (" <OfficeDocumentSettings xmlns=\"urn:schemas-microsoft-com:office:office\">\n");
	stream << ("  <AllowPNG/>\n");
	stream << (" </OfficeDocumentSettings>\n");

	stream << (" <ExcelWorkbook xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
	stream << ("  <WindowHeight>9795</WindowHeight>\n");
	stream << ("  <WindowWidth>21435</WindowWidth>\n");
	stream << ("  <WindowTopX>120</WindowTopX>\n");
	stream << ("  <WindowTopY>75</WindowTopY>\n");
	stream << ("  <WindowTopY>75</WindowTopY>\n");
	stream << ("  <ProtectWindows>False</ProtectWindows>\n");
	stream << (" </ExcelWorkbook>\n");

	//样式表
	stream << (" <Styles>\n");
	stream << ("  <Style ss:ID=\"Default\" ss:Name=\"Normal\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
	stream << ("   <Borders/>\n");
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"10\" ss:Color=\"#000000\"/>\n");
	stream << ("   <Interior/>\n");
	stream << ("   <NumberFormat/>\n");
	stream << ("   <Protection/>\n");
	stream << ("  </Style>\n");

	//标题样式居中大号加粗字体
	stream << ("  <Style ss:ID=\"s_title\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
	checkBorder(true, stream);
	stream << ("   <Font ss:FontName=\"微软雅黑\" x:CharSet=\"134\" ss:Size=\"15\" ss:Color=\"#000000\" ss:Bold=\"1\"/>\n");
	stream << ("  </Style>\n");

	//字段名样式
	stream << ("  <Style ss:ID=\"s_normal\">\n");
	stream << ("   <Alignment ss:Horizontal=\"Center\" ss:Vertical=\"Center\"/>\n");
	checkBorder(true, stream);
	stream << (" </Style>\n");

	//生成多个带背景色的样式
	QStringList colorName;
	colorName << "#FF99CC" << "#FFCC99" << "#FF8080" << "#8DB4E2" << "#EBF1DE" << "#CC99FF" << "#99CC00" << "#99CCFF" << "#FFFF00" << "#FF6600";

	for (int i = 0; i < 10; i++) {
		stream << (QString("  <Style ss:ID=\"s_color_%1\">\n").arg(i));
		stream << ("   <Alignment ss:Horizontal=\"Left\" ss:Vertical=\"Center\"/>\n");
		checkBorder(true, stream);
		stream << (QString("   <Interior ss:Color=\"%1\" ss:Pattern=\"Solid\"/>\n").arg(colorName.at(i)));
		stream << (" </Style>\n");
	}

	stream << (" </Styles>\n");

	//工作表名称
	stream << (QString(" <Worksheet ss:Name=\"%1\">\n").arg(sheetName));

	//表格开始
	stream << (QString("  <Table ss:ExpandedColumnCount=\"%1\" x:FullColumns=\"1\"\n").arg(columnCount));
	stream << ("   x:FullRows=\"1\" ss:DefaultColumnWidth=\"54\" ss:DefaultRowHeight=\"18\">\n");

	//设置字段宽度
	for (int i = 0; i < columnCount; i++) {
		stream << (QString("   <Column ss:AutoFitWidth=\"0\" ss:Width=\"%1\"/>\n").arg(columnWidths.at(i)));
	}

	//表格标题
	if (title.length() > 0) {
		stream << ("   <Row ss:AutoFitHeight=\"0\" ss:Height=\"22\">\n");
		stream << (QString("    <Cell ss:MergeAcross=\"%1\" ss:StyleID=\"s_title\"><Data ss:Type=\"String\">%2</Data></Cell>\n")
		           .arg(columnCount - 1).arg(title));
		stream << ("   </Row>");
	}

	//逐个添加字段名称
	if (columnCount > 0) {
		stream << ("   <Row ss:AutoFitHeight=\"0\">\n");

		for (int i = 0; i < columnCount; i++) {
			stream << (QString("    <Cell ss:StyleID=\"s_normal\"><Data ss:Type=\"String\">%1</Data></Cell>\n")
			           .arg(columnNames.at(i)));
		}

		stream << ("   </Row>\n");
	}

	//逐个添加数据
	for (int i = 0; i < rowCount; i++) {
		stream << ("   <Row ss:AutoFitHeight=\"0\">\n");

		QString temp = content.at(i);
		QStringList value = temp.split(";");
		int valueCount = value.count();

		//随机取背景颜色样式
		QString style = "s_normal";
		static int styleIndex = 0;

		for (int j = 0; j < valueCount; j++) {
            if (j > 1) {
				style = QString("s_color_%1").arg(styleIndex);
			}

			stream << (QString("    <Cell ss:StyleID=\"%1\"><Data ss:Type=\"String\">%2</Data></Cell>\n").arg(style).arg(value.at(j)));
		}

		styleIndex++;

		if (styleIndex > 9) {
			styleIndex = 0;
		}

		stream << ("   </Row>\n");
	}

	stream << ("  </Table>\n");

	//固定结尾格式
	stream << ("  <WorksheetOptions xmlns=\"urn:schemas-microsoft-com:office:excel\">\n");
	stream << ("   <PageSetup>\n");
	stream << ("    <Header x:Margin=\"0.3\"/>\n");
	stream << ("    <Footer x:Margin=\"0.3\"/>\n");
	stream << ("    <PageMargins x:Bottom=\"0.75\" x:Left=\"0.7\" x:Right=\"0.7\" x:Top=\"0.75\"/>\n");
	stream << ("   </PageSetup>\n");
	stream << ("   <Unsynced/>\n");
	stream << ("   <Selected/>\n");
	stream << ("   <ProtectObjects>False</ProtectObjects>\n");
	stream << ("   <ProtectScenarios>False</ProtectScenarios>\n");
	stream << ("  </WorksheetOptions>\n");
	stream << (" </Worksheet>\n");
	stream << ("</Workbook>\n");
}
