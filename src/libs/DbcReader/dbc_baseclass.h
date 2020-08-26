#ifndef DBC_BASECLASS_H
#define DBC_BASECLASS_H

#include <QObject>

class DBC_BaseClass : public QObject
{
    Q_OBJECT
public:
    explicit DBC_BaseClass(QObject *parent = nullptr);
    ~DBC_BaseClass(){}
signals:

public slots:
private:
    QString file;

};

#endif // DBC_BASECLASS_H
