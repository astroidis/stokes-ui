#ifndef ROWEDITINGDELEGATE_H
#define ROWEDITINGDELEGATE_H

#include <QStyledItemDelegate>

class RowEditingDelegate : public QStyledItemDelegate
{
public:
    RowEditingDelegate();
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();
};

#endif // ROWEDITINGDELEGATE_H
