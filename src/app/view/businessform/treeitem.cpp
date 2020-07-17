#include <QStringList>
#include <QDateTime>
#include "TreeItem.h"
#include "define.h"

TreeItem::TreeItem(TreeItem *parent)
{
    mParentItem = parent;
    mPtr = NULL;
    mLevel = 0;
	mRow = 0;
}

TreeItem::~TreeItem()
{
    removeChilds();
}

void TreeItem::appendChild(TreeItem *item)
{
    item->setRow(mChildItems.size());   //item存自己是第几个，可以优化效率
    mChildItems.append(item);
}

void TreeItem::removeChilds()
{
    qDeleteAll(mChildItems);
    mChildItems.clear();
}

TreeItem *TreeItem::child(int row)
{
    return mChildItems.value(row);
}

TreeItem *TreeItem::parentItem()
{
    return mParentItem;
}

int TreeItem::childCount() const
{
    return mChildItems.count();
}

int TreeItem::row() const
{
    return mRow;
}

QVariant TreeItem::data(int column) const
{
    if(mLevel == 1)
    {
        //一级节点，班级
        if(column == 0)
        {
            CLASS* c = (CLASS*)mPtr;
            return c->name;
        }
    }
    else if(mLevel==2)
    {
        //二级节点学生信息
        STUDENT* s = (STUDENT*)mPtr;
        switch (column)
        {
        case 0: return s->name;
        case 1: return s->time;

        default:
            return QVariant();
        }
    }
    return QVariant();
}




