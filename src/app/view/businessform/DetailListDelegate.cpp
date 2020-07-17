#include "DetailListDelegate.h"
#include <QMouseEvent>
#include <QApplication>
#include <QToolTip>
DetailListDelegate::DetailListDelegate(QTreeView *pView, QObject *parent) : QStyledItemDelegate(parent),
    m_view(pView),
    m_expandIcon(QIcon(QPixmap(":/icon/icon/atos_icon_style_2/arrow_down.png"))),
    m_unexpandIcon(QIcon(QPixmap(":/icon/icon/atos_icon_style_2/arrow_right.png")))
{
    m_view->installEventFilter(parent);
    m_view->setMouseTracking(true);
}

void DetailListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem  viewOption(option);

    if(!index.isValid())
        return ;
    DetailsTreeItem *node = static_cast<DetailsTreeItem *>(index.internalPointer());
    if( !node)
        return ;
    if(nullptr == node->parent())
        return ;
    QRect rectText = option.rect;
    QRect rectLogo = option.rect;
    QPen textPen(QColor(255, 255, 255));
    QIcon::Mode iconMode = QIcon::Normal;
    if(option.state.testFlag(QStyle::State_Selected)){
        painter->setPen(Qt::NoPen);
        painter->fillRect(option.rect,QColor(217,136,7));

        if(option.state.testFlag(QStyle::State_MouseOver)) {
           textPen.setColor(QColor(0,0,0));
        }
    }else if(option.state.testFlag(QStyle::State_MouseOver)) {
        painter->setPen(Qt::NoPen);
        painter->fillRect(option.rect,QColor(57,57,57));
        textPen.setColor(QColor(255,180,0));
    }

    painter->setPen(textPen);
    if(!option.state.testFlag(QStyle::State_Enabled)) {
        QPen textPen(QColor(120, 120, 120));
        painter->setPen(textPen);
        iconMode = QIcon::Disabled;
    }



    if(node->parent()->parent()){
        rectText.setX(option.rect.x() + 60);
        rectLogo.setX(rectLogo.x() + 25);  
    } else {
        rectText.setX(option.rect.x() + 40);
        rectLogo.setX(rectLogo.x() + 5);
    }
    rectLogo.setWidth(25);
    rectLogo.setHeight(25);
    painter->drawPixmap(rectLogo,node->icon().pixmap(QSize(65,65),iconMode));
    rectText.setY(rectText.y() + 7);
    painter->drawText(rectText,node->data());

    bool bExpanded = false;
    if(!m_view)
        return ;
    if(node->children().size() == 0 )
        return ;
    bExpanded = m_view->isExpanded(index);
    QPixmap pixmap = bExpanded ? m_expandIcon.pixmap(QSize(32,32)): m_unexpandIcon.pixmap(QSize(32,32));
    QRect rectExpand = QRect(option.rect.left() + option.rect.width() - 30, option.rect.top() + 8, 15, 15);
    painter->drawPixmap(rectExpand,pixmap);
}

QSize DetailListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    return size;
}

bool DetailListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{

    QRect rectExpand = QRect(option.rect.left() + option.rect.width() - 30, option.rect.top() + 8, 15, 15);
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    bool bExpanded = false;
    if (m_view != NULL)
    {
        const QAbstractItemModel *model = index.model();
        if (!model->hasChildren(index))
        {
            return false;
        }
        bExpanded = m_view->isExpanded(index);
    }
    if (event->type() == QEvent::MouseButtonPress && rectExpand.contains(mouseEvent->pos()))
    {
        m_view->setExpanded(index, !bExpanded);
    }

    if (event->type() == QEvent::MouseMove && rectExpand.contains(mouseEvent->pos()))
    {

        QCursor cursor(Qt::PointingHandCursor);
        QApplication::setOverrideCursor(cursor);
        QString strText = bExpanded ? tr("collapse") : tr("expand");
        QToolTip::showText(mouseEvent->globalPos(), strText);
    }
    else
    {
        QCursor cursor(Qt::ArrowCursor);
        QApplication::setOverrideCursor(cursor);
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}
