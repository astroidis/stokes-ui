#include "roweditingdelegate.h"
#include "editexperimentdialog.h"
#include <QSqlRecord>

RowEditingDelegate::RowEditingDelegate()
{

}

QWidget *RowEditingDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    EditExperimentDialog *dlg = new EditExperimentDialog();
    return dlg;
}

void RowEditingDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSqlRecord rec = qobject_cast<const QSqlTableModel *>(index.model())->record(index.row());
    EditExperimentDialog *dlg = qobject_cast<EditExperimentDialog *>(editor);
    dlg->initFieldData(rec);
}

void RowEditingDelegate::setModelData(QWidget *editor,
                                      QAbstractItemModel *model,
                                      const QModelIndex &index) const
{
//    EditExperimentDialog *dialog = qobject_cast<EditExperimentDialog *>(editor);

}

void RowEditingDelegate::commitAndCloseEditor()
{
    EditExperimentDialog *editor = qobject_cast<EditExperimentDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
