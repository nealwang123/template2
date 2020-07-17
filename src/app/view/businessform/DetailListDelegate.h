#ifndef DETAILLISTDELEGATE_H
#define DETAILLISTDELEGATE_H
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTreeView>
#include <QIcon>
//#include "DetailsListModel.h"

class DetailListDelegate : public QStyledItemDelegate
{
public:
    DetailListDelegate(QTreeView *pView, QObject *parent = 0);
private:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
    QTreeView *m_view;
    QIcon m_expandIcon;
    QIcon m_unexpandIcon;
};

#endif // DETAILLISTDELEGATE_H
