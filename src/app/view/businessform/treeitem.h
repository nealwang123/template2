#ifndef TREEITEM_H
#define TREEITEM_H

#include <QVariant>
#include"define.h"
class TreeItem
{
public:
    explicit TreeItem(TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);      //在本节点下增加子节点
    void removeChilds();                    //清空所有节点

    TreeItem *child(int row);               //获取第row个子节点指针
    TreeItem *parentItem();                 //获取父节点指针
    int childCount() const;                 //子节点计数
    int row() const;                        //获取该节点是父节点的第几个子节点

    //核心函数：获取节点第column列的数据
    QVariant data(int column) const;

    //设置、获取节点是几级节点（就是树的层级）
    int level(){ return mLevel; }
    void setLevel(int level){ mLevel = level; }

    //设置、获取节点存的数据指针
    void setPtr(void* p){ mPtr = p; }
    void* ptr(){ return mPtr; }

    //保存该节点是其父节点的第几个子节点，查询优化所用
    void setRow(int row){
        mRow = row;
    }
	
private:
    QList<TreeItem*> mChildItems;   //子节点
    TreeItem *mParentItem;          //父节点
    int mLevel;     //该节点是第几级节点
    void* mPtr;     //存储数据的指针
    int mRow;       //记录该item是第几个，可优化查询效率

};

#endif
