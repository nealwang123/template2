#ifndef XMLREADER_H
#define XMLREADER_H

#include <QObject>
#include <QtXml> //也可以include <QDomDocument>
class ReadXmlClass{
public:
    QString index;
    QString readdata;
    QString description;
};
class XmlReader : public QObject
{
    Q_OBJECT
public:
    static XmlReader *Instance();

private:
    static QScopedPointer<XmlReader> self;
public:
    explicit XmlReader(QObject *parent = nullptr);
    void WriteXml(QList<ReadXmlClass>&);
    QList<ReadXmlClass> ReadXml();
    //增加xml内容
    void AddXml();
    //删减xml内容
    void RemoveXml();
    //更新xml内容
    void UpdateXml();
    void setImportFile(QString im);
    void setExportFile(QString ex);

signals:

public slots:
private:
    QString importfilename;
    QString exportfilename;
    QList<ReadXmlClass> xmllist;
};

#endif // XMLREADER_H
