#ifndef ITEMDEF_H
#define ITEMDEF_H

#include <QMetaType>
#include <QString>
typedef enum{
    S_RED,
    S_BLUE,
    S_YELLOW,
} ItemStatus;

struct ItemData{
    int id;
    QString date;
    int state;
};

Q_DECLARE_METATYPE(ItemData)

#endif // ITEMDEF_H
