#include "tabledelegate.h"
#include<QComboBox>
TableDelegate::TableDelegate(QObject *parent): QItemDelegate(parent)
{

}
QWidget* TableDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    if ( index.column() == 0 )
    {
        return NULL;
    }
    if ( index.column() == 1 )
    {
        QComboBox *cbx = new QComboBox(parent);
        cbx->addItem(QString("男"),0);
        cbx->addItem(QString("女"),1);
        return cbx;
    }

    return QItemDelegate::createEditor(parent, option, index);
}
