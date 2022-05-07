#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H
#include<QItemDelegate>

class TableDelegate : public QItemDelegate
{

public:
    TableDelegate(QObject *parent = Q_NULLPTR);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TABLEDELEGATE_H
