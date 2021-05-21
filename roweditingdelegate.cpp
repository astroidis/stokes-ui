#include "roweditingdelegate.h"
#include "editexperimentdialog.h"
#include <QSqlRecord>
#include <QSqlTableModel>

RowEditingDelegate::RowEditingDelegate()
{

}

QWidget *RowEditingDelegate::createEditor(QWidget *parent,
                                          const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);
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
    EditExperimentDialog *dlg = qobject_cast<EditExperimentDialog *>(editor);
    if (dlg->is_accepted){
        QSqlTableModel *mod = static_cast<QSqlTableModel *>(model);
        mod->setRecord(index.row(), dlg->rec);
        mod->submitAll();
    }
}

void RowEditingDelegate::commitAndCloseEditor()
{
    EditExperimentDialog *editor = qobject_cast<EditExperimentDialog *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
